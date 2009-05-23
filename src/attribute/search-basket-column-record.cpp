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

#include "search-basket-column-record.h"

namespace Solang
{

enum
{
    COLUMN_PIXBUF = 0,
    COLUMN_DESCRIPTION,
    COLUMN_CRITERIA,
    COLUMN_COUNT
};

SearchBasketColumnRecord::SearchBasketColumnRecord() throw() :
    Gtk::TreeModelColumnRecord(),
    columnPixbuf_(),
    columnDescription_(),
    criteria_()
{
    add(columnPixbuf_);
    add(columnDescription_);
    add(criteria_);
}

SearchBasketColumnRecord::~SearchBasketColumnRecord() throw()
{
}

const Gtk::TreeModelColumn<PixbufPtr> &
SearchBasketColumnRecord::get_column_pixbuf() const throw()
{
    return columnPixbuf_;
}

gint
SearchBasketColumnRecord::get_column_pixbuf_num() const throw()
{
    return COLUMN_PIXBUF;
}

const Gtk::TreeModelColumn<Glib::ustring> &
SearchBasketColumnRecord::get_column_description() const throw()
{
    return columnDescription_;
}

gint
SearchBasketColumnRecord::get_column_description_num() const throw()
{
    return COLUMN_DESCRIPTION;
}

const Gtk::TreeModelColumn<PhotoSearchCriteriaPtr> &
SearchBasketColumnRecord::get_column_criteria() const throw()
{
    return criteria_;
}

gint
SearchBasketColumnRecord::get_column_criteria_num() const throw()
{
    return COLUMN_CRITERIA;
}

} // namespace Solang
