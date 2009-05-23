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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include<iostream>

#include "tag.h"
#include "basic-exif-view.h"
#include "exif-data.h"
#include "exif-data-key.h"

namespace Solang
{

BasicExifView::BasicExifView() throw() :
    Gtk::TreeView(),
    modelColumnRecord_(),
    listStore_(Gtk::ListStore::create(modelColumnRecord_))
{
    configure();
}

#if 0
BasicExifView::BasicExifView(const TagList & tags) throw() :
    Gtk::TreeView(),
    modelColumnRecord_(),
    listStore_(Gtk::ListStore::create(modelColumnRecord_))
{
    configure();
    populate(tags);
}
#endif

BasicExifView::~BasicExifView() throw()
{
    unset_model();
}

void
BasicExifView::configure() throw()
{
    set_model(listStore_);
    append_column("Selected", modelColumnRecord_.get_column_selected());
    append_column("Description",
                  modelColumnRecord_.get_column_description());
    append_column("Value", modelColumnRecord_.get_column_value());
    set_enable_search(true);
    // set_fixed_height_mode(true);
    set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_NONE);
    set_headers_clickable(false);
    set_headers_visible(false);
    //set_tooltip_column(modelColumnRecord_.get_column_description_num());
}

void
BasicExifView::populate(const ExifData& data) throw()
{
    listStore_->clear();

    {
        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;
        const ExifDataKeyPtr key(
                    new ExifDataKey("Aperture",
                                    "aperture", data.get_aperture()));

        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_selected()] = false;
        row[modelColumnRecord_.get_column_description()] = "Aperture";
        row[modelColumnRecord_.get_column_value()] = data.get_aperture();
    }

    {
        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;
        const ExifDataKeyPtr key(
                new ExifDataKey("Shutter Speed",
                                "shutter_speed",
                                data.get_shutter_speed()));
        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_selected()] = false;
        row[modelColumnRecord_.get_column_description()] = "Shutter Speed";
        row[modelColumnRecord_.get_column_value()] = data.get_shutter_speed();
    }

    {
        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;
        const ExifDataKeyPtr key(
                new ExifDataKey("Exposure Program",
                                "exposure_program" ,
                                data.get_exposure_program()));
        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_selected()] = false;
        row[modelColumnRecord_.get_column_description()] = "Exposure Program";
        row[modelColumnRecord_.get_column_value()] = data.get_exposure_program();
    }

    {
        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;
        std::ostringstream sout;
        sout<<data.get_iso_speed();
        const ExifDataKeyPtr key(
                new ExifDataKey("ISO Rating",
                                "iso_speed", sout.str() ) );
        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_selected()] = false;
        row[modelColumnRecord_.get_column_description()] = "ISO Rating";
        row[modelColumnRecord_.get_column_value()] = sout.str();
    }

    {
        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;
        const ExifDataKeyPtr key(
                new ExifDataKey("Exposure Metering Mode",
                                "metering_mode" ,
                                data.get_exposure_metering_mode()));
        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_selected()] = false;
        row[modelColumnRecord_.get_column_description()]
                                = "Exposure Metering Mode";
        row[modelColumnRecord_.get_column_value()]
                                = data.get_exposure_metering_mode();
    }

    {
        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;
        const ExifDataKeyPtr key(
                new ExifDataKey("Actual Focal Length",
                                "focal_length" ,
                                data.get_focal_length()));
        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_selected()] = false;
        row[modelColumnRecord_.get_column_description()]
                                = "Actual Focal Length";
        row[modelColumnRecord_.get_column_value()]
                                = data.get_focal_length();
    }

    {
        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;
        const ExifDataKeyPtr key(
                new ExifDataKey("White Balance",
                                "white_balance" ,
                                data.get_white_balance()));
        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_selected()] = false;
        row[modelColumnRecord_.get_column_description()]
                                = "White Balance";
        row[modelColumnRecord_.get_column_value()]
                                = data.get_white_balance();
    }

    {
        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;
        const ExifDataKeyPtr key(
                new ExifDataKey("35mm Camera Focal Length",
                                "focal_length_in_film" ,
                                data.get_focal_length_film()));
        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_selected()] = false;
        row[modelColumnRecord_.get_column_description()]
                                = "35mm Camera Focal Length";
        row[modelColumnRecord_.get_column_value()]
                                = data.get_focal_length_film();
    }
}

void
BasicExifView::on_row_activated ( const Gtk::TreeModel::Path& path,
                            Gtk::TreeViewColumn* column )
{
    TreeView::on_row_activated( path, column);

    Gtk::TreeModel::iterator current = listStore_->get_iter( path );
    Gtk::TreeModel::Row row = (*current);

    row[modelColumnRecord_.get_column_selected()]
            = !( row[modelColumnRecord_.get_column_selected()] ) ;
}

void
BasicExifView::get_criterion(
                PhotoSearchCriteriaList &selectedTags) const throw()
{

    Gtk::TreeModel::Children children = listStore_->children();
    for( Gtk::TreeModel::const_iterator current = children.begin();
                                current != children.end(); current++)
    {
        Gtk::TreeModel::Row row = (*current);

        if( row[modelColumnRecord_.get_column_selected()] )
        {
            ExifDataKeyPtr key
                = row[modelColumnRecord_.get_column_key()];

            selectedTags.push_back( key );
        }
    }
    return ;
}

void
BasicExifView::clear_selection() throw()
{
    Gtk::TreeModel::Children children = listStore_->children();

    for( Gtk::TreeModel::const_iterator current = children.begin();
                                current != children.end(); current++)
    {
        Gtk::TreeModel::Row row = (*current);

        row[modelColumnRecord_.get_column_selected()] = false;
    }
    return;

}

} // namespace Solang
