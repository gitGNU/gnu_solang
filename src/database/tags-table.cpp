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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "db-table-visitor.h"
#include "tag.h"
#include "tags-table.h"
#include "progress-observer.h"

namespace Solang
{

TagsTable::TagsTable( const Glib::ustring &name )
    :DBTable( name )
{
}

TagsTable::~TagsTable()
{
}

Glib::ustring
TagsTable::getSelectionQuery() const
{
    return "select * from tags";
}

Glib::ustring
TagsTable::getInsertQuery() const
{
    return "insert into tags( \
                tagid, tag, description, iconpath)  \
                values(    \
                    ## /* name: +0, type:gint*/, \
                    ## /* name: +1, type:gchararray*/, \
                    ## /* name: +2, type:gchararray*/, \
                    ## /* name: +3, type:gchararray*/)";
                
}

Glib::ustring
TagsTable::getUpdateQuery() const
{
    return "update tags set \
                tagid=## /* name:+0, type:gint*/, \
                tag=## /* name:+1, type:gchararray*/, \
                description=## /* name:+2, type:gchararray*/, \
                iconpath=## /* name:+3, type:gchararray*/ \
                where tagid=##/* name:-0, type:gint*/";
                
}

Glib::ustring
TagsTable::getDeleteQuery() const
{
    return "delete from tags  \
                where tagid=##/* name:-0, type:gint*/";
}

void
TagsTable::receive(Solang::DBTableVisitor& visitor,
                   Solang::ProgressObserverPtr &observer)
{
    gint32 numRows = model_->get_n_rows();
    observer->set_num_events( numRows );
    observer->set_event_description( "Generating list of tags" );

    TagList tags;

    for( gint32 row = 0; row < numRows; row++ )
    {
        if( !observer->get_stop() )
        {
            TagPtr tag( new Tag() );
            tag->create( model_, row );
            tags.push_back( tag );
            observer->receive_event_notifiation();
        }
    }
    
    visitor.receive_data( tags );
    
    return;

}

} // namespace Solang
