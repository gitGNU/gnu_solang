/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * thumbnail.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * thumbnail.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * thumbnail.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <iostream>

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <giomm.h>

#include "database.h"
#include "photo.h"
#include "thumbnail.h"

namespace Solang
{

const gint32 Thumbnail::PATH_COL = 5;
const gint32 Thumbnail::RES_X_COL = 6;
const gint32 Thumbnail::RES_Y_COL = 7;

Thumbnail::Thumbnail( )
    : path_(),
    resolution_()
{
}


#if 0
Thumbnail::Thumbnail( const UnrefPhotoPtr &photo,
                    const Glib::ustring &path,
                    const Resolution &resolution)
    : 
    path_(path),
    resolution_(resolution)
{
}
#endif

Thumbnail::Thumbnail( const Thumbnail &rhs )
    :path_( rhs.path_ ),
    resolution_( rhs.resolution_ )
{
}
        
Thumbnail::~Thumbnail() throw()
{
}

Thumbnail & 
Thumbnail::operator =( const Thumbnail &rhs )
{
    if( this != &rhs )
    {
        path_ = rhs.path_ ;
        resolution_ = rhs.resolution_ ;
    }
    return *this;    
}
    

void
Thumbnail::set_path(const Glib::ustring & path)
{
    path_ = path;
}

void
Thumbnail::set_resolution(const Resolution & resolution)
{
    resolution_ = resolution;
}

void
Thumbnail::generate(const Resolution & new_resolution,
                    const DatabasePtr & db,
                    const Photo &photo) throw(Error)
{
    if( new_resolution == get_resolution() 
            || new_resolution < get_resolution() )
    {
        return;
    }

    //We assume the photo has physical path
    if( photo.get_disk_file_path().empty() )
    {
        //TBD::Error
    }

    // We resize the thumbnail only.
    // Makes it faster.
    generate_using_gdkpixbuf(get_path(), new_resolution );

    return;
        
}

void
Thumbnail::generate( const Photo &photo )throw(Error)
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

    generate_using_gdkpixbuf( photo.get_disk_file_path(), 
                            Resolution(128, 128) );    

    return;
}

void
Thumbnail::generate(const Exiv2::ExifData & exifData,
                        const Photo &photo) throw(Error)
{
    // We assume the photo has physical path.
    if (photo.get_disk_file_path().empty())
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
        if (-1 == exifData.writeThumbnail(get_path().c_str()))
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

        set_resolution( res );
    }
#endif

    if (false == thumbnail_generated)
    {
        set_path( get_path() + ".jpg" );
        generate_using_gdkpixbuf(photo.get_disk_file_path(),
                                   Resolution(256, 256));
    }    
 
    return;
}

void
Thumbnail::make_thumb_path() throw(Error)
{
     Glib::RefPtr<Gio::File> tFile
                    = Gio::File::create_for_path( get_path() );
    std::cout<<tFile->get_path().substr(0,get_path().find( tFile->get_basename()))<<std::endl;
     Glib::RefPtr<Gio::File>tDestPath 
                    = Gio::File::create_for_path( 
                            tFile->get_path().substr(0,
                            get_path().find( tFile->get_basename())) );
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
Thumbnail::generate_using_gdkpixbuf(const Glib::ustring & path,
                                      const Resolution & new_size_hint)
{
#if 0
    Glib::ustring tPath = get_path() + ".jpg";
    set_path( tPath );

    // Open and copy source image.
    Magick::Image source(path);
    Magick::Image thumbnail(source);
    Magick::Geometry imageDim = thumbnail.density();

    // We need to keep aspect ratio.
    // So we consider the height only to derive the resize ratio.
    double ratio = static_cast<double>( 
                        new_size_hint.get_y() / imageDim.height() );
    Magick::Geometry newSize( imageDim.width() * ratio , 
                            imageDim.height() * ratio );
    thumbnail.zoom( newSize );
    thumbnail.write( get_path() );
#endif
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

    thumbnail->save( get_path(), "jpeg" );    

    set_resolution( Resolution( thumbnail->get_width(), thumbnail->get_height() ) );
    return;
}

void Thumbnail::insert( std::list<Gnome::Gda::Value> &values )
{
    values.push_back(Gnome::Gda::Value( get_path() ) );
    values.push_back(Gnome::Gda::Value( get_resolution().get_x()));
    values.push_back(Gnome::Gda::Value( get_resolution().get_y()));

    return;
}

void Thumbnail::update( 
                DataModelPtr &model, gint32 row) throw(Glib::Error)
try
{

    if( get_path().length() > 0 
            && get_path() != model->get_value_at( 
                                PATH_COL, row ).get_string() )
    {
        Gnome::Gda::Value v;
        v.set( Glib::ustring( get_path() ) );
        model->set_value_at( 
                PATH_COL, row, Gnome::Gda::Value( v ) );
    }

    if (0 < get_resolution().get_x()
            && get_resolution().get_x() != model->get_value_at( 
                                RES_X_COL, row ).get_int() )
    {
        model->set_value_at( 
                RES_X_COL, row, Gnome::Gda::Value( 
                                            get_resolution().get_x()));
    }

    if (0 < get_resolution().get_y()
            && get_resolution().get_y() != model->get_value_at( 
                                RES_Y_COL, row ).get_int() )
    {
        model->set_value_at( 
                RES_Y_COL, row, Gnome::Gda::Value( 
                                            get_resolution().get_y()));
    }

    return;

}
catch(Glib::Error &e)
{
    std::cerr<<"Error:"<<e.what()<<std::endl;
    throw;
}

void
Thumbnail::create(DataModelPtr & dataModel, int32_t row)
{
    set_path( dataModel->get_value_at( 
                                    PATH_COL, row ).get_string());
    Resolution tmp(dataModel->get_value_at(RES_X_COL, row).get_int(),
                   dataModel->get_value_at(RES_Y_COL, row).get_int());
    set_resolution(tmp);

    return;
}

} //namespace Solang
