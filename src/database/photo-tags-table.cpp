/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * photo-tags-table.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * photo-tags-table.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * photo-tags-table.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "db-table-visitor.h"
#include "photo-tags-table.h"
#include "progress-observer.h"


namespace Solang
{

PhotoTagsTable::PhotoTagsTable( const Glib::ustring &name )
    :DBTable( name )
{
}

PhotoTagsTable::~PhotoTagsTable()
{
}

Glib::ustring PhotoTagsTable::getSelectionQuery() const
{
    return "select * from photo_tags";
}

Glib::ustring PhotoTagsTable::getInsertQuery() const
{
    return "insert into photo_tags(photoid, tagid)  \
                values(    \
                    ## /* name:+0, type:gint*/, \
                    ## /* name:+1, type:gint*/)";
                
}

Glib::ustring PhotoTagsTable::getUpdateQuery() const
{
    return "update photo_tags set \
                tagid=## /* name:+0, type:gint*/, \
                photoid=## /* name:+1, type:gint*/ \
                where photoid=## /* name:-0, type:gint*/ \
                and tagid=## /* name:-1, type:gint*/";
                
}

Glib::ustring PhotoTagsTable::getDeleteQuery() const
{
    return "delete from photo_tags  \
                where photoid=## /* name:-0, type:gint*/ \
                and tagid=## /* name:-1, type:gint*/";
}

void PhotoTagsTable::receive(Solang::DBTableVisitor& visitor, 
                        Solang::ProgressObserverPtr &observer)
{
    gint32 numRows = gdaDataModel_->get_n_rows();
    observer->set_num_events( numRows );
    observer->set_event_description( 
                "Generating list of photo-tag associations" );

    TagList tags;

    for( gint32 row = 0; row < numRows; row++ )
    {
        if( !observer->get_stop() )
        {
            TagPtr tag( new Tag() );
            tag->create( gdaDataModel_, row );
            tags.push_back( tag );
            observer->receive_event_notifiation();
        }
    }
    
    visitor.receive_data( tags );
    
    return;

}

} //namespace Solang
