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
#endif

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <gdkmm.h>

#include "editable-photo.h"
#include "rotate.h"

namespace Solang
{

Rotate::Rotate( Direction direction )
    :EditAction(),
    direction_( direction )
{
}

Rotate::~Rotate() throw()
{
}

void
Rotate::execute( EditablePhoto &photo) throw(Error)
{
    try
    {
        rotatePhoto( photo, direction_ );
    }
    catch( Error &e )
    {
        e.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
Rotate::reverse( EditablePhoto &photo ) throw(Error)
{
    Direction direction;

    switch( direction_ )
    {
        case Rotate::ROTATE_CLOCKWISE:
        {
            direction = Rotate::ROTATE_COUNTERCLOCKWISE;
            break;
        }
        case Rotate::ROTATE_COUNTERCLOCKWISE:
        {
            direction = Rotate::ROTATE_CLOCKWISE;
            break;
        }
    }
    try
    {
        rotatePhoto( photo, direction );
    }
    catch( Error &e )
    {
        e.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
Rotate::rotatePhoto( EditablePhoto &photo,
                    Direction direction ) throw(Error)
{
    try
    {
        //execute( buffer );
        Gdk::PixbufRotation rotation = Gdk::PIXBUF_ROTATE_NONE;

        switch( direction )
        {
            case Rotate::ROTATE_CLOCKWISE:
            {
                rotation = Gdk::PIXBUF_ROTATE_CLOCKWISE;
                break;
            }
            case Rotate::ROTATE_COUNTERCLOCKWISE:
            {
                rotation = Gdk::PIXBUF_ROTATE_COUNTERCLOCKWISE;
                break;
            }
        }

        PixbufPtr dest
                    = photo.get_buffer()->rotate_simple( rotation );
        photo.set_buffer( dest );
        PixbufPtr thumb = photo.get_photo()->get_thumbnail_buffer();
        if( thumb )
        {
            photo.get_photo()->set_thumbnail_buffer(
                    thumb ->rotate_simple( rotation ));
        }

        //Update exif
        {
            Exiv2::ExifData &exifData = photo.get_exif_data();
            int32_t width = exifData["Exif.Photo.PixelXDimension"].toLong();
            int32_t height = exifData["Exif.Photo.PixelYDimension"].toLong();
            exifData["Exif.Photo.PixelXDimension"] = height;
            exifData["Exif.Photo.PixelYDimension"] = width;
        }
        {
            Exiv2::XmpData &xmpData = photo.get_xmp_data();
            int32_t width = xmpData["Xmp.exif.PixelXDimension"].toLong();
            int32_t height = xmpData["Xmp.exif.PixelYDimension"].toLong();
            xmpData["Xmp.exif.PixelXDimension"] = height;
            xmpData["Xmp.exif.PixelYDimension"] = width;
        }

    }
    catch( Error &e )
    {
        e.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
    catch( Glib::Error &e )
    {
        std::cerr<<"Error: "<<e.what()<<std::endl;
    }
}

} //namespace Solang
