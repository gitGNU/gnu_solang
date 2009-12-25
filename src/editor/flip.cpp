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

#include <iostream>

#include <gdkmm.h>

#include "editable-photo.h"
#include "flip.h"

namespace Solang
{

Flip::Flip( bool horizontal )
    :EditAction(),
    horizontal_( horizontal )
{
}

Flip::~Flip() throw()
{
}

void
Flip::execute( EditablePhoto &photo) throw(Error)
{
    try
    {
        Glib::RefPtr<Gdk::Pixbuf> dest
                    = photo.get_buffer()->flip( horizontal_ );
        photo.set_buffer( dest );

        PixbufPtr thumb = photo.get_photo()->get_thumbnail_buffer();
        if( thumb )
        {
            photo.get_photo()->set_thumbnail_buffer(
                                        thumb ->flip( horizontal_ ));
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

void
Flip::reverse( EditablePhoto &photo) throw(Error)
{
    try
    {
        execute( photo ); //Flipping is complementary
    }
    catch( Error &e )
    {
        e.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

EditActionPtr
Flip::clone()
{
    return EditActionPtr( new Flip( horizontal_ ) );
}

} //namespace Solang
