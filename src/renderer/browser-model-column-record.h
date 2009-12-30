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

#ifndef SOLANG_BROWSER_MODEL_COLUMN_RECORD_H
#define SOLANG_BROWSER_MODEL_COLUMN_RECORD_H

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

        const Gtk::TreeModelColumn<guint> &
        get_column_serial() const throw();

        gint
        get_column_serial_num() const throw();

        const Gtk::TreeModelColumn<PhotoPtr> &
        get_column_photo() const throw();

        gint
        get_column_photo_num() const throw();

    protected:
        Gtk::TreeModelColumn<guint> columnSerial_;

        Gtk::TreeModelColumn<PhotoPtr> columnPhoto_;

    private:
};

} // namespace Solang

#endif // SOLANG_BROWSER_MODEL_COLUMN_RECORD_H
