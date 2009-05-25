/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
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

#include <iostream>

#include <gdkmm.h>
#include <glibmm/i18n.h>
#include <gtkimageview/gtkimagescrollwin.h>
#include <gtkimageview/gtkimageview.h>

#include "application.h"
#include "browser-model-column-record.h"
#include "enlarged-renderer.h"
#include "engine.h"
#include "main-window.h"
#include "photo.h"
#include "photo-search-criteria.h"
#include "thumbnail.h"
#include "types.h"

namespace Solang
{

static const std::string uiFile
    = PACKAGE_DATA_DIR"/"PACKAGE_NAME"/ui/"
          PACKAGE_NAME"-enlarged-renderer.ui";

EnlargedRenderer::EnlargedRenderer() throw() :
    Renderer(),
    sigc::trackable(),
    application_(NULL),
    iconFactory_(Gtk::IconFactory::create()),
    actionGroup_(Gtk::ActionGroup::create()),
    uiID_(0),
    dockItemName_("enlarged-dock-item"),
    dockItemTitle_("Enlarged"),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NO_GRIP),
    dockItem_(NULL),
    imageView_(0),
    imageScrollWin_(0),
    pageNum_(-1),
    signalInitEnd_(),
    signalItemActivated_(),
    signalSwitchPage_()
{
    Gtk::IconSource icon_source;
    Gtk::IconSet icon_set_mode_image_edit;

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME
        "/pixmaps/mode-image-edit-16.png");
    icon_source.set_size(Gtk::IconSize(16));
    icon_set_mode_image_edit.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME
        "/pixmaps/mode-image-edit-22.png");
    icon_source.set_size(Gtk::IconSize(22));
    icon_set_mode_image_edit.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME
        "/pixmaps/mode-image-edit-24.png");
    icon_source.set_size(Gtk::IconSize(24));
    icon_set_mode_image_edit.add_source(icon_source);

    iconFactory_->add(Gtk::StockID(PACKAGE_NAME"-mode-image-edit"),
                      icon_set_mode_image_edit);
    iconFactory_->add_default();

    actionGroup_->add(
        Gtk::Action::create(
            "ActionMenuGo", _("_Go")));

    {
        ActionPtr action = Gtk::Action::create(
            "ActionGoPrevious", Gtk::Stock::GO_BACK,
            _("_Previous Photo"),
            _("Go to the previous photo in the collection"));

        action->property_short_label().set_value(_("Previous"));
        action->property_is_important().set_value(true);
        actionGroup_->add(
            action, Gtk::AccelKey("<alt>Left"),
            sigc::mem_fun(*this,
                          &EnlargedRenderer::on_action_go_previous));
    }

    {
        ActionPtr action = Gtk::Action::create(
            "ActionGoNext", Gtk::Stock::GO_FORWARD,
            _("_Next Photo"),
            _("Go to the next photo in the collection"));

        action->property_short_label().set_value(_("Next"));
        action->property_is_important().set_value(true);
        actionGroup_->add(
            action, Gtk::AccelKey("<alt>Right"),
            sigc::mem_fun(*this,
                          &EnlargedRenderer::on_action_go_next));
    }

    actionGroup_->add(
        Gtk::Action::create(
            "ActionGoFirst", Gtk::Stock::GOTO_FIRST,
            _("_First Photo"),
            _("Go to the first photo in the collection")),
        Gtk::AccelKey("<alt>Home"),
        sigc::mem_fun(*this, &EnlargedRenderer::on_action_go_first));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionGoLast", Gtk::Stock::GOTO_LAST,
            _("_Last Photo"),
            _("Go to the last photo in the collection")),
        Gtk::AccelKey("<alt>End"),
        sigc::mem_fun(*this, &EnlargedRenderer::on_action_go_last));

    dockItem_ = gdl_dock_item_new_with_stock(dockItemName_.c_str(),
                    dockItemTitle_.c_str(),
                    PACKAGE_NAME"-mode-image-edit",
                    dockItemBehaviour_);
}

EnlargedRenderer::~EnlargedRenderer() throw()
{
    //g_object_unref(dockItem_);
    iconFactory_->remove_default();
}

void
EnlargedRenderer::init(Application & application) throw()
{
    application_ = &application;

    Engine & engine = application.get_engine();

    signalItemActivated_
        = engine.item_activated().connect(
              sigc::mem_fun(*this,
                            &EnlargedRenderer::on_item_activated));

    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_center(GDL_DOCK_OBJECT(dockItem_));

    signalInitEnd_
        = application.init_end().connect(
              sigc::mem_fun(*this,
                            &EnlargedRenderer::on_init_end));

    // initialized_.emit(*this);
}

void
EnlargedRenderer::render(const PhotoPtr & photo) throw()
{
    Engine & engine = application_->get_engine();
    const IStoragePtr & storage = engine.get_current_storage_system(
                                      "file");
    photo->set_disk_file_path(storage);

    PixbufPtr pixbuf;
    std::string path;

    try
    {
        path = Glib::filename_from_utf8(photo->get_disk_file_path());
    }
    catch (const Glib::ConvertError & e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
        return;
    }

    try
    {
        pixbuf = Gdk::Pixbuf::create_from_file(path);
    }
    catch (const Glib::FileError & e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
        return;
    }
    catch (const Gdk::PixbufError & e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
        return;
    }

    if (0 == imageView_)
    {
        imageView_ = gtk_image_view_new();
        if (0 == imageView_)
        {
            return;
        }

        GtkBindingSet * binding_set = gtk_binding_set_by_class(
            GTK_IMAGE_VIEW_GET_CLASS(imageView_));

        // Get rid of some of the default keybindings.

        gtk_binding_entry_remove(binding_set, GDK_Right,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_Left,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_Down,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_Up,
                                 static_cast<GdkModifierType>(0));

        Gtk::Widget * image_view = Glib::wrap(GTK_WIDGET(imageView_),
                                              false);
        image_view->signal_key_press_event().connect(
            sigc::mem_fun(*this,
                          &EnlargedRenderer::on_key_press_event));
    }

    if (0 == imageScrollWin_)
    {
        imageScrollWin_ = gtk_image_scroll_win_new(
                              GTK_IMAGE_VIEW(imageView_));
        if (0 == imageScrollWin_)
        {
            return;
        }

        gtk_container_add(GTK_CONTAINER(dockItem_), imageScrollWin_);
        gtk_widget_show_all(GTK_WIDGET(imageScrollWin_));
    }

    gtk_image_view_set_pixbuf(GTK_IMAGE_VIEW(imageView_),
                              pixbuf->gobj(), TRUE);
}

void
EnlargedRenderer::render(const PhotoList & photos) throw()
{
}

void
EnlargedRenderer::final(Application & application) throw()
{
    signalItemActivated_.disconnect();
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

    // finalized_.emit(*this);
}

PhotoList
EnlargedRenderer::get_current_selection() throw()
{
    const Gtk::TreeModel::iterator & iter
        = application_->get_list_store_iter();
    Gtk::TreeModel::Row row = *iter;

    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    PhotoList photos;
    photos.push_back(photo);

    return photos;
}

void
EnlargedRenderer::on_action_go_previous() throw()
{
    const ListStorePtr & list_store = application_->get_list_store();
    Gtk::TreeModel::iterator & iter
        = application_->get_list_store_iter();

    if (true == iter.equal(list_store->children().begin()))
    {
        iter = list_store->children().end();
    }
    iter--;

    Gtk::TreeModel::Row row = *iter;
    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    render(photo);
}

void
EnlargedRenderer::on_action_go_next() throw()
{
    const ListStorePtr & list_store = application_->get_list_store();
    Gtk::TreeModel::iterator & iter
        = application_->get_list_store_iter();

    iter++;
    if (false == iter)
    {
        iter = list_store->children().begin();
    }

    Gtk::TreeModel::Row row = *iter;
    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    render(photo);
}

void
EnlargedRenderer::on_action_go_first() throw()
{
    const ListStorePtr & list_store = application_->get_list_store();
    Gtk::TreeModel::iterator & iter
        = application_->get_list_store_iter();

    iter = list_store->children().begin();

    Gtk::TreeModel::Row row = *iter;
    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    render(photo);
}

void
EnlargedRenderer::on_action_go_last() throw()
{
    const ListStorePtr & list_store = application_->get_list_store();
    Gtk::TreeModel::iterator & iter
        = application_->get_list_store_iter();

    iter = list_store->children().end();
    iter--;

    Gtk::TreeModel::Row row = *iter;
    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    render(photo);
}

bool
EnlargedRenderer::on_key_press_event(GdkEventKey * event) throw()
{
    switch (event->keyval)
    {
        case GDK_Right:
            on_action_go_next();
            break;

        case GDK_Left:
            on_action_go_previous();
            break;

        case GDK_Home:
            on_action_go_first();
            break;

        case GDK_End:
            on_action_go_last();
            break;

        default:
            break;
    }

    return true;
}

void
EnlargedRenderer::on_init_end(Application & application) throw()
{
    MainWindow & main_window = application.get_main_window();
    Gtk::Notebook * notebook = main_window.get_notebook_center();

    if (0 == notebook)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << "0 == notebook"
                  << std::endl;
        return;
    }

    pageNum_ = notebook->page_num(*Glib::wrap(dockItem_, false));

    signalSwitchPage_
        = notebook->signal_switch_page().connect(
              sigc::mem_fun(*this,
                            &EnlargedRenderer::on_switch_page));

    signalInitEnd_.disconnect();
}

void
EnlargedRenderer::on_item_activated(const Gtk::TreeIter & iter)
                                    throw()
{
    application_->set_list_store_iter(iter);

    Gtk::TreeModel::Row row = *iter;
    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    render(photo);

    MainWindow & main_window = application_->get_main_window();
    main_window.present_dock_object(GDL_DOCK_OBJECT(dockItem_));
}

void
EnlargedRenderer::on_switch_page(GtkNotebookPage * notebook_page,
                                 guint page_num) throw()
{
    MainWindow & main_window = application_->get_main_window();
    const Glib::RefPtr<Gtk::UIManager> & ui_manager
        = main_window.get_ui_manager();

    // NB: Sometimes this gets invoked more than once consecutively
    //     -- no idea why (FIXME). Better safe than sorry.

    if (pageNum_ == static_cast<gint>(page_num))
    {
        if (0 == uiID_)
        {
            uiID_ = ui_manager->add_ui_from_file(uiFile);
            if (0 == uiID_)
            {
                // FIXME: error condition.
            }

            ui_manager->insert_action_group(actionGroup_);
        }
    }
    else
    {
        if (0 != uiID_)
        {
            ui_manager->remove_action_group(actionGroup_);
            ui_manager->remove_ui(uiID_);
            uiID_ = 0;
        }
    }
}

} // namespace Solang
