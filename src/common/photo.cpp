/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * photo.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * photo.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * photo.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <iostream>
#include <libgdamm.h>

#include "database.h"
#include "photo.h"
#include "i-storage.h"

namespace Solang
{

const gint32 Photo::PHOTOID_COL                        = 0;
const gint32 Photo::URI_COL                            = 1;
#if 0
const gint32 Photo::THUMBNAIL_PATH_COL                 = 5;
const gint32 Photo::THUMBNAIL_RES_X_COL                = 6;
const gint32 Photo::THUMBNAIL_RES_Y_COL                = 7;
#endif

Photo::Photo() throw() :
    DBObject(),
    photoId_(-1),
    uri_(),
    diskFilePath_(),
    thumbnail_(),
    exifData_()
{
}

#if 0
Photo::Photo(gint64 photoId, const Glib::ustring & uri) throw() :
    DBObject(),
    photoId_(photoId),
    uri_(uri),
    thumbnail_(),
    diskFilePath_()
{
}
#endif

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

#if 0
Glib::ustring Photo::getCreateSQL() throw(Error)
{
    Glib::ustring sql("insert into photos    (photoid,uri) values(\'");
    sql += photoId_ + "', '" ;
    sql += uri_ + "')";
    return sql;
}
#endif

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

#if 0
    if( thumbnail_ )
    {
        thumbnail_->insert( model, get_photoId_() );
    }
#endif

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

#if 0       
    if( get_thumbnail_path().length() > 0 
            && get_thumbnail_path() != model->get_value_at( 
                                PATH_COL, row ).get_string() )
    {
        Gnome::Gda::Value v;
        v.set( Glib::ustring( get_thumbnail_path() ) );
        model->set_value_at( 
                PATH_COL, row, Gnome::Gda::Value( v ) );
    }

    if (0 < get_thumbnail_resolution().get_x()
        && get_thumbnail_resolution().get_x() != model->get_value_at( 
                                RES_X_COL, row ).get_int() )
    {
        model->set_value_at( 
                RES_X_COL, row, Gnome::Gda::Value( 
                            get_thumbnail_resolution().get_x()));
    }

    if (0 < get_thumbnail_resolution().get_y()
        && get_thumbnail_resolution().get_y() != model->get_value_at( 
                                RES_Y_COL, row ).get_int() )
    {
        model->set_value_at( 
                RES_Y_COL, row, Gnome::Gda::Value( 
                                get_thumbnail_resolution().get_y()));
    }
#endif
    thumbnail_.update( model, row );
    exifData_.update( model, row );


    return;

}
catch(Glib::Error &e)
{
    std::cerr<<"Error:"<<e.what()<<std::endl;
    throw;
}


#if 0
void Photo::create(Database &db, gint32 row) throw(Error)
{
    DBObject::create( db, row );

    try
    {    
        thumbnail->DBObject::create( db, row );
    }
    catch( Error &error )
    {
        error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }

    ExifData exif();

    set_exif_data( exif ); 

    try
    {    
        exif->DBObject::create( , row );
    }
    catch( Error &error )
    {
        error.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }


}
#endif

void Photo::create(
                DataModelPtr& dataModel, int32_t row) throw(Error)
{
    set_row_( row );

    set_photoId_( dataModel->get_value_at( 
                                    PHOTOID_COL, row ).get_int());
    set_uri( dataModel->get_value_at( URI_COL, row ).get_string());
#if 0
    set_thumbnail_path( dataModel->get_value_at( 
                                    PATH_COL, row ).get_string());
    Resolution tmp(dataModel->get_value_at(RES_X_COL, row).get_int(),
                   dataModel->get_value_at(RES_Y_COL, row).get_int());
    set_thumbnail_resolution(tmp);
#endif
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

#if 0
void
Photo::set_thumbnail_path(const Glib::ustring & path)
{
    path_ = path;
}

void
Photo::set_thumbnail_resolution(const Resolution & resolution)
{
    resolution_ = resolution;
}

void
Photo::generate_thumbnail(const Resolution & new_resolution,
                    const DatabasePtr & db) throw(Error)
{
    if( new_resolution == get_thumbnail_resolution() 
            || new_resolution < get_thumbnail_resolution() )
    {
        return;
    }

    //We assume the photo has physical path
    if( photo_->get_disk_file_path().empty() )
    {
        //TBD::Error
        return;
    }

    // We resize the thumbnail only.
    // Makes it faster.
    generate_using_gdkpixbuf(
                    get_thumbnail_path(), new_resolution);

    db->save( *this );

    return;
        
}

void
Photo::generate_thumbnail() throw(Error)
{
    try
    {
        make_thumb_path();    
    }
    catch(Error & e)
    {
        //TBD::Error
        throw;
    }

    generate_using_gdkpixbuf(photo_->get_disk_file_path(),
                               Resolution(128, 128));    

    return;
}

void
Photo::generate_thumbnail(const Exiv2::ExifData & exifData) throw(Error)
{
    // We assume the photo has physical path.
    if (photo_->get_disk_file_path().empty())
    {
        //TBD::Error
        return;
    }

    Glib::ustring path;

    bool thumbnail_generated = false;

    try
    {
        make_thumb_path();    
    }
    catch(Error & e)
    {
        std::cerr<<"Error::"<<e.what()<<std::endl;
        //TBD::Error
        throw;
    }

#if 0
    if (!exifData.empty())
    {
        // Extract from exif if present.
        if (-1 == exifData.writeThumbnail(
                                        get_thumbnail_path().c_str()))
        {
            thumbnail_generated = false;
        }

        thumbnail_generated = true;

        //Update resoulution info
        Thumbnail::Resolution res;

        {

            Exiv2::ExifKey key("Exif.Thumbnail.XResolution");
            Exiv2::ExifData::const_iterator resX 
                                        = exifData.findKey( key );
            if (resX != exifData.end())
            {
                res.set_x((*resX).value().toRational().first
                          / (*resX).value().toRational().second);
            }
    
        }

        {
            Exiv2::ExifKey key("Exif.Thumbnail.YResolution");
            Exiv2::ExifData::const_iterator resY 
                                        = exifData.findKey( key );
            if( resY != exifData.end() )
            {
                res.set_y((*resY).value().toRational().first
                          / (*resY).value().toRational().second);
            }
    
        }

        set_thumbnail_resolution( res );
    }
#endif

    if (false == thumbnail_generated)
    {
        set_thumbnail_path( get_thumbnail_path() + ".jpg" );
        generate_using_gdkpixbuf(photo_->get_disk_file_path(),
                                   Resolution(256, 256));
    }    
 
    return;
}

void
Photo::make_thumb_path() throw(Error)
{
     Glib::RefPtr<Gio::File> tFile
                = Gio::File::create_for_path( get_thumbnail_path() );
    std::cout<<tFile->get_thumbnail_path().substr(
                0,get_thumbnail_path().find( tFile->get_basename()))
              <<std::endl;
     Glib::RefPtr<Gio::File>tDestPath 
                = Gio::File::create_for_path( 
                    tFile->get_thumbnail_path().substr(
                        0, get_thumbnail_path().find( 
                        tFile->get_basename())) );
#ifdef GLIBMM_EXCEPTIONS_ENABLED
    try
    {
        tDestPath->make_directory_with_parents();
    }
    catch( Glib::Error &)
    {
        //TBD::Error
    }
#else
        std::auto_ptr<Glib::Error> error;
        tDestPath->make_directory_with_parents(error);
#endif
    return;
}

void
Photo::generate_using_gdkpixbuf(const Glib::ustring & path,
                                      const Resolution & new_size_hint)
{
    PixbufPtr thumbnail;
    try
    {
        thumbnail = Gdk::Pixbuf::create_from_file(
                                     path,
                                     -1, //We keep aspect
                                     new_size_hint.get_y(),
                                     true);
    }
    catch (const Glib::FileError & e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
        return;
    }
    catch (const Gdk::PixbufError & e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
        return;
    }

    thumbnail->save( get_thumbnail_path(), "jpeg" );    

    set_thumbnail_resolution( 
        Resolution( 
            thumbnail->get_width(), thumbnail->get_height() ) );
    return;
}
#endif

} // namespace Solang
