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

#ifndef SOLANG_DATE_VIEW_MODEL_COLUMN_RECORD_H
#define SOLANG_DATE_VIEW_MODEL_COLUMN_RECORD_H

#include <gdkmm.h>
#include <glibmm.h>
#include <gtkmm.h>

#include "types.h"
#include "modification-date.h"

namespace Solang
{

class DateViewModelColumnRecord :
    public Gtk::TreeModelColumnRecord
{
    public:
        DateViewModelColumnRecord() throw();

        virtual
        ~DateViewModelColumnRecord() throw();

        const Gtk::TreeModelColumn<ModificationDate> &
        get_column_date() const throw();

        gint
        get_column_date_num() const throw();

        const Gtk::TreeModelColumn<Glib::ustring> &
        get_column_entry() const throw();

        gint
        get_column_entry_num() const throw();

        const Gtk::TreeModelColumn<gint> &
        get_column_photo_count() const throw();

        gint
        get_column_photo_count_num() const throw();

        const Gtk::TreeModelColumn<gdouble> &
        get_column_percentage() const throw();

        gint
        get_column_percentage_num() const throw();

    protected:
        Gtk::TreeModelColumn<ModificationDate> date_;
        Gtk::TreeModelColumn<Glib::ustring> entry_; //Year/Month/Day
        Gtk::TreeModelColumn<gint> photoCount_;
        Gtk::TreeModelColumn<gdouble> percentage_;


    private:
};

} // namespace Solang

#endif // SOLANG_DATE_VIEW_MODEL_COLUMN_RECORD_H
