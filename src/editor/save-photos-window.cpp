/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Santanu Sinha <santanu.sinha@gmail.com>
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

#include <glibmm/i18n.h>

#include "editable-photo.h"
#include "save-photos-window.h"

namespace Solang
{
SavePhotosWindow::SavePhotosWindow( Engine &engine,
                        const EditablePhotoList &photos )
    :Gtk::Dialog( _("Unsaved Photos"), true ),
    engine_( engine ),
    lblPhotos_( _("Unsaved photos") ),
    photosView_(),
    modifiedPhotos_(),
    photosModel_(
        Gtk::ListStore::create( modifiedPhotos_ ))
{
    set_default_size(320, 200);
    set_has_separator(false);
    Gtk::VBox * const vBox = get_vbox();
    vBox->set_spacing(18);
    vBox->pack_start( photosView_, Gtk::PACK_EXPAND_WIDGET );
    photosView_.set_model( photosModel_ );
    photosView_.append_column( "", modifiedPhotos_.select_ );
    photosView_.append_column( _("Preview"), modifiedPhotos_.preview_ );
    photosView_.append_column( _("Name"), modifiedPhotos_.name_ );
    add_button( Gtk::Stock::CANCEL, Gtk::RESPONSE_REJECT);
    add_button( Gtk::Stock::SAVE, Gtk::RESPONSE_OK);

    //Signal handlers
    photosView_.signal_row_activated().connect(
        sigc::mem_fun( *this,
            &SavePhotosWindow::on_row_activated));

    //Populate
    for( EditablePhotoList::const_iterator photo = photos.begin();
                            photo != photos.end(); photo++ )
    {
        if( !(*photo)->get_photo()->get_has_unsaved_data() )
            continue;

        Gtk::TreeModel::Row row = *(photosModel_->append());
        row[ modifiedPhotos_.select_ ] = true;
        (*photo)->set_to_save(true);
        row[ modifiedPhotos_.preview_ ] = (*photo)->get_photo()
                                    ->get_thumbnail_buffer()
                                    ->scale_simple(32,32,
                                                Gdk::INTERP_NEAREST);
        row[ modifiedPhotos_.name_ ] =
                        Glib::filename_display_basename(
                            Glib::filename_from_uri(
                                (*photo)->get_photo()->get_uri()));
        row[ modifiedPhotos_.photo_ ] = *photo;
    }
    show_all_children();
}

SavePhotosWindow::~SavePhotosWindow()
{
}

void
SavePhotosWindow::on_row_activated(
        const Gtk::TreeModel::Path& path,
                Gtk::TreeViewColumn* column)
{
    Gtk::TreeModel::iterator current
                    = photosModel_->get_iter( path );
    Gtk::TreeModel::Row row = (*current);
    row[ modifiedPhotos_.select_ ] = !row[ modifiedPhotos_.select_ ];
    EditablePhotoPtr photo = row[ modifiedPhotos_.photo_ ];
    photo->set_to_save( row[ modifiedPhotos_.select_ ] );
}

} //namespace Solang

