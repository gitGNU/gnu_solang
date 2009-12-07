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

#ifndef SOLANG_THUMBBUF_MAKER_H
#define SOLANG_THUMBBUF_MAKER_H

#include <functional>

#include <gdkmm.h>
#include <glibmm.h>

#include "types.h"

namespace Solang
{

class ThumbbufMaker :
    public std::unary_function<const PhotoPtr &, PixbufPtr>
{
    public:
        ThumbbufMaker(guint width, guint height, bool rotate) throw();

        ThumbbufMaker(const ThumbbufMaker & source) throw();

        ~ThumbbufMaker() throw();

        ThumbbufMaker &
        operator=(const ThumbbufMaker & source) throw();

        PixbufPtr
        operator()(const PhotoPtr & photo) throw();

    protected:
        PixbufLoaderPtr
        create_pixbuf_loader(const std::string & path) const
                             throw(Gdk::PixbufError);

    private:
        bool rotate_;

        guint width_;

        guint height_;
};

} // namespace Solang

#endif // SOLANG_THUMBBUF_MAKER_H
