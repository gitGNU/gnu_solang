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

#ifndef SOLANG_FLICKR_PHOTO_URI_BUILDER_H
#define SOLANG_FLICKR_PHOTO_URI_BUILDER_H

#include <functional>

#include <flickcurl.h>

namespace Solang
{

class FlickrPhotoUriBuilder :
    public std::unary_function<flickcurl_photo *, char *>
{
    public:
        FlickrPhotoUriBuilder(char size = '\0') throw();

        FlickrPhotoUriBuilder(const FlickrPhotoUriBuilder & source)
                              throw();

        virtual
        ~FlickrPhotoUriBuilder() throw();

        FlickrPhotoUriBuilder &
        operator=(const FlickrPhotoUriBuilder & source) throw();

        char *
        operator()(flickcurl_photo * photo) throw();

    protected:
        char size_;
};

} // namespace Solang

#endif // SOLANG_FLICKR_PHOTO_URI_BUILDER_H
