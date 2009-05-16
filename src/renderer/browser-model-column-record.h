/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * browser-model-column-record.h
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * browser-model-column-record.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * browser-model-column-record.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_BROWSER_MODEL_COLUMN_RECORD_H
#define SOLANG_BROWSER_MODEL_COLUMN_RECORD_H

#include <gdkmm.h>
#include <gtkmm.h>

#include "types.h"

namespace Solang
{

class BrowserModelColumnRecord :
    public Gtk::TreeModelColumnRecord
{
    public:
        BrowserModelColumnRecord() throw();

        virtual
        ~BrowserModelColumnRecord() throw();

        const Gtk::TreeModelColumn<PhotoPtr> &
        get_column_photo() const throw();

        gint
        get_column_photo_num() const throw();

        const Gtk::TreeModelColumn<PixbufPtr> &
        get_column_pixbuf() const throw();

        gint
        get_column_pixbuf_num() const throw();

        const Gtk::TreeModelColumn<Glib::ustring> &
        get_column_tag_name() const throw();

        gint
        get_column_tag_name_num() const throw();

    protected:
        Gtk::TreeModelColumn<PhotoPtr> columnPhoto_;

        Gtk::TreeModelColumn<PixbufPtr> columnPixbuf_;

        Gtk::TreeModelColumn<Glib::ustring> columnTagName_;

    private:
};

} // namespace Solang

#endif // SOLANG_BROWSER_MODEL_COLUMN_RECORD_H
