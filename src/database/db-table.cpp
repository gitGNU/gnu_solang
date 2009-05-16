/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * db-table.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * db-table.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * d-b-table.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <iostream>

#include "db-table.h"
#include "db-object.h"

namespace Solang
{

DBTable::DBTable( const Glib::ustring &tableName )
    :tableName_(tableName),
    query_(),
    lastKey_(0),
    model_()
{
}

DBTable::DBTable( const DBTable &rhs )
    :tableName_( rhs.tableName_ ),
    query_( rhs.query_ ),
    lastKey_( rhs.lastKey_ ),
    model_( rhs.model_ )
{
}

DBTable &DBTable::operator = (const DBTable &rhs)
{
    if( this != &rhs )
    {
        tableName_ =  rhs.tableName_ ;
        query_ = rhs.query_ ;
        model_ = rhs.model_ ;
        lastKey_ = rhs.lastKey_ ;
    }    
    return *this;
}

void DBTable::open( 
            const Glib::RefPtr<Gnome::Gda::Dict> &gdaDict) throw(Error)
{
    query_ = Gnome::Gda::Query::create( gdaDict );
    query_->set_sql_text( getSelectionQuery() );
    model_ = Gnome::Gda::DataModelQuery::create( query_ );
    try
    {
#if 0
        model_->set_modification_query( getInsertQuery() );
        model_->set_modification_query( getUpdateQuery() );
        model_->set_modification_query( getDeleteQuery() );
#endif
        model_->compute_modification_queries( tableName_, 
                    Gnome::Gda::DATA_MODEL_QUERY_OPTION_USE_ALL_FIELDS_IF_NO_PK ); 
    }
    catch( const Glib::Error &error )
    {
        //TBD::Error
        std::cerr<<"Error:"<<tableName_<<": "<<error.what()<<std::endl;
    }
    gint32 numRows = model_->get_n_rows();
    lastKey_ = ( numRows ) ? (model_->get_value_at( 0, numRows -1 ).get_int()) : 0;
    return;
    
}

void DBTable::insert( DBObject &object )
{
    try
    {
        object.insert( model_, lastKey_ );
    }
    catch( Error &error )
    {
        error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    lastKey_ ++;
    return;
}

void DBTable::update( DBObject &object )
{
    try
    {
        object.update( model_ );
    }
    catch( Error &error )
    {
        error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    return;
}

void DBTable::remove( DBObject &object )
{
    try
    {
        object.remove( model_ );
    }
    catch( Error &error )
    {
        error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    return;
}

} //namespace Solang
