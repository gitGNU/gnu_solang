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

#include "basic-exif-view-model-column-record.h"

namespace Solang
{

enum
{
    COLUMN_KEY = 0,
    COLUMN_SELECTED,
    COLUMN_DESCRIPTION,
    COLUMN_VALUE,
    COLUMN_COUNT
};

BasicExifViewModelColumnRecord::BasicExifViewModelColumnRecord() throw() :
    Gtk::TreeModelColumnRecord(),
    key_(),
    selected_(),
    description_(),
    value_()
{
    add(key_);
    add(selected_);
    add(description_);
    add(value_);
}

BasicExifViewModelColumnRecord::~BasicExifViewModelColumnRecord() throw()
{
}

const Gtk::TreeModelColumn<ExifDataKeyPtr> &
BasicExifViewModelColumnRecord::get_column_key() const throw()
{
    return key_;
}

gint
BasicExifViewModelColumnRecord::get_column_key_num() const throw()
{
    return COLUMN_KEY;
}

const Gtk::TreeModelColumn<bool> &
BasicExifViewModelColumnRecord::get_column_selected() const throw()
{
    return selected_;
}

gint
BasicExifViewModelColumnRecord::get_column_selected_num() const throw()
{
    return COLUMN_SELECTED;
}

const Gtk::TreeModelColumn<Glib::ustring> &
BasicExifViewModelColumnRecord::get_column_description() const throw()
{
    return description_;
}

gint
BasicExifViewModelColumnRecord::get_column_description_num() const throw()
{
    return COLUMN_DESCRIPTION;
}

const Gtk::TreeModelColumn<Glib::ustring> &
BasicExifViewModelColumnRecord::get_column_value() const throw()
{
    return value_;
}

gint
BasicExifViewModelColumnRecord::get_column_value_num() const throw()
{
    return COLUMN_VALUE;
}

} // namespace Solang
