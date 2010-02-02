/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009, 2010 Debarshi Ray <rishi@gnu.org>
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

#include <iostream>

#include <glibmm/i18n.h>

#include "application.h"
#include "basic-exif-view.h"
#include "exif-data.h"
#include "exif-data-key.h"

namespace Solang
{

BasicExifView::BasicExifView() throw() :
    Gtk::TreeView(),
    application_(),
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
    append_column("Description",
                  modelColumnRecord_.get_column_description());
    append_column("Value", modelColumnRecord_.get_column_value());
    set_enable_search(true);
    // set_fixed_height_mode(true);
    set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_NONE);
    set_headers_clickable(false);
    set_headers_visible(false);

    std::vector<Gtk::TargetEntry> targets;
    targets.push_back(Gtk::TargetEntry("STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    targets.push_back(Gtk::TargetEntry("UTF8_STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    enable_model_drag_source(targets,
                            Gdk::MODIFIER_MASK, Gdk::ACTION_COPY);

}

void
BasicExifView::populate(const ExifData& data) throw()
{
    listStore_->clear();

    const Glib::ustring & camera_data = data.get_camera();

    if (false == camera_data.empty())
    {
        const Glib::ustring camera_desc = _("Camera");
        const ExifDataKeyPtr key(new ExifDataKey(camera_desc,
                                                 "nmm:camera",
                                                 camera_data));

        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;

        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_description()]
                                   = camera_desc;
        row[modelColumnRecord_.get_column_value()] = camera_data;
    }

    const Glib::ustring & exposure_time_data
                              = data.get_exposure_time();

    if (false == exposure_time_data.empty())
    {
        const Glib::ustring exposure_time_desc = _("Exposure time");
        const ExifDataKeyPtr key(new ExifDataKey(
                                         exposure_time_desc,
                                         "nmm:exposureTime",
                                         data.get_exposure_time()));

        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;

        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_description()]
                                   = exposure_time_desc;
        row[modelColumnRecord_.get_column_value()]
                                   = exposure_time_data;
    }

    const Glib::ustring & flash_data = data.get_flash();

    if (false == flash_data.empty())
    {
        const Glib::ustring flash_desc = _("Flash");
        const ExifDataKeyPtr key(new ExifDataKey(
                                     flash_desc,
                                     flash_data,
                                     "nmm:flash",
                                     data.get_flash_enum()));

        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;

        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_description()]
                                   = flash_desc;
        row[modelColumnRecord_.get_column_value()] = flash_data;
    }

    const Glib::ustring & fnumber_data = data.get_fnumber();

    if (false == fnumber_data.empty())
    {
        const Glib::ustring fnumber_desc = _("F-number");
        const ExifDataKeyPtr key(new ExifDataKey(fnumber_desc,
                                                 "nmm:fnumber",
                                                 fnumber_data));

        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;

        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_description()]
                                   = fnumber_desc;
        row[modelColumnRecord_.get_column_value()]
                                   = data.get_fnumber();
    }

    const Glib::ustring & iso_speed_data = data.get_iso_speed();

    if (false == iso_speed_data.empty())
    {
        const Glib::ustring iso_speed_desc = _("ISO speed");
        const ExifDataKeyPtr key(new ExifDataKey(iso_speed_desc,
                                                 "nmm:isoSpeed",
                                                 iso_speed_data));

        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;

        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_description()]
                                   = iso_speed_desc;
        row[modelColumnRecord_.get_column_value()] = iso_speed_data;
    }

    const Glib::ustring & metering_mode_data
                              = data.get_metering_mode();

    if (false == metering_mode_data.empty())
    {
        const Glib::ustring metering_mode_desc = _("Metering mode");
        const ExifDataKeyPtr key(new ExifDataKey(
                                     metering_mode_desc,
                                     metering_mode_data,
                                     "nmm:meteringMode",
                                     data.get_metering_mode_enum()));

        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;

        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_description()]
                                   = metering_mode_desc;
        row[modelColumnRecord_.get_column_value()]
                                   = metering_mode_data;
    }

    const Glib::ustring & focal_length_data = data.get_focal_length();

    if (false == focal_length_data.empty())
    {
        const Glib::ustring focal_length_desc = _("Focal length");
        const ExifDataKeyPtr key(new ExifDataKey(focal_length_desc,
                                                 "nmm:focalLength",
                                                 focal_length_data));

        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;

        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_description()]
                                   = focal_length_desc;
        row[modelColumnRecord_.get_column_value()]
                                   = focal_length_data;
    }

    const Glib::ustring & white_balance_data
                              = data.get_white_balance();

    if (false == white_balance_data.empty())
    {
        const Glib::ustring white_balance_desc = _("White balance");
        const ExifDataKeyPtr key(new ExifDataKey(
                                     white_balance_desc,
                                     white_balance_data,
                                     "nmm:whiteBalance",
                                     data.get_white_balance_enum()));

        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;

        row[modelColumnRecord_.get_column_key()] = key;
        row[modelColumnRecord_.get_column_description()]
                                   = white_balance_desc;
        row[modelColumnRecord_.get_column_value()]
                                   = white_balance_data;
    }
}

void
BasicExifView::on_drag_data_get(
          const Glib::RefPtr<Gdk::DragContext>& context,
          Gtk::SelectionData& selection_data, guint info, guint time)
{
    if( !application_ )
    {
        return;
    }
    const TreeSelectionPtr selected = get_selection();
    if( 0 == selected->count_selected_rows() )
        return;

    Gtk::TreeModel::iterator item = selected->get_selected();

    Gtk::TreeModel::Row row= (*item);
    ExifDataKeyPtr key = row[modelColumnRecord_.get_column_key()];
    Glib::ustring criteria = key->get_query_criteria();
    application_->set_drag_item( criteria, key );
    selection_data.set_text( criteria );

    return;

}

void
BasicExifView::set_application( ApplicationPtr application ) throw()
{
    application_ = application;
}

} // namespace Solang
