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

#include "config.h"

#include "browser-model-column-record.h"
#include "photo.h"

namespace Solang
{

enum
{
    COLUMN_PHOTO = 0,
    COLUMN_PIXBUF,
    COLUMN_TAG_NAME,
    COLUMN_COUNT
};

BrowserModelColumnRecord::BrowserModelColumnRecord() throw() :
    Gtk::TreeModelColumnRecord(),
    columnPhoto_(),
    columnPixbuf_(),
    columnTagName_()
{
    add(columnPhoto_);
    add(columnPixbuf_);
    add(columnTagName_);
}

BrowserModelColumnRecord::~BrowserModelColumnRecord() throw()
{
}

const Gtk::TreeModelColumn<PhotoPtr> &
BrowserModelColumnRecord::get_column_photo() const throw()
{
    return columnPhoto_;
}

gint
BrowserModelColumnRecord::get_column_photo_num() const throw()
{
    return COLUMN_PHOTO;
}

const Gtk::TreeModelColumn<PixbufPtr> &
BrowserModelColumnRecord::get_column_pixbuf() const throw()
{
    return columnPixbuf_;
}

gint
BrowserModelColumnRecord::get_column_pixbuf_num() const throw()
{
    return COLUMN_PIXBUF;
}

const Gtk::TreeModelColumn<Glib::ustring> &
BrowserModelColumnRecord::get_column_tag_name() const throw()
{
    return columnTagName_;
}

gint
BrowserModelColumnRecord::get_column_tag_name_num() const throw()
{
    return COLUMN_TAG_NAME;
}

} // namespace Solang
