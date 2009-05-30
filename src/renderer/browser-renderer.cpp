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

#include "application.h"
#include "browser-renderer.h"
#include "engine.h"
#include "main-window.h"
#include "photo.h"
#include "photo-search-criteria.h"
#include "thumbnail.h"
#include "types.h"

namespace Solang
{

BrowserRenderer::BrowserRenderer() throw() :
    Renderer(),
    sigc::trackable(),
    application_(NULL),
    iconFactory_(Gtk::IconFactory::create()),
    dockItemName_("browser-dock-item"),
    dockItemTitle_("Browser"),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NO_GRIP),
    dockItem_(NULL),
    scrolledWindow_(),
    thumbnailView_(),
    pageNum_(-1),
    signalInitEnd_(),
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

    scrolledWindow_.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC);

    scrolledWindow_.add(thumbnailView_);

    dockItem_ = gdl_dock_item_new_with_stock(
                    dockItemName_.c_str(),
                    dockItemTitle_.c_str(),
                    PACKAGE_TARNAME"-mode-browse",
                    dockItemBehaviour_);
    gtk_container_add(GTK_CONTAINER(dockItem_),
                      GTK_WIDGET(scrolledWindow_.gobj()));
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
    thumbnailView_.set_model(list_store);

    Engine & engine = application.get_engine();

    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_center(GDL_DOCK_OBJECT(dockItem_));

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
    signalSelectionChanged_.disconnect();
    signalSwitchPage_.disconnect();
    // finalized_.emit(*this);
}

PhotoList
BrowserRenderer::get_current_selection() throw()
{
    return thumbnailView_.get_selected_photos();
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
    const TreeModelPtr & model = thumbnailView_.get_model();
    g_object_ref(model->gobj());

    Gtk::TreeModel::iterator model_iter = model->get_iter(path);
    application_->get_engine().item_activated().emit(model_iter);
}

void
BrowserRenderer::on_switch_page(GtkNotebookPage * notebook_page,
                                guint page_num) throw()
{
    if (pageNum_ != static_cast<gint>(page_num))
    {
        signalSelectionChanged_.block();
        return;
    }

    Engine & engine = application_->get_engine();
    RendererPtr renderer = application_->get_renderer(
                               "browser-renderer");
    engine.set_current_renderer(renderer);

    const ListStorePtr & list_store = application_->get_list_store();
    const Gtk::TreeModel::iterator & iter
        = application_->get_list_store_iter();

    if (true == iter)
    {
        const Gtk::TreeModel::Path path = list_store->get_path(iter);

        if (false == path.empty())
        {
            thumbnailView_.unselect_all();
            thumbnailView_.scroll_to_path(path, false, 0, 0);
            thumbnailView_.select_path(path);
        }
    }

    signalSelectionChanged_.unblock();
}

} // namespace Solang
