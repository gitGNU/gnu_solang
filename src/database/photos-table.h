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

#ifndef SOLANG_PHOTOS_TABLE_H
#define SOLANG_PHOTOS_TABLE_H

#include <glibmm.h>

#include "db-table.h"

namespace Solang
{

class PhotosTable :
    public DBTable
{
    public:
        PhotosTable(const Glib::ustring & name);

        ~PhotosTable();

        virtual Glib::ustring
        getSelectionQuery() const;

        virtual Glib::ustring
        getInsertQuery() const;

        virtual Glib::ustring
        getUpdateQuery() const;

        virtual Glib::ustring
        getDeleteQuery() const;

        virtual void
        receive(Solang::DBTableVisitor&,
                Solang::ProgressObserverPtr &);
};

} // namespace Solang

#endif // SOLANG_PHOTOS_TABLE_H
