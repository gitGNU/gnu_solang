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

#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <strings.h>

#include <glibmm.h>

#include "histogram.h"

namespace Solang
{

Histogram::Histogram()
    :data_(NULL),
    maxLevel_(0),
    buffer_()
{
    max[0]=max[1]=max[2]=0;
}

Histogram::~Histogram()
{
    cleanup();
}

void
Histogram::createData( const Histogram::DataBuffer &data )
{
    if( NULL != data_ )
        cleanup();
    buffer_ = data;
    //Buffer is a 2D data-buffer
    //Rows = 2^bitsPerChannel (256 for 8bit)
    //Columns = 5 (R,G,B,A,Value)
    gsize levels = 1 << data.bitsPerChannel_;
    gsize channels = data.numChannels_;
    gsize rowStride = data.rowstride_;
    data_ = g_new0( guint32 *, channels );
    for(guint channel = 0; channel < channels; channel++ )
    {
        data_[ channel ] = g_new0( guint32, levels );
        ::bzero( data_[ channel ], levels * sizeof(guint32) );
    }
    guint8 *pixelData = data.data_;

    //Buffer is created
    guint8 *line = pixelData;
    for( gsize i = 0; i < data.height_; i ++ )
    {
        guint8 *pixel = line;
        line += rowStride;
        for( gsize j = 0; j < data.width_; j ++ )
        {
            data_[ RED ][ pixel[ RED ] ]++;
            data_[ GREEN ][ pixel[ GREEN ] ]++;
            data_[ BLUE ][ pixel[ BLUE ] ]++;
            pixel += data.numChannels_;
        }
    }

    max[RED]=max[GREEN]=max[BLUE]=0;
    for( gsize i = 0; i < levels; i++ )
    {
        guint32 r = data_[ RED ][ i ];
        guint32 g = data_[ GREEN ][ i ];
        guint32 b = data_[ BLUE ][ i ];
        max[RED] = ( max[RED] < r )?r:max[RED];
        max[GREEN] = ( max[GREEN] < g )?g:max[GREEN];
        max[BLUE] = ( max[BLUE]< b )?b:max[BLUE];
    }
    guint64 total = 0L;
    for( gsize i = 0; i < channels; i++ )
    {
        for( gsize j = 0; j < levels; j++ )
        {
            maxLevel_ = ( maxLevel_ < data_[ i ][ j ] )
                                ? data_[ i ][ j ] : maxLevel_;
            total += data_[ i ][ j ];
        }
    }
}

Glib::RefPtr<Gdk::Pixbuf>
Histogram::get_pixbuf( Type type, gsize width, gsize height,
                        Channel channel ) throw()
{
    if( Histogram::LINEAR == type )
    {
        return get_linear_histogram( width, height, channel );
    }
    return get_logarithmic_histogram(
                                width, height, channel );
}

Glib::RefPtr<Gdk::Pixbuf>
Histogram::get_logarithmic_histogram(
                gsize resultWidth, gsize resultHeight,
                Histogram::Channel resultChannel) throw()
{
    gsize maxLevel = 1 << buffer_.bitsPerChannel_;
    gsize width = maxLevel;
    gsize height = maxLevel;
    guint8 *pixels = g_new0( guint8, width * height * 3);
    memset( pixels, 0x28, 3 * width * height ); //dark grey

    //Grid lines
    for( guint32 i = 0; i < width; i += 64 )
    {
        for( guint32 j = 0; j < height; j+=4 )
        {
            guint8 *fill = ( pixels + ( i + j * width ) * 3 );
            fill[ RED ] = fill[ GREEN ] = fill[ BLUE ] = 0x73;
        }
    }
    for( guint32 i = 0; i < width; i +=4 )
    {
        for( guint32 j = 0; j < height; j+=64 )
        {
            guint8 *fill = ( pixels + ( i + j * width ) * 3 );
            fill[ RED ] = fill[ GREEN ] = fill[ BLUE ] = 0x73;
        }
    }
	if( !data_ )
	{
	    Glib::RefPtr<Gdk::Pixbuf> buf
            = Gdk::Pixbuf::create_from_data(
                        (guint8 *)pixels,
                        Gdk::COLORSPACE_RGB,
                        false,
                        8,
                        width,
                        height,
                        3 * width )->flip( false )->
                add_alpha(false, 0, 0, 0)->scale_simple(
                                resultWidth, resultHeight,
                                Gdk::INTERP_HYPER);
		if( pixels )
			g_free( pixels );

		return buf;
	}

    for( gsize i = 0; i < maxLevel; i++ )
    {
        //we have to set rgb and A for each pixel
        // and we are going L2R

        //Considering we use only RGB channels
        guint8 channel = 0;
        guint32 maxVal = 0;
        for( ; channel < 3; channel++ )
        {
            double level = (double)data_[ channel ][ i ];
            double logLvl = (level>0)?log(level):0.0;
            double logMax = (max[channel]>0)?log((double)max[channel]):1.0;
            guint32 normLvl = (guint32)((double)( logLvl/logMax) * (double)(height-1));
            maxVal = ( maxVal < normLvl ) ? normLvl : maxVal;
            if( RGB == resultChannel
                                || channel == resultChannel )
            {
                for( gsize j = 0; j <= normLvl; j++ )
                {
                    Pixel *fill = (Pixel *)(
                                pixels + (i  + j * width)*3);
                    fill->channels_[ channel ]
                            |= (j + 2 > normLvl ) ? 0xFF : 0xAA;
                }
            }
        }

        //Demarkate value channel
        for(guint32 j = maxVal - ((maxVal > 0 )?1:0);
                                        j <= maxVal; j++ )
        {
            guint8 *fill = (pixels + (i  + j * width)*3);
            fill[ RED ] = fill[ GREEN ] = fill[ BLUE ] = 0xFF;
        }
    }

    Glib::RefPtr<Gdk::Pixbuf> buf
            = Gdk::Pixbuf::create_from_data(
                        (guint8 *)pixels,
                        Gdk::COLORSPACE_RGB,
                        false,
                        8,
                        width,
                        height,
                        3 * width )->flip( false )->
                add_alpha(false, 0, 0, 0)->scale_simple(
                                resultWidth, resultHeight,
                                Gdk::INTERP_HYPER);
    if( pixels )
        g_free( pixels );

    return buf;

}

Glib::RefPtr<Gdk::Pixbuf>
Histogram::get_linear_histogram(
                gsize resultWidth, gsize resultHeight,
                Histogram::Channel resultChannel) throw()
{
    gsize maxLevel = 1 << buffer_.bitsPerChannel_;
    gsize width = maxLevel;
    gsize height = maxLevel;
    guint8 *pixels = g_new0( guint8, width * height * 3);
    memset( pixels, 0x28, 3 * width * height ); //dark grey

    //Grid lines
    for( guint32 i = 0; i < width; i += 64 )
    {
        for( guint32 j = 0; j < height; j+=4 )
        {
            guint8 *fill = ( pixels + ( i + j * width ) * 3 );
            fill[ RED ] = fill[ GREEN ] = fill[ BLUE ] = 0x73;
        }
    }
    for( guint32 i = 0; i < width; i +=4 )
    {
        for( guint32 j = 0; j < height; j+=64 )
        {
            guint8 *fill = ( pixels + ( i + j * width ) * 3 );
            fill[ RED ] = fill[ GREEN ] = fill[ BLUE ] = 0x73;
        }
    }
	if( !data_ )
	{
	    Glib::RefPtr<Gdk::Pixbuf> buf
            = Gdk::Pixbuf::create_from_data(
                        (guint8 *)pixels,
                        Gdk::COLORSPACE_RGB,
                        false,
                        8,
                        width,
                        height,
                        3 * width )->flip( false )->
                add_alpha(false, 0, 0, 0)->scale_simple(
                                resultWidth, resultHeight,
                                Gdk::INTERP_HYPER);
		if( pixels )
			g_free( pixels );

		return buf;
	}

    for( gsize i = 0; i < maxLevel; i++ )
    {
        //we have to set rgb and A for each pixel
        // and we are going L2R

        //Considering we use only RGB channels
        guint8 channel = 0;
        guint32 maxVal = 0;
        for( ; channel < 3; channel++ )
        {
            double level = (double)data_[ channel ][ i ];
            guint32 normLvl = (guint32)((double)( level/maxLevel_ ) * (double)(height-1));
            maxVal = ( maxVal < normLvl ) ? normLvl : maxVal;
            if( RGB == resultChannel
                                || channel == resultChannel )
            {
                for( gsize j = 0; j <= normLvl; j++ )
                {
                    Pixel *fill = (Pixel *)(
                                pixels + (i  + j * width)*3);
                    fill->channels_[ channel ]
                            |= (j + 2 > normLvl ) ? 0xFF : 0xAA;
                }
            }
        }

        //Demarkate value channel
        for(guint32 j = maxVal - ((maxVal > 0 )?1:0);
                                        j <= maxVal; j++ )
        {
            guint8 *fill = (pixels + (i  + j * width)*3);
            fill[ RED ] = fill[ GREEN ] = fill[ BLUE ] = 0xFF;
        }
    }

    Glib::RefPtr<Gdk::Pixbuf> buf
            = Gdk::Pixbuf::create_from_data(
                        (guint8 *)pixels,
                        Gdk::COLORSPACE_RGB,
                        false,
                        8,
                        width,
                        height,
                        3 * width )->flip( false )->
                add_alpha(false, 0, 0, 0)->scale_simple(
                                resultWidth, resultHeight,
                                Gdk::INTERP_HYPER);
    if( pixels )
        g_free( pixels );

    return buf;

}

void
Histogram::cleanup()
{
    max[ RED ] = max[ GREEN ] = max[ BLUE ] = 0;
    maxLevel_ = 0;
    if( data_ )
    {
        gsize channels = buffer_.numChannels_;
        for( gsize i = 0; i < channels; i++ )
        {
            if( data_[ i ] )
                g_free( data_[ i ] );
        }
        g_free( data_ );
        data_ = NULL;
    }
}

} // namespace Solang
