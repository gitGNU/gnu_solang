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
#include "scale.h"

namespace Solang
{

Scale::Scale( double fraction )
    :EditAction(),
    fraction_( fraction ),
    oldBuf_()
{
}

Scale::~Scale() throw()
{
}

void
Scale::execute( EditablePhoto &photo) throw(Error)
{
    try
    {
        oldBuf_ = photo.get_buffer();
        PixbufPtr dest = oldBuf_->scale_simple(
                            oldBuf_->get_width() * fraction_,
                            oldBuf_->get_height() * fraction_,
                            Gdk::INTERP_HYPER);
        photo.set_buffer( dest );
    }
    catch( Error &e )
    {
        e.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
Scale::reverse( EditablePhoto &photo ) throw(Error)
{
    photo.set_buffer( oldBuf_ );
    return;
}

} //namespace Solang
