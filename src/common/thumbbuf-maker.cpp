/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#include <cstdio>

#include <giomm.h>

#include "photo.h"
#include "thumbbuf-maker.h"

namespace Solang
{

ThumbbufMaker::ThumbbufMaker(guint width, guint height, bool rotate)
                             throw() :
    std::unary_function<const PhotoPtr &, PixbufPtr>(),
    rotate_(rotate),
    width_(width),
    height_(height)
{
}

ThumbbufMaker::ThumbbufMaker(const ThumbbufMaker & source) throw() :
    std::unary_function<const PhotoPtr &, PixbufPtr>(source),
    rotate_(source.rotate_),
    width_(source.width_),
    height_(source.height_)
{
}

ThumbbufMaker::~ThumbbufMaker() throw()
{
}

ThumbbufMaker &
ThumbbufMaker::operator=(const ThumbbufMaker & source) throw()
{
    if (this != &source)
    {
        std::unary_function<const PhotoPtr &, PixbufPtr>
            ::operator=(source);
        rotate_ = source.rotate_;
        width_ = source.width_;
        height_ = source.height_;
    }

    return *this;
}

PixbufPtr
ThumbbufMaker::operator()(const PhotoPtr & photo) throw(Gdk::PixbufError,
                                                        Glib::FileError)
{
    const std::string path = photo->get_thumbnail_path();
    PixbufPtr pixbuf = Gdk::Pixbuf::create_from_file(path, width_, -1, true);

    if (true == rotate_)
    {
        pixbuf = Glib::wrap(gdk_pixbuf_apply_embedded_orientation(
                                pixbuf->gobj()), false);
    }

    const gint height = pixbuf->get_height();

    if (height_ < height)
    {
        const gdouble width = static_cast<gdouble>(pixbuf->get_width());
        const gdouble aspect_ratio = width / static_cast<gdouble>(height);
        pixbuf = pixbuf->scale_simple(
                     static_cast<gint>(height_ * aspect_ratio),
                     height_, Gdk::INTERP_BILINEAR);
    }

    return pixbuf;
}

} // namespace Solang
