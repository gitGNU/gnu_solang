/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * tag-view-model-column-record.cpp
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * tag-view-model-column-record.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * tag-view-model-column-record.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "tag-view-model-column-record.h"

namespace Solang
{

enum
{
    COLUMN_TAG = 0,
    COLUMN_SELECTED,
    COLUMN_PIXBUF,
    COLUMN_NAME,
    COLUMN_DESCRIPTION,
    COLUMN_COUNT
};

TagViewModelColumnRecord::TagViewModelColumnRecord() throw() :
    Gtk::TreeModelColumnRecord(),
    columnTag_(),
    columnSelected_(),
    columnPixbuf_(),
    columnName_(),
    columnDescription_()
{
    add(columnTag_);
    add(columnSelected_);
    add(columnPixbuf_);
    add(columnName_);
    add(columnDescription_);
}

TagViewModelColumnRecord::~TagViewModelColumnRecord() throw()
{
}

const Gtk::TreeModelColumn<TagPtr> &
TagViewModelColumnRecord::get_column_tag() const throw()
{
    return columnTag_;
}

gint
TagViewModelColumnRecord::get_column_tag_num() const throw()
{
    return COLUMN_TAG;
}

const Gtk::TreeModelColumn<bool> &
TagViewModelColumnRecord::get_column_selected() const throw()
{
    return columnSelected_;
}

gint
TagViewModelColumnRecord::get_column_selected_num() const throw()
{
    return COLUMN_SELECTED;
}

const Gtk::TreeModelColumn<PixbufPtr> &
TagViewModelColumnRecord::get_column_pixbuf() const throw()
{
    return columnPixbuf_;
}

gint
TagViewModelColumnRecord::get_column_pixbuf_num() const throw()
{
    return COLUMN_PIXBUF;
}

const Gtk::TreeModelColumn<Glib::ustring> &
TagViewModelColumnRecord::get_column_name() const throw()
{
    return columnName_;
}

gint
TagViewModelColumnRecord::get_column_name_num() const throw()
{
    return COLUMN_NAME;
}

const Gtk::TreeModelColumn<Glib::ustring> &
TagViewModelColumnRecord::get_column_description() const throw()
{
    return columnDescription_;
}

gint
TagViewModelColumnRecord::get_column_description_num() const throw()
{
    return COLUMN_DESCRIPTION;
}

} // namespace Solang
