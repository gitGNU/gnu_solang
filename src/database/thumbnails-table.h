/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * thumbnails-table.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * thumbnails-table.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * thumbnails-table.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_THUMBNAILS_TABLE_H
#define SOLANG_THUMBNAILS_TABLE_H

#include "db-table.h"

namespace Solang
{

class ThumbnailsTable :
    public DBTable
{

    public:
		ThumbnailsTable( const Glib::ustring &name );
		~ThumbnailsTable();

        virtual Glib::ustring getSelectionQuery() const ;
        virtual Glib::ustring getInsertQuery() const;
        virtual Glib::ustring getUpdateQuery() const;
        virtual Glib::ustring getDeleteQuery() const;
        virtual void receive(Solang::DBTableVisitor&, 
                             Solang::ProgressObserverPtr &);

        
};

} //namesapce Solang

#endif // SOLANG_THUMBNAILS_TABLE_H
