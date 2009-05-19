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

#ifndef SOLANG_BASIC_VIEW_MODEL_COLUMN_RECORD_H
#define SOLANG_BASIC_VIEW_MODEL_COLUMN_RECORD_H

#include <gdkmm.h>
#include <glibmm.h>
#include <gtkmm.h>

#include "types.h"

namespace Solang
{

class BasicExifViewModelColumnRecord :
    public Gtk::TreeModelColumnRecord
{
    public:
        BasicExifViewModelColumnRecord() throw();

        virtual
        ~BasicExifViewModelColumnRecord() throw();

        const Gtk::TreeModelColumn<ExifDataKeyPtr> &
        get_column_key() const throw();

        gint
        get_column_key_num() const throw();

        const Gtk::TreeModelColumn<bool> &
        get_column_selected() const throw();

        gint
        get_column_selected_num() const throw();

        const Gtk::TreeModelColumn<Glib::ustring> &
        get_column_description() const throw();

        gint
        get_column_description_num() const throw();

        const Gtk::TreeModelColumn<Glib::ustring> &
        get_column_value() const throw();

        gint
        get_column_value_num() const throw();

    protected:
        Gtk::TreeModelColumn<ExifDataKeyPtr> key_;
        Gtk::TreeModelColumn<bool> selected_;
        Gtk::TreeModelColumn<Glib::ustring> description_;
        Gtk::TreeModelColumn<Glib::ustring> value_;


    private:
};

} // namespace Solang

#endif // SOLANG_BASIC_VIEW_MODEL_COLUMN_RECORD_H
