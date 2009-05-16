/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * tag-view-model-column-record.h
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * tag-view-model-column-record.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * tag-view-model-column-record.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_TAG_VIEW_MODEL_COLUMN_RECORD_H
#define SOLANG_TAG_VIEW_MODEL_COLUMN_RECORD_H

#include <gdkmm.h>
#include <glibmm.h>
#include <gtkmm.h>

#include "types.h"

namespace Solang
{

class TagViewModelColumnRecord :
    public Gtk::TreeModelColumnRecord 
{
    public:
        TagViewModelColumnRecord() throw();

        virtual
        ~TagViewModelColumnRecord() throw();

        const Gtk::TreeModelColumn<TagPtr> &
        get_column_tag() const throw();

        gint
        get_column_tag_num() const throw();

        const Gtk::TreeModelColumn<bool> &
        get_column_selected() const throw();

        gint
        get_column_selected_num() const throw();

        const Gtk::TreeModelColumn<PixbufPtr> &
        get_column_pixbuf() const throw();

        gint
        get_column_pixbuf_num() const throw();

        const Gtk::TreeModelColumn<Glib::ustring> &
        get_column_name() const throw();

        gint
        get_column_name_num() const throw();

        const Gtk::TreeModelColumn<Glib::ustring> &
        get_column_description() const throw();

        gint
        get_column_description_num() const throw();

    protected:
        Gtk::TreeModelColumn<TagPtr> columnTag_;

        Gtk::TreeModelColumn<bool> columnSelected_;

        Gtk::TreeModelColumn<PixbufPtr> columnPixbuf_;

        Gtk::TreeModelColumn<Glib::ustring> columnName_;

        Gtk::TreeModelColumn<Glib::ustring> columnDescription_;

    private:
};

} // namespace Solang

#endif // SOLANG_TAG_VIEW_MODEL_COLUMN_RECORD_H
