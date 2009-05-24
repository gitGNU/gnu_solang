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

#include "date-view-model-column-record.h"

namespace Solang
{

enum
{
    COLUMN_DATE = 0,
    COLUMN_ENTRY,
    COLUMN_PHOTO_COUNT,
    COLUMN_PERCENTAGE,
    COLUMN_COUNT
};

DateViewModelColumnRecord::DateViewModelColumnRecord() throw() :
    Gtk::TreeModelColumnRecord(),
    date_(),
    entry_(),
    photoCount_(),
	percentage_()
{
    add(date_);
    add(entry_);
    add(photoCount_);
	add(percentage_);
}

DateViewModelColumnRecord::~DateViewModelColumnRecord() throw()
{
}

const Gtk::TreeModelColumn<ModificationDate> &
DateViewModelColumnRecord::get_column_date() const throw()
{
    return date_;
}

gint
DateViewModelColumnRecord::get_column_date_num() const throw()
{
    return COLUMN_DATE;
}

const Gtk::TreeModelColumn<Glib::ustring> &
DateViewModelColumnRecord::get_column_entry() const throw()
{
    return entry_;
}

gint
DateViewModelColumnRecord::get_column_entry_num() const throw()
{
    return COLUMN_ENTRY;
}

const Gtk::TreeModelColumn<gint> &
DateViewModelColumnRecord::get_column_photo_count() const throw()
{
    return photoCount_;
}

gint
DateViewModelColumnRecord::get_column_photo_count_num() const throw()
{
    return COLUMN_PHOTO_COUNT;
}

const Gtk::TreeModelColumn<gdouble> &
DateViewModelColumnRecord::get_column_percentage() const throw()
{
    return percentage_;
}

gint
DateViewModelColumnRecord::get_column_percentage_num() const throw()
{
    return COLUMN_PERCENTAGE;
}

} // namespace Solang
