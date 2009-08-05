/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
 *
 * Solang is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solang is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <sstream>

#include <glibmm/i18n.h>

#include "application.h"
#include "browser-model-column-record.h"
#include "browser-renderer.h"
#include "engine.h"
#include "i-plugin.h"
#include "main-window.h"
#include "photo.h"
#include "photo-search-criteria.h"
#include "thumbbuf-maker.h"

namespace Solang
{

static const guint lowerZoomValue = 20;
static const guint higherZoomValue = 100;
static const guint initialZoomValue
                       = lowerZoomValue
                         + (higherZoomValue - lowerZoomValue) / 2;

static const gint thumbnailRendererWidth = 168;
static const gint thumbnailRendererHeight = 130;

static const double ratioWidth
    = static_cast<double>(thumbnailRendererWidth - 12)
      / static_cast<double>(initialZoomValue);
static const double ratioHeight
    = static_cast<double>(thumbnailRendererHeight - 12)
      / static_cast<double>(initialZoomValue);

static const std::string uiFile
    = PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/ui/"
          PACKAGE_TARNAME"-browser-renderer.ui";

BrowserRenderer::BrowserRenderer() throw() :
    IRenderer(),
    sigc::trackable(),
    application_(NULL),
    iconFactory_(Gtk::IconFactory::create()),
    actionGroup_(Gtk::ActionGroup::create()),
    uiID_(0),
    dockItemName_("browser-dock-item"),
    dockItemTitle_("Browser"),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NO_GRIP),
    dockItem_(NULL),
    vBox_(false, 6),
    hBox_(false, 12),
    paginationBar_(),
    dummyLabel_(),
    zoomer_(lowerZoomValue, higherZoomValue, initialZoomValue, 5.0),
    scrolledWindow_(),
    treeModelFilter_(),
    thumbnailView_(thumbnailRendererWidth, thumbnailRendererHeight),
    pageNum_(-1),
    signalInitEnd_(),
    signalListStoreChangeBegin_(),
    signalListStoreChangeEnd_(),
    signalSelectionChanged_(),
    signalSwitchPage_()
{
    Gtk::IconSource icon_source;
    Gtk::IconSet icon_set_mode_browse;

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME
            "/pixmaps/mode-browse-16.png");
    icon_source.set_size(Gtk::IconSize(16));
    icon_set_mode_browse.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME
            "/pixmaps/mode-browse-22.png");
    icon_source.set_size(Gtk::IconSize(22));
    icon_set_mode_browse.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/tag-new-24.png");
    icon_source.set_size(Gtk::IconSize(24));
    icon_set_mode_browse.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME
            "/pixmaps/mode-browse-32.png");
    icon_source.set_size(Gtk::IconSize(32));
    icon_set_mode_browse.add_source(icon_source);

    iconFactory_->add(Gtk::StockID(PACKAGE_TARNAME"-mode-browse"),
                      icon_set_mode_browse);
    iconFactory_->add_default();

    actionGroup_->add(
        zoomer_.action_zoom_in(),
        Gtk::AccelKey("<control>plus"));

    actionGroup_->add(
        zoomer_.action_zoom_out(),
        Gtk::AccelKey("<control>minus"));

    actionGroup_->add(Gtk::Action::create(
        "ActionViewEdit", Gtk::Stock::EDIT, _("_Edit"),
        _("Edit picture")), Gtk::AccelKey("<control>e"),
        sigc::mem_fun(*this, &BrowserRenderer::on_item_edit));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionMenuGo", _("_Go")));

    actionGroup_->add(
        paginationBar_.action_previous(),
        Gtk::AccelKey("<alt>Page_Up"));

    actionGroup_->add(
        paginationBar_.action_next(),
        Gtk::AccelKey("<alt>Page_Down"));

    actionGroup_->add(
        paginationBar_.action_first(),
        Gtk::AccelKey("<alt>Home"));

    actionGroup_->add(
        paginationBar_.action_last(),
        Gtk::AccelKey("<alt>End"));

    hBox_.pack_start(paginationBar_, Gtk::PACK_SHRINK, 0);
    hBox_.pack_start(dummyLabel_, Gtk::PACK_EXPAND_PADDING, 0);
    hBox_.pack_start(zoomer_, Gtk::PACK_SHRINK, 0);

    vBox_.pack_start(hBox_, Gtk::PACK_SHRINK, 0);

    scrolledWindow_.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC);

    scrolledWindow_.add(thumbnailView_);
    vBox_.pack_start(scrolledWindow_, Gtk::PACK_EXPAND_WIDGET, 0);

    dockItem_ = gdl_dock_item_new_with_stock(
                    dockItemName_.c_str(),
                    dockItemTitle_.c_str(),
                    PACKAGE_TARNAME"-mode-browse",
                    dockItemBehaviour_);
    gtk_container_add(GTK_CONTAINER(dockItem_),
                      GTK_WIDGET(vBox_.gobj()));

    paginationBar_.limits_changed().connect(
        sigc::mem_fun(*this,
                      &BrowserRenderer::on_limits_changed));

    zoomer_.get_scale().signal_value_changed().connect(
        sigc::mem_fun(*this,
                      &BrowserRenderer::on_signal_value_changed));
}

BrowserRenderer::~BrowserRenderer() throw()
{
    //g_object_unref(dockItem_);
    iconFactory_->remove_default();
}

void
BrowserRenderer::init(Application & application) throw()
{
    application_ = &application;

    const ListStorePtr & list_store = application.get_list_store();

    treeModelFilter_ = Gtk::TreeModelFilter::create(list_store);
    treeModelFilter_->set_visible_func(sigc::mem_fun(*this,
                                       &BrowserRenderer::on_visible));
    thumbnailView_.set_model(treeModelFilter_);

    Engine & engine = application.get_engine();

    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_center(GDL_DOCK_OBJECT(dockItem_));

    const Glib::RefPtr<Gtk::UIManager> & ui_manager
        = main_window.get_ui_manager();

    uiID_ = ui_manager->add_ui_from_file(uiFile);
    if (0 == uiID_)
    {
        // FIXME: error condition.
    }

    ui_manager->insert_action_group(actionGroup_);

    Glib::ThreadPool & thread_pool = application.get_thread_pool();
    PhotoSearchCriteriaList criterion;

    thread_pool.push(sigc::bind(
        sigc::mem_fun1<const PhotoSearchCriteriaList &>(
            engine, &Engine::show), criterion));

    signalInitEnd_
        = application.init_end().connect(
              sigc::mem_fun(*this,
                            &BrowserRenderer::on_init_end));

    // NB: This should not be done in the constructor because if
    //     'false == application_' then the handler will crash.
    //     Better safe than sorry.
    signalItemActivated_
        = thumbnailView_.signal_item_activated().connect(
              sigc::mem_fun(*this,
                            &BrowserRenderer::on_item_activated));

    signalListStoreChangeBegin_
        = application.list_store_change_begin().connect(
              sigc::mem_fun(*this,
                            &BrowserRenderer::on_list_store_change_begin));

    signalListStoreChangeEnd_
        = application.list_store_change_end().connect(
              sigc::mem_fun(*this,
                            &BrowserRenderer::on_list_store_change_end));

    signalSelectionChanged_
        = thumbnailView_.signal_selection_changed().connect(
              sigc::mem_fun(*this,
                            &BrowserRenderer::on_selection_changed));

    // initialized_.emit(*this);
}

void
BrowserRenderer::render(const PhotoPtr & photo) throw()
{
}

void
BrowserRenderer::render(const PhotoList & photos) throw()
{
}

void
BrowserRenderer::final(Application & application) throw()
{
    signalItemActivated_.disconnect();
    signalListStoreChangeBegin_.disconnect();
    signalListStoreChangeEnd_.disconnect();
    signalSelectionChanged_.disconnect();
    signalSwitchPage_.disconnect();

    MainWindow & main_window = application.get_main_window();
    const Glib::RefPtr<Gtk::UIManager> & ui_manager
        = main_window.get_ui_manager();

    if (0 != uiID_)
    {
        ui_manager->remove_action_group(actionGroup_);
        ui_manager->remove_ui(uiID_);
        uiID_ = 0;
    }

    treeModelFilter_.reset();

    // finalized_.emit(*this);
}

void
BrowserRenderer::clear_thumbnails() throw()
{
    Gtk::TreeModel::Children children
        = treeModelFilter_->get_model()->children();
    Gtk::TreeModel::iterator iter;

    for (iter = children.begin(); children.end() != iter; iter++)
    {
        BrowserModelColumnRecord model_column_record;
        Gtk::TreeModel::Row row = *iter;

        row[model_column_record.get_column_pixbuf()] = PixbufPtr(0);
        PhotoPtr photo = row[model_column_record.get_column_photo()];
        if( !photo->get_has_unsaved_data() )
        {
            photo->set_thumbnail_buffer( PixbufPtr(0) );
        }
        row[model_column_record.get_column_tag_name()] = "";

        while (true == Gtk::Main::events_pending())
        {
            Gtk::Main::iteration();
        }
    }
}

void
BrowserRenderer::generate_thumbnails() throw()
{
    std::ostringstream fsout;
    fsout << paginationBar_.get_lower_limit();

    const Gtk::TreeModel::Path first_path(fsout.str());

    if (true == first_path.empty())
    {
        return;
    }

    const Gtk::TreeModel::iterator first_iter
        = treeModelFilter_->get_model()->get_iter(first_path);

    if (false == first_iter)
    {
        return;
    }

    std::ostringstream lsout;
    lsout << paginationBar_.get_upper_limit() - 1;

    const Gtk::TreeModel::Path last_path(lsout.str());

    if (true == last_path.empty())
    {
        return;
    }

    const Gtk::TreeModel::iterator last_iter
        = treeModelFilter_->get_model()->get_iter(last_path);

    if (false == last_iter)
    {
        return;
    }

    const guint zoom_value = zoomer_.get_scale().get_value();
    const guint thumbnail_width
                    = static_cast<guint>(
                          ratioWidth
                          * static_cast<double>(zoom_value));
    const guint thumbnail_height
                    = static_cast<guint>(
                          ratioHeight
                          * static_cast<double>(zoom_value));

    thumbnailView_.set_thumbnail_width(thumbnail_width + 12);
    thumbnailView_.set_thumbnail_height(thumbnail_height + 12);

    Gtk::TreeModel::iterator iter;

    // operator<= is not defined.
    for (iter = first_iter; ; iter++)
    {
        BrowserModelColumnRecord model_column_record;
        Gtk::TreeModel::Row row = *iter;

        const PhotoPtr & photo
            = row[model_column_record.get_column_photo()];
        const PixbufPtr & pixbuf
            = row[model_column_record.get_column_pixbuf()];

        if (0 == photo->get_thumbnail_buffer() )
        {
            ThumbbufMaker thumbbuf_maker(thumbnail_width,
                                         thumbnail_height);

            photo->set_thumbnail_buffer( thumbbuf_maker(photo) );

            row[model_column_record.get_column_pixbuf()]
                = photo->get_thumbnail_buffer();

            row[model_column_record.get_column_tag_name()]
                = photo->get_exif_data().get_picture_taken_time();
        }
        else
        {
            row[model_column_record.get_column_pixbuf()]
                = photo->get_thumbnail_buffer();
        }

        // operator<= is not defined.
        if (last_iter == iter)
        {
            break;
        }
    }
}

PhotoList
BrowserRenderer::get_current_selection() throw()
{
    return thumbnailView_.get_selected_photos();
}

std::string
BrowserRenderer::get_name() const throw()
{
    return "browser-renderer";
}

void
BrowserRenderer::receive_plugin(IPlugin & plugin) throw()
{
    plugin.visit_renderer(*this);
}

void
BrowserRenderer::on_init_end(Application & application) throw()
{
    MainWindow & main_window = application.get_main_window();
    Gtk::Notebook * notebook = main_window.get_notebook_center();

    if (0 == notebook)
    {
        g_warning("0 == notebook");
        return;
    }

    pageNum_ = notebook->page_num(*Glib::wrap(dockItem_, false));

    signalSwitchPage_
        = notebook->signal_switch_page().connect(
              sigc::mem_fun(*this,
                            &BrowserRenderer::on_switch_page));

    signalInitEnd_.disconnect();
}

void
BrowserRenderer::on_selection_changed() throw()
{
    application_->get_engine().selection_changed().emit();
}

void
BrowserRenderer::on_item_activated(const Gtk::TreeModel::Path & path)
                                   throw()
{
    const Gtk::TreeModel::iterator filter_iter
                                       = treeModelFilter_->get_iter(path);
    const Gtk::TreeModel::iterator model_iter
        = treeModelFilter_->convert_iter_to_child_iter(filter_iter);

    application_->get_engine().item_activated().emit(model_iter);
}

void
BrowserRenderer::on_item_edit() throw()
{
    application_->get_engine().item_edit().emit(
                            thumbnailView_.get_selected_photos() );
}

void
BrowserRenderer::on_limits_changed() throw()
{
    generate_thumbnails();
    treeModelFilter_->refilter();
}

void
BrowserRenderer::on_list_store_change_begin(Application & application)
                                            throw()
{
    thumbnailView_.set_model(TreeModelPtr());
}

void
BrowserRenderer::on_list_store_change_end(Application & application)
                                          throw()
{
    const ListStorePtr & list_store = application.get_list_store();
    const Gtk::TreeModel::Children children = list_store->children();
    guint total;

    if (true == children.empty())
    {
        total = 0;
    }
    else
    {
        total = children.size();
    }

    thumbnailView_.set_model(treeModelFilter_);
    paginationBar_.set_total(total);
}

void
BrowserRenderer::on_signal_value_changed() throw()
{
    static sigc::connection connection;

    connection.disconnect();
    connection
        = Glib::signal_timeout().connect_seconds(
              sigc::bind_return(
                  sigc::mem_fun(*this,
                                &BrowserRenderer::reload), false),
              1, Glib::PRIORITY_DEFAULT);
}

void
BrowserRenderer::on_switch_page(GtkNotebookPage * notebook_page,
                                guint page_num) throw()
{
    MainWindow & main_window = application_->get_main_window();
    const Glib::RefPtr<Gtk::UIManager> & ui_manager
        = main_window.get_ui_manager();

    // NB: Sometimes this gets invoked more than once consecutively
    //     -- no idea why (FIXME). Better safe than sorry.

    if (pageNum_ == static_cast<gint>(page_num))
    {
        Engine & engine = application_->get_engine();
        IRendererPtr renderer = application_->get_renderer(
                                    "browser-renderer");
        engine.set_current_renderer(renderer);

        const Gtk::TreeModel::iterator & model_iter
            = application_->get_list_store_iter();

        if (true == model_iter)
        {
            const Gtk::TreeModel::Row row = *model_iter;
            BrowserModelColumnRecord model_column_record;
            const guint serial
                = row[model_column_record.get_column_serial()];

            paginationBar_.scroll_to_position(serial);

            const Gtk::TreeModel::iterator filter_iter
                = treeModelFilter_->convert_child_iter_to_iter(
                                        model_iter);
            const Gtk::TreeModel::Path path
                = treeModelFilter_->get_path(filter_iter);

            if (false == path.empty())
            {
                thumbnailView_.unselect_all();
                thumbnailView_.scroll_to_path(path, false, 0, 0);
                thumbnailView_.select_path(path);
            }
        }

        signalSelectionChanged_.unblock();

        if (0 == uiID_)
        {
            uiID_ = ui_manager->add_ui_from_file(uiFile);
            if (0 == uiID_)
            {
                // FIXME: error condition.
            }

            ui_manager->insert_action_group(actionGroup_);
        }
        reload();
    }
    else
    {
        if (0 != uiID_)
        {
            ui_manager->remove_action_group(actionGroup_);
            ui_manager->remove_ui(uiID_);
            uiID_ = 0;
        }

        signalSelectionChanged_.block();
    }
}

bool
BrowserRenderer::on_visible(
                     const Gtk::TreeModel::const_iterator & iter)
                     throw()
{
    const Gtk::TreeModel::Row row = *iter;
    BrowserModelColumnRecord model_column_record;
    const guint serial = row[model_column_record.get_column_serial()];

    return (serial >= paginationBar_.get_lower_limit()
            && serial < paginationBar_.get_upper_limit())
            ? true : false;
}

void
BrowserRenderer::reload() throw()
{
    clear_thumbnails();
    generate_thumbnails();
    treeModelFilter_->refilter();
}

} // namespace Solang
