/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * db-object.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * db-object.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * d-b-object.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "database.h"
#include "db-object.h"

namespace Solang
{

DBObject::DBObject() throw()
    :row_( -1 ),
    table_()
{
}

DBObject::~DBObject() throw()
{
}

void DBObject::save(Database &db) throw(Error)
{
    if( !get_table_() ) // If this is not in the DB
    {
        DBTablePtr table = db.getTable( get_db_object_type_name() );
        try
        {
            table->insert( *this );
        }
        catch( Error &error )
        {
            error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
        set_table_( table_ );
        return;
    }
    table_->update( *this );
    return;
}

void DBObject::create(Database &db, gint32 row) throw(Error)
{
    DBTablePtr table;
    if( ! (table = get_table_()) )
    {
        table = db.getTable(get_db_object_type_name());
        set_table_( table );
    }
    
    Glib::RefPtr<Gnome::Gda::DataModelQuery> model 
                                        = table->get_model();
    create( model, row );

    return;
}


void DBObject::update( DataModelPtr &model ) throw(Error)
{
    try
    {
        update( model, get_row_() );
    }
    catch( Error &error )
    {
        error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    return;
}

void DBObject::remove( DataModelPtr &model ) throw(Error)
{
    try
    {
        remove( model, get_row_() );
    }
    catch( Error &error )
    {
        error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void DBObject::remove( DataModelPtr &model, gint32 row ) throw(Error)
{
    try
    {
        model->remove_row( row_ );
    }
    catch( Glib::Error &error )
    {
        //TBD::Error
    }
    set_row_( -1 );
    return;
}

void DBObject::set_row_( gint32 row ) throw()
{
    row_ = row;
}

void DBObject::set_table_( const DBTablePtr &table ) throw()
{
    table_ = table;
}

} //namespace Solang
