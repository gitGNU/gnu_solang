/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#include <iostream>
#include <sstream>
#include <vector>

#include "database.h"
#include "db-table-factory.h"
#include "progress-observer.h"
#include "thumbnail.h"
#include "date-photo-info.h"

namespace Solang
{

class Comparator
{
    public:
        bool operator()  ( const PhotoSearchCriteriaPtr &lhs,
                            const PhotoSearchCriteriaPtr &rhs )
        {
            return lhs->get_id() < rhs->get_id();
        }
};

const Glib::ustring Database::DB_NAME="solang";

Database::Database( const Glib::ustring &path)
    : path_(path)
{
}

Database::Database( const Database &rhs )
    : path_(rhs.path_)
{
}

Database &
Database::operator =( const Database &rhs )
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

void
Database::set_path(const Glib::ustring &path)
{
    path_ = path;
}

void
Database::open() throw(Error)
{
    Glib::ustring dbPath;
    dbPath = "DB_DIR=";
    dbPath += get_path();
    dbPath += ";DB_NAME=";
    dbPath += DB_NAME;

    bool dbExists = db_file_exists();
    gdaConnection_ = Gnome::Gda::Connection::open_from_string(
                         "SQLite", dbPath, "",
                         Gnome::Gda::CONNECTION_OPTIONS_NONE);
    if( !dbExists )
    {
        create_db( );
    }

    gdaConnection_->update_meta_store();
    
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
        table->open(gdaConnection_);
    }

}

void
Database::save( DBObject &object ) throw(Error)
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

void
Database::save( const DBObjectList &objects,
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

DBTablePtr
Database::getTable(const Glib::ustring &tableName) const
{
    std::map<Glib::ustring, DBTablePtr>::const_iterator table 
                                        = tables_.find( tableName );
    if( table != tables_.end() )
    {
        return (*table).second;
    }

    return DBTablePtr( );
}

void
Database::run_sql( const Glib::ustring &sql ) throw(Error)
{
    try
    {
        gdaConnection_->statement_execute(
            sql, Gnome::Gda::STATEMENT_MODEL_RANDOM_ACCESS);
    }
    catch(Glib::Error &error)
    {
        std::cerr<<"Error: "<<error.what()<<std::endl;
    }
}

void
Database::close() throw(Error)
{
    gdaConnection_->close();    
    return;
}

PhotoList
Database::search(const PhotoSearchCriteriaList & criterion,
                 const ProgressObserverPtr & observer) throw(Error)
{

    //Selection from views not supported? WTF?!!
    //Glib::ustring sql = "select * from photo_data ";
    std::vector<PhotoSearchCriteriaPtr> tmpList( criterion.begin(), criterion.end() );
    std::sort( tmpList.begin(), tmpList.end(), Comparator() );

    Glib::ustring sql = "select distinct photos.* \
            from photos, photo_tags \
            where   \
            photo_tags.photoid = photos.photoid ";

    if( ! criterion.empty() )
    {
        bool first = true;
        gint32 lastId = -1;
        for( std::vector<PhotoSearchCriteriaPtr>::const_iterator it
                                        = tmpList.begin();
                                        it != tmpList.end(); it++ )
        {
            if( !first )
            {
                if( lastId == (*it)->get_id() )
                {
                    switch( (*it)->get_clubbing_type() )
                    {
                        case PhotoSearchCriteria::CLUB_AND:
                        {
                            sql += " and ";
                            break;
                        }
                        case PhotoSearchCriteria::CLUB_OR:
                        {
                            sql += " or ";
                            break;
                        }
                    }
                }
                else
                {
                    sql += " ) and ( ";
                    //first = true;
                }
            }
            else
            {
                sql += " and ( ";
                first = false;
            }
            sql += (*it)->get_query_criteria();
            lastId = (*it)->get_id();
        }
        sql += " )";    
    }
    sql += " order by mod_year,mod_month,mod_day,picture_taken_time";

    PhotoList photos;
    try
    {
        const DataModelPtr model
            = gdaConnection_->statement_execute_select(
                  sql, Gnome::Gda::STATEMENT_MODEL_RANDOM_ACCESS);

        const gint32 numRows
                         = static_cast<gint32>(model->get_n_rows());

        observer->set_num_events( numRows );
        observer->set_event_description( "Generating list of photos" );
        DBTablePtr table = getTable( "photos" );

        for( gint32 row = 0; row < numRows; row++ )
        {
            if( !observer->get_stop() )
            {
                PhotoPtr photo( new Photo() );
                photo->create( model, row );
                photo->set_table( table );
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

//Group by year
DatePhotoInfoList
Database::get_dates_with_picture_count(
              const ProgressObserverPtr & observer)
{
    Glib::ustring sql
        = "select 0, 0, mod_year, count(*) from photos ";
    sql += "group by mod_year";
    return get_dates_with_picture_count( sql, observer );
}

//Group by year, month
DatePhotoInfoList
Database::get_dates_with_picture_count(
              gint year,
              const ProgressObserverPtr & observer)
{
    std::ostringstream sout;
    sout<<"select 0, mod_month, mod_year, count(*) from photos ";
    sout<<"where mod_year="<<year<<" ";
    sout<<"group by mod_year,mod_month ";
//    sout<<"order by mod_year desc, mod_month desc";
    return get_dates_with_picture_count( sout.str(), observer );
}

//Group by year, month, day
DatePhotoInfoList
Database::get_dates_with_picture_count(
              gint year, gint month,
              const ProgressObserverPtr & observer)
{
    std::ostringstream sout;
    sout<<"select mod_day, mod_month, mod_year, count(*) from photos ";
    sout<<"where mod_year="<<year<<" ";
    sout<<"and mod_month="<<month<<" ";
    sout<<"group by mod_year,mod_month,mod_day ";
//    sout<<"order by mod_year desc, mod_month desc, mod_day desc";
    return get_dates_with_picture_count( sout.str(), observer );
}

DatePhotoInfoList
Database::get_dates_with_picture_count(
              const Glib::ustring & sql,
              const ProgressObserverPtr & observer)
{
    DatePhotoInfoList infos;

    try
    {
        const DataModelPtr model
            = gdaConnection_->statement_execute_select(
                  sql, Gnome::Gda::STATEMENT_MODEL_RANDOM_ACCESS);

        const gint32 numRows
                         = static_cast<gint32>(model->get_n_rows());

        observer->set_num_events( numRows );
        observer->set_event_description(
                            "Generating summary of photos" );

        for( gint32 row = 0; row < numRows; row++ )
        {
            if( !observer->get_stop() )
            {
                infos.push_back(
                    DatePhotoInfo(
                        ModificationDate(
                            model->get_value_at( 0, row ).get_int(),
                            model->get_value_at( 1, row ).get_int(),
                            model->get_value_at( 2, row ).get_int()),
                        model->get_value_at( 3, row ).get_int()));
                observer->receive_event_notifiation();
            }
        }
    }
    catch (Glib::Error &e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
    }

    return infos;
}

bool
Database::db_file_exists() const throw()
{
    Glib::ustring dbPath;
    dbPath = get_path();
    dbPath += "/";
    dbPath += DB_NAME;
    dbPath +=".db";

    FilePtr dbFile = Gio::File::create_for_path( dbPath );
    return dbFile->query_exists() ;
}

void
Database::create_db() throw(Error)
{
    try
    {
        //Photos
        {
            Glib::ustring sql = "CREATE TABLE photos(\
                        photoid integer primary key,\
                        uri varchar(255),\
                        content_type varchar(50),\
                        mod_day integer,\
                        mod_month integer,\
                        mod_year integer,\
                        thumbnail varchar(255),\
                        thumbnailx int,\
                        thumbnaily int,\
                        aperture varchar(100),\
                        shutter_speed varchar(100),\
                        exposure_program varchar(100),\
                        iso_speed integer,\
                        metering_mode varchar(100),\
                        focal_length varchar(100),\
                        white_balance varchar(100),\
                        focal_length_in_film varchar(100),\
                        picture_taken_time varchar(100))";

            gdaConnection_->statement_execute_non_select(sql);
        }
        //Tags
        {
            Glib::ustring sql = "CREATE TABLE tags(\
                                tagid integer primary key,\
                                tag varchar(255),\
                                description varchar(255),\
                                iconpath varchar(255))";

            gdaConnection_->statement_execute_non_select(sql);

            //Create the all photos tag
            {
                Glib::ustring sql = "INSERT INTO tags\
                                values(0, 'All Photos', \
                                'All photos imported in solang','')";

                gdaConnection_->statement_execute_non_select(sql);
            }
        }
        //Photo Tags
        {
            Glib::ustring sql = "CREATE TABLE photo_tags(\
                                    photoid integer,\
                                    tagid integer,\
                                    primary key (photoid, tagid))";

            gdaConnection_->statement_execute_non_select(sql);
        }
    }
    catch (Glib::Error &e)
    {
    }
}

} // namespace Solang
