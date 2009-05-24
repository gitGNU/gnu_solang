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

EnlargedRenderer::EnlargedRenderer() throw() :
    Renderer(),
    sigc::trackable(),
    application_(NULL),
    iconFactory_(Gtk::IconFactory::create()),
    dockItemName_("enlarged-dock-item"),
    dockItemTitle_("Enlarged"),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NO_GRIP),
    dockItem_(NULL),
    imageView_(0),
    imageScrollWin_(0),
    modelIter_(),
    signalItemActivated_()
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

    // initialized_.emit(*this);
}

void
EnlargedRenderer::render(const PhotoPtr & photo) throw()
{
}

void
EnlargedRenderer::render(const PhotoList & photos) throw()
{
}

void
EnlargedRenderer::final(Application & application) throw()
{
    signalItemActivated_.disconnect();
    // finalized_.emit(*this);
}

PhotoList
EnlargedRenderer::get_current_selection() throw()
{
    Gtk::TreeModel::Row row = *modelIter_;

    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    PhotoList photos;
    photos.push_back(photo);

    return photos;
}

void
EnlargedRenderer::on_item_activated(const Gtk::TreeIter & iter)
                                    throw()
{
    modelIter_ = iter;

    Engine & engine = application_->get_engine();
    const IStoragePtr & storage = engine.get_current_storage_system(
                                      "file");

    Gtk::TreeModel::Row row = *iter;

    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];
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

    MainWindow & main_window = application_->get_main_window();
    main_window.present_dock_object(GDL_DOCK_OBJECT(dockItem_));
}

} // namespace Solang
