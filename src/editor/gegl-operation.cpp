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

#include "buffer.h"
#include "editable-photo.h"
#include "gegl-operation.h"
#include "operation.h"

namespace Solang
{

GeglOperation::GeglOperation( const OperationPtr &operation,
                        const ProgressObserverPtr &observer )
    :EditAction(),
    operation_( operation ),
    oldBuf_(),
    newBuf_(),
    observer_( observer )
{
}

GeglOperation::~GeglOperation() throw()
{
}

void
GeglOperation::execute( EditablePhoto &photo) throw(Error)
{
    if( !newBuf_ )
    {
        oldBuf_ = photo.get_buffer();
        BufferPtr geglBuf = operation_->apply( observer_,
                                photo.get_edit_buffer() );
        //photo.set_edit_buffer( newBuf );
        PixbufPtr outBuf = geglBuf->get_pixbuf();
        newBuf_ = outBuf;
        //operation_.reset();
    }
    photo.set_buffer( newBuf_, false );
    PixbufPtr thumb = photo.get_photo()->get_thumbnail_buffer();
    if( thumb )
    {
        int width = thumb->get_width();
        int height = thumb->get_height();
        thumb = newBuf_->scale_simple(
                width, height, Gdk::INTERP_NEAREST );
        photo.get_photo()->set_thumbnail_buffer( thumb );
    }
}

void
GeglOperation::reverse( EditablePhoto &photo) throw(Error)
{
#if 0
    BufferPtr newBuf = operation_->revert( observer_ );
    photo.set_edit_buffer( newBuf );
    PixbufPtr outBuf = newBuf->get_pixbuf();
    photo.set_buffer( outBuf, false );
#endif
    photo.set_buffer( oldBuf_, false );
    PixbufPtr thumb = photo.get_photo()->get_thumbnail_buffer();
    if( thumb )
    {
        int width = thumb->get_width();
        int height = thumb->get_height();
        thumb = oldBuf_->scale_simple(
                width, height, Gdk::INTERP_NEAREST );
        photo.get_photo()->set_thumbnail_buffer( thumb );
    }
}

EditActionPtr
GeglOperation::clone()
{
    return EditActionPtr( new GeglOperation( operation_, observer_ ) );
}


} //namespace Solang
