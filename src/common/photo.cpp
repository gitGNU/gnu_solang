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

#include <iostream>
#include <libgdamm.h>

#include "database.h"
#include "photo.h"
#include "i-storage.h"

namespace Solang
{

const gint32 Photo::PHOTOID_COL                        = 0;
const gint32 Photo::URI_COL                            = 1;

Photo::Photo() throw() :
    DBObject(),
    photoId_(-1),
    uri_(),
    diskFilePath_(),
    thumbnail_(),
    exifData_()
{
}

Photo::~Photo() throw()
{
}

void Photo::set_photoId_( gint64 photoId ) throw()
{
    photoId_ = photoId;
}

void
Photo::set_uri(const Glib::ustring & uri)
{
    uri_ = uri;
}

void
Photo::set_thumbnail( const Thumbnail &thumb ) throw()
{
    thumbnail_ = thumb;
}

void Photo::set_exif_data( const ExifData &exifData ) throw()
{
    exifData_ = exifData;
}

void Photo::set_disk_file_path(const IStoragePtr & storage)
{
    diskFilePath_ = storage->retrieve(*this);
}

void Photo::set_disk_file_path(const Glib::ustring & disk_file_path)
{
    diskFilePath_ = disk_file_path;
}

void Photo::insert( DataModelPtr &model, gint32 lastIndex) throw(Error)
{
    std::vector<Gnome::Gda::Value> values;

    values.push_back( Gnome::Gda::Value( lastIndex + 1 ) ); //photoid
    values.push_back( Gnome::Gda::Value( get_uri() ) ); 

    modDate_.insert( values );
    thumbnail_.insert( values );
    exifData_.insert( values );

    gint32 row=0;
    try
    {
        row = model->append_values( values );
    }
    catch(Glib::Error &e)
    {
        std::cerr<<"Error: "<<e.what()<<std::endl;
        //TBD::Error
    }

    if( -1 == row )
    {
        //TBD::Error
    }
    
    set_row_( row );

    set_photoId_( lastIndex + 1 );

    return;
}

void Photo::update( DataModelPtr &model, gint32 row) throw(Error)
try
{
    if( get_uri().length() > 0 
            && get_uri() != model->get_value_at( 
                                Photo::URI_COL, row ).get_string() )
    {
        model->set_value_at( 
                Photo::URI_COL, row, Gnome::Gda::Value( get_uri() ) );
    }

    thumbnail_.update( model, row );
    exifData_.update( model, row );


    return;

}
catch(Glib::Error &e)
{
    std::cerr<<"Error:"<<e.what()<<std::endl;
    throw;
}

void Photo::create(
                DataModelPtr& dataModel, int32_t row) throw(Error)
{
    set_row_( row );

    set_photoId_( dataModel->get_value_at(
                                    PHOTOID_COL, row ).get_int());
    set_uri( dataModel->get_value_at( URI_COL, row ).get_string());
    ModificationDate date;
    date.create( dataModel, row );
    set_modification_date( date );

    Thumbnail thumb;
    thumb.create( dataModel, row );
    set_thumbnail( thumb );

    ExifData data;
    data.create( dataModel, row );
    set_exif_data( data );

    return;    
    
}

Glib::ustring 
Photo::get_db_object_type_name() const throw()
{
    return "photos";
}

void
Photo::set_modification_date( const ModificationDate &modDate )
{
    modDate_ = modDate;
}

} // namespace Solang
