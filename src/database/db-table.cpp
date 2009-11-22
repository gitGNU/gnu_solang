/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * db-table.cpp
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
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
    lastKey_(0),
    gdaDataModel_()
{
}

DBTable::DBTable( const DBTable &rhs )
    :tableName_( rhs.tableName_ ),
    lastKey_( rhs.lastKey_ ),
    gdaDataModel_( rhs.gdaDataModel_ )
{
}

DBTable &DBTable::operator = (const DBTable &rhs)
{
    if( this != &rhs )
    {
        tableName_ =  rhs.tableName_ ;
        lastKey_ = rhs.lastKey_ ;
        gdaDataModel_ = rhs.gdaDataModel_ ;
    }    
    return *this;
}

void DBTable::open(const ConnectionPtr & connection) throw(Error)
{
    gdaDataModel_
        = connection->statement_execute_select(
              getSelectionQuery(),
              Gnome::Gda::STATEMENT_MODEL_RANDOM_ACCESS);

    const DataSelectPtr data_select = Glib::RefPtr<
        Gnome::Gda::DataSelect>::cast_dynamic(gdaDataModel_);

    try
    {
#if 0
        data_select->set_modification_query( getInsertQuery() );
        data_select->set_modification_query( getUpdateQuery() );
        data_select->set_modification_query( getDeleteQuery() );
#endif
        data_select->compute_modification_statements();
    }
    catch( const Glib::Error &error )
    {
        //TBD::Error
        std::cerr<<"Error:"<<tableName_<<": "<<error.what()<<std::endl;
    }

    const gint32 numRows
        = static_cast<gint32>(gdaDataModel_->get_n_rows());

    lastKey_ = ( numRows ) ? (gdaDataModel_->get_value_at(
                                 0, numRows -1 ).get_int())
                           : 0;
    return;
    
}

void DBTable::insert( DBObject &object )
{
    try
    {
        object.insert( gdaDataModel_, lastKey_ );
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
        object.update( gdaDataModel_ );
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
        object.remove( gdaDataModel_ );
    }
    catch( Error &error )
    {
        error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    return;
}

} //namespace Solang
