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

#include "photo.h"
#include "thumbbuf-maker.h"
#include "thumbnail.h"

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
ThumbbufMaker::operator()(const PhotoPtr & photo) throw()
{
    const Thumbnail & thumbnail = photo->get_thumbnail();

    std::string path;
    try
    {
        path = Glib::filename_from_utf8(thumbnail.get_path());
    }
    catch (const Glib::ConvertError & e)
    {
        g_warning("%s", e.what().c_str());
        return PixbufPtr(0);
    }

    if (false == Glib::file_test(path, Glib::FILE_TEST_EXISTS))
    {
        try
        {
            path = Glib::filename_from_utf8(
                             photo->get_disk_file_path());
        }
        catch (const Glib::ConvertError & e)
        {
            g_warning("%s", e.what().c_str());
            return PixbufPtr(0);
        }
    }

    PixbufPtr pixbuf;
    try
    {
        pixbuf = Gdk::Pixbuf::create_from_file(path, width_, -1, true);
    }
    catch (const Glib::FileError & e)
    {
        g_warning("%s", e.what().c_str());
        return PixbufPtr(0);
    }
    catch (const Gdk::PixbufError & e)
    {
        g_warning("%s", e.what().c_str());
        return PixbufPtr(0);
    }

    if (true == rotate_)
    {
        pixbuf = Glib::wrap(gdk_pixbuf_apply_embedded_orientation(
                                pixbuf->gobj()), false);
    }

    const double height = static_cast<double>(pixbuf->get_height());

    if (height_ < height)
    {
        const double width = static_cast<double>(pixbuf->get_width());
        const double aspect_ratio = width / height;
        pixbuf = pixbuf->scale_simple(
                     static_cast<gint>(height_ * aspect_ratio),
                     height_, Gdk::INTERP_BILINEAR);
    }

    return pixbuf;
}

} // namespace Solang
