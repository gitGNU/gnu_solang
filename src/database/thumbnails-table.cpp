/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * thumbnails-table.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * thumbnails-table.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * thumbnails-table.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "db-table-visitor.h"
#include "photo.h"
#include "thumbnails-table.h"
#include "progress-observer.h"

namespace Solang
{

ThumbnailsTable::ThumbnailsTable( const Glib::ustring &name )
    :DBTable( name )
{
}

ThumbnailsTable::~ThumbnailsTable()
{
}

Glib::ustring ThumbnailsTable::getSelectionQuery() const
{
    return "select * from thumbnails";
}

Glib::ustring ThumbnailsTable::getInsertQuery() const
{
    return "insert into thumbnails\
                values(    \
                    ## /*name: +0, type:gint*/, \
                    ## /*name: +1, type:gchararray*/, \
                    ## /*name: +2, type:gint*/, \
                    ## /*name: +3, type:gint*/)";
                
}

Glib::ustring ThumbnailsTable::getUpdateQuery() const
{
    return "update thumbnails set \
                photoid=## /* name: +0, type:gint*/, \
                thumbnail=## /* name: +1, type:gchararray*/, \
                thumbnailx=## /* name: +2, type:gint*/, \
                thumbnaily=## /* name: +3, type:gint*/ \
                where photoid=## /* name: -0, type:gint*/";
                
}

Glib::ustring ThumbnailsTable::getDeleteQuery() const
{
    return "delete from thumbnails  \
                where photoid=##/* name: -0, type: gint*/";
}

void ThumbnailsTable::receive(Solang::DBTableVisitor& visitor, 
                              Solang::ProgressObserverPtr & observer)
{

}

} //namespace Solang
