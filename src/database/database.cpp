/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * database.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * database.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * database.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <iostream>
#include <sstream>

#include "database.h"
#include "db-table-factory.h"
#include "progress-observer.h"
#include "thumbnail.h"

namespace Solang
{

const Glib::ustring Database::DB_NAME="solang.db";

Database::Database( const Glib::ustring &path)
    : path_(path)
{
}

Database::Database( const Database &rhs )
    : path_(rhs.path_)
{
}

Database & Database::operator =( const Database &rhs )
{
    if( this != &rhs )
    {
        path_ = rhs.path_;
    }

    return *this;
}

Database::~Database()
{
    tables_.clear();
}

void Database::set_path(const Glib::ustring &path)
{
    path_ = path;
}

void Database::open() throw(Error)
{
    Glib::ustring dbPath;
    dbPath = "DB_DIR=";
    dbPath += get_path();
    dbPath += ";DB_NAME=";
    dbPath += DB_NAME;

    gdaClient_ = Gnome::Gda::Client::create();
    gdaConnection_ = gdaClient_->open_connection_from_string(
                                "SQLite", dbPath, "", "",
                                Gnome::Gda::ConnectionOptions(0));    
    gdaDict_ = Gnome::Gda::Dict::create();
    gdaDict_->set_connection( gdaConnection_ );
    gdaDict_->update_dbms_meta_data();
    
    const char *tableNames[] = 
    {
        "photos",
        "photo_tags",
        "tags",
        NULL
    };
    for( const char **tableName = tableNames; *tableName != NULL; tableName++ )
    {
        DBTablePtr table = DBTableFactory::create( *tableName );
        tables_[ *tableName ] = table;
        table->open( gdaDict_ );
    }

}

void Database::save( DBObject &object ) throw(Error)
{
    try
    {
        object.save( *this );
    }
    catch( Error &error )
    {
        error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void Database::save( const DBObjectList &objects,
                        ProgressObserverPtr &observer ) throw(Error)
{
    observer->set_num_events( objects.size() );
    observer->set_event_description( "Saving objects" );
    for( DBObjectList::const_iterator it = objects.begin(); 
                                        it != objects.end(); it++ )
    {
        if( !observer->get_stop() )
        {
            try
            {
                (*it)->save( *this );
                observer->receive_event_notifiation();    
            }
            catch(Error &e)
            {
                e.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
                throw;
            }
        }
    }
}

DBTablePtr Database::getTable(const Glib::ustring &tableName) const
{
    std::cerr<<"Lookin' for:"<<tableName<<std::endl;
    std::map<Glib::ustring, DBTablePtr>::const_iterator table 
                                        = tables_.find( tableName );
    if( table != tables_.end() )
    {
        return (*table).second;
    }

    return DBTablePtr( );
}

void Database::close() throw(Error)
{
    gdaConnection_->close();    
    return;
}

PhotoList Database::search(
                const PhotoSearchCriteriaList &criterion,
                const ProgressObserverPtr & observer) throw(Error)
{

    //Selection from views not supported? WTF?!!
    //Glib::ustring sql = "select * from photo_data ";

    Glib::ustring sql = "select distinct photos.* \
            from photos, photo_tags \
            where   \
            photo_tags.photoid = photos.photoid ";

    if( ! criterion.empty() )
    {
        //sql += " where ";
        bool first = true;
        for( PhotoSearchCriteriaList::const_iterator it 
                                        = criterion.begin();
                                        it != criterion.end(); it++ )
        {
            sql += ( first ) ? " and ( ": " or ";
            sql += (*it)->get_query_criteria();
            first = false;
        }
        sql += " )";    
    }

    PhotoList photos;
    try
    {
        std::cout<<sql<<std::endl;
        Glib::RefPtr<Gnome::Gda::Query> query 
                                = Gnome::Gda::Query::create( gdaDict_ );
        query->set_sql_text( sql );
        Glib::RefPtr<Gnome::Gda::DataModelQuery> model 
                            =  Gnome::Gda::DataModelQuery::create( query );

        gint32 numRows = model->get_n_rows();
        observer->set_num_events( numRows );
        observer->set_event_description( "Generating list of photos" );

        for( gint32 row = 0; row < numRows; row++ )
        {
            if( !observer->get_stop() )
            {
                PhotoPtr photo( new Photo() );
                photo->create( model, row );
                photos.push_back( photo );
                observer->receive_event_notifiation();
            }
        }
    }
    catch(Glib::Error &e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
    }
    return photos;


}

} //namespace Solang
