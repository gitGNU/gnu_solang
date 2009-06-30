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

#include <glibmm.h>

#include "flickr-photo-uri-builder.h"

namespace Solang
{

static const char sizes[] = {'b', 'm', 't', 's'};

FlickrPhotoUriBuilder::FlickrPhotoUriBuilder(char size) throw() :
    std::unary_function<flickcurl_photo *, char *>(),
    size_(size)
{
}

FlickrPhotoUriBuilder::FlickrPhotoUriBuilder(
    const FlickrPhotoUriBuilder & source) throw() :
    std::unary_function<flickcurl_photo *, char *>(source),
    size_(source.size_)
{
}

FlickrPhotoUriBuilder::~FlickrPhotoUriBuilder() throw()
{
}

FlickrPhotoUriBuilder &
FlickrPhotoUriBuilder::operator=(const FlickrPhotoUriBuilder & source)
                                 throw()
{
    if (this != &source)
    {
        std::unary_function<flickcurl_photo *, char *>
            ::operator=(source);
        size_ = source.size_;
    }

    return *this;
}

char *
FlickrPhotoUriBuilder::operator()(flickcurl_photo * photo) throw()
{
    if ('\0' == size_)
    {
        guint i;

        for (i = 0; sizeof(sizes) / sizeof(sizes[0]) > i; i++)
        {
            char * const uri = flickcurl_photo_as_source_uri(
                                   photo, sizes[i]);

            if (0 != uri)
            {
                return uri;
            }
        }
    }
    else
    {
        return flickcurl_photo_as_source_uri(photo, size_);
    }

    return 0;
}

} // namespace Solang
