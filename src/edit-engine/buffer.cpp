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

#ifndef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H

extern "C"
{
#include <babl/babl.h>
}

#include <gdkmm.h>
#include <gegl.h>
#include <glibmm/i18n.h>
#include <glibmm.h>
#include <iostream>

#include "buffer.h"
#include "edit-engine.h"
#include "edit-utils.h"
#include "progress-observer.h"


namespace Solang
{


EditBits Buffer::BITS_ = EDIT_8_BITS;

Buffer::Buffer()
    :buffer_( NULL ),
    refresh_()
{
}

Buffer::Buffer( const GeglBufferPtr &buffer )
    :buffer_( buffer ),
    refresh_()
{
}

Buffer::Buffer( const GeglRectangle &rect )
    :buffer_( NULL ),
    refresh_()
{
    buffer_ = create_buffer( rect );
}

Buffer::Buffer( const Buffer &rhs )
    :buffer_( rhs.buffer_ ),
    refresh_()
{
}

Buffer::~Buffer()
{
    if( buffer_ )
        gegl_buffer_destroy( buffer_ );
    buffer_ = NULL;
}

Buffer &
Buffer::operator =( const Buffer & rhs )
{
    if( this != &rhs )
    {
        set_buffer( rhs.buffer_ );
    }
    return *this;
}

PixbufPtr
Buffer::get_pixbuf() throw()
{
    //The steps to do this are:
    //1. create Cairo surface from buffer
    //2. create pixmap from surface
    //3. put pixmap in pixbuf
    //Step 1:
    //const Babl *fmt= get_babl_format_from_cairo();
    const guint8 channels = 3;
    gint width = gegl_buffer_get_width( buffer_ );
    gint height = gegl_buffer_get_height( buffer_ );
    gpointer dataBuffer = g_new0 (guint8, gegl_buffer_get_pixel_count (buffer_) * channels);
    bzero( dataBuffer, gegl_buffer_get_pixel_count (buffer_) * channels);
    gegl_buffer_get( buffer_, //buffer
                    1.0, //scale
                    NULL, //extent .. we want the full thing
                    babl_format("RGB u8"), //color
                    dataBuffer, //pointer to buffer
                    channels * width ); //rowstride
#if 0
    Glib::RefPtr<Gdk::Pixmap> pixmap
            = Gdk::Pixmap::create(
                    Glib::RefPtr<Gdk::Drawable>(),
                    width, height, 32 );
    Cairo::RefPtr<Cairo::Context> context
            = pixmap->create_cairo_context();
    context->set_source( surface, 0, 0 );
    context->paint();
#endif
    std::cout<<"Here"<<std::endl;
    return Gdk::Pixbuf::create_from_data (
                    reinterpret_cast<guint8 *>(dataBuffer),
                                        //buffer
                    Gdk::COLORSPACE_RGB, //Colorspace
                    false, //has alpha channel
                    8, //bits per sample
                    width,
                    height,
                    channels * width ); //rowstride
                        //= num channels * width
}

GeglBufferPtr
Buffer::create_buffer(const GeglRectangle &rect)
{
    return gegl_buffer_new(
                    &rect, babl_format (
                        (EDIT_16_BITS == Buffer::BITS_ )
                        ?"RGB u16":"RGB u8"));
}

void
Buffer::set_buffer( const GeglBufferPtr &buffer )
{
    buffer_ = buffer;
    refresh_.emit();
}

void
Buffer::open_image_file( const Glib::ustring &path,
                            EditEnginePtr engine )
{
    if( buffer_ )
    {
        refresh_.emit();
        return;
    }
    std::cout<<"Open: "<<path<<std::endl;
    GeglNodePtr pAction = gegl_node_new_child(
                            engine->get_root_node(),
                            "operation", "gegl:load",
                            "path", path.c_str(),
                            NULL);
    GeglRectangle rect = gegl_node_get_bounding_box( pAction );
    GeglBufferPtr pBuf = NULL;
    GeglNodePtr pOut = gegl_node_new_child(
                            engine->get_root_node(),
                            "operation", "gegl:save-buffer",
                            "buffer", &pBuf,
                            NULL);
    gegl_node_link_many( pAction, pOut, NULL );

    GeglProcessor *processor
                = gegl_node_new_processor (pOut, &rect);
    double progress = 0.0;
    ProgressObserverPtr obs = engine->get_observer();
    obs->set_num_events( 100 );
    obs->set_event_description( _("Running Filter") );

    while ( gegl_processor_work (processor, &progress) )
    {
        obs->set_current_events( progress * 100.0 );
        std::cout<<"Prog: "<<progress<<std::endl;
    }

    gegl_processor_destroy (processor);

    g_object_unref( pAction );

    set_buffer( pBuf );

}

void
Buffer::serialize( const BufferPtr &buffer,
                    const Glib::ustring &path) throw(Error)
{
    GeglBufferPtr pBuf = buffer->get_buffer();
    const GeglRectangle *extent
                            = gegl_buffer_get_extent( pBuf );
    gegl_buffer_save( pBuf, path.c_str(), extent );
    return;
}

BufferPtr
Buffer::deserialize( const Glib::ustring &path ) throw(Error)
{
    GeglBufferPtr pBuf = gegl_buffer_load( path.c_str() );
    return BufferPtr( new Buffer( pBuf ) );
}

} //namespace Solang
