/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Debarshi Ray 2009, 2010 <rishi@gnu.org>
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

#ifndef SOLANG_SEARCH_BASKET_COLUMN_RECORD_H
#define SOLANG_SEARCH_BASKET_COLUMN_RECORD_H

#include <gdkmm.h>
#include <glibmm.h>
#include <gtkmm.h>

#include "types.h"

namespace Solang
{

class SearchBasketColumnRecord :
    public Gtk::TreeModelColumnRecord
{
    public:
        SearchBasketColumnRecord() throw();

        virtual
        ~SearchBasketColumnRecord() throw();

        const Gtk::TreeModelColumn<PixbufPtr> &
        get_column_pixbuf() const throw();

        gint
        get_column_pixbuf_num() const throw();

        const Gtk::TreeModelColumn<Glib::ustring> &
        get_column_description() const throw();

        gint
        get_column_description_num() const throw();

        const Gtk::TreeModelColumn<IPhotoSearchCriteriaPtr> &
        get_column_criteria() const throw();

        gint
        get_column_criteria_num() const throw();

    protected:
        Gtk::TreeModelColumn<PixbufPtr> columnPixbuf_;

        Gtk::TreeModelColumn<Glib::ustring> columnDescription_;

        Gtk::TreeModelColumn<IPhotoSearchCriteriaPtr> criteria_;

    private:
};

} // namespace Solang

#endif // SOLANG_SEARCH_BASKET_COLUMN_RECORD_H
