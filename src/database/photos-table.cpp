/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * photos-table.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * photos-table.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * photos-table.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "db-table-visitor.h"
#include "photo.h"
#include "photos-table.h"
#include "progress-observer.h"

namespace Solang
{

PhotosTable::PhotosTable( const Glib::ustring &name )
    :DBTable( name )
{
}

PhotosTable::~PhotosTable()
{
}

Glib::ustring PhotosTable::getSelectionQuery() const
{
    return "select * from photos order by "
               "mod_year, mod_month, mod_day, picture_taken_time";
}

Glib::ustring PhotosTable::getInsertQuery() const
{
    return "insert into photos( photoid, uri)  \
                values(    \
                    ## /*name: +0, type:gint*/, \
                    ## /*name: +1, type:gchararray*/)";
                
}

Glib::ustring PhotosTable::getUpdateQuery() const
{
    return "update photos set \
                photoid=## /* name: +0, type:gint, nullok: true*/, \
                uri=## /* name: +1, type:gchararray, nullok: true*/ \
                where photoid=## /* name: -0, type:gint, nullok: true*/";
                
}

Glib::ustring PhotosTable::getDeleteQuery() const
{
    return "delete from photos  \
                where photoid=##/* name: -0, type: gint*/";
}

void
PhotosTable::receive(Solang::DBTableVisitor & visitor, 
                     Solang::ProgressObserverPtr & observer)
{
    gint32 numRows = model_->get_n_rows();
    observer->set_num_events(numRows);
    observer->set_event_description("Generating list of photos");

    PhotoList photos;

    for( gint32 row = 0; row < numRows; row++ )
    {
        if( !observer->get_stop() )
        {
            PhotoPtr photo( new Photo() );
            photo->create( model_, row );
            photos.push_back( photo );
            observer->receive_event_notifiation();
        }
    }
    
    visitor.receive_data( photos );
    
    return;

}

} //namespace Solang
