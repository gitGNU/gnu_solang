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
    modelColumnRecord_(),
    listStore_(Gtk::ListStore::create(modelColumnRecord_)),
    iconFactory_(Gtk::IconFactory::create()),
    dockItemName_("browser-dock-item"),
    dockItemTitle_("Browser"),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NO_GRIP),
    dockItem_(NULL),
    scrolledWindow_(),
    thumbnailView_(listStore_),
    photoRenderBegin_()
{
    Gtk::IconSource icon_source;
    Gtk::IconSet icon_set_mode_browse;

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/mode-browse-16.png");
    icon_source.set_size(Gtk::IconSize(16));
    icon_set_mode_browse.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/mode-browse-22.png");
    icon_source.set_size(Gtk::IconSize(22));
    icon_set_mode_browse.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/tag-new-24.png");
    icon_source.set_size(Gtk::IconSize(24));
    icon_set_mode_browse.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/mode-browse-32.png");
    icon_source.set_size(Gtk::IconSize(32));
    icon_set_mode_browse.add_source(icon_source);

    iconFactory_->add(Gtk::StockID(PACKAGE_NAME"-mode-browse"),
                      icon_set_mode_browse);
    iconFactory_->add_default();

    scrolledWindow_.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC);

    scrolledWindow_.add(thumbnailView_);

    dockItem_ = gdl_dock_item_new_with_stock(dockItemName_.c_str(),
                                             dockItemTitle_.c_str(),
                                             PACKAGE_NAME"-mode-browse",
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

    Engine & engine = application.get_engine();
    photoRenderBegin_ = engine.photo_render_begin().connect(
        sigc::mem_fun(*this, &BrowserRenderer::on_photo_render_begin));

    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_center(GDL_DOCK_OBJECT(dockItem_));

    Glib::ThreadPool & thread_pool = application.get_thread_pool();
    PhotoSearchCriteriaList criterion;

    thread_pool.push(sigc::bind(
        sigc::mem_fun1<const PhotoSearchCriteriaList &>(
            engine, &Engine::show), criterion));

    // initialized_.emit(*this);
}

void
BrowserRenderer::render(const PhotoPtr & photo) throw()
{
    const Thumbnail & thumbnail = photo->get_thumbnail();
    PixbufPtr pixbuf;
    std::string path;

    try
    {
        path = Glib::filename_from_utf8(thumbnail.get_path());
    }
    catch (const Glib::ConvertError & e)
    {
        return;
    }

    try
    {
        pixbuf = Gdk::Pixbuf::create_from_file(path, -1, 120, true);
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

    Gtk::TreeModel::iterator model_iter = listStore_->append();
    Gtk::TreeModel::Row row = *model_iter;

    row[modelColumnRecord_.get_column_photo()] = photo;
    row[modelColumnRecord_.get_column_pixbuf()] = pixbuf;
    row[modelColumnRecord_.get_column_tag_name()] 
        = photo->get_exif_data().get_picture_taken_time();
}

void
BrowserRenderer::render(const PhotoList & photos) throw()
{
    PhotoList::const_iterator list_iter;

    listStore_->clear();

    for (list_iter = photos.begin(); list_iter != photos.end();
         list_iter++)
    {
        render(*list_iter);
        while (true == Gtk::Main::events_pending())
        {
            Gtk::Main::iteration();
        }
    }
}

void
BrowserRenderer::final(Application & application) throw()
{
    photoRenderBegin_.disconnect();
    // finalized_.emit(*this);
}

void
BrowserRenderer::on_photo_render_begin() throw()
{
    Engine & engine = application_->get_engine();
    PhotoList photos = engine.get_photos();
    render(photos);
}

} // namespace Solang
