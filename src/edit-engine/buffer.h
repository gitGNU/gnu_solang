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

#ifndef SOLANG_BUFFER_H
#define SOLANG_BUFFER_H

#include <gegl.h>
#include <glibmm.h>

#include "edit-types.h"
#include "error.h"
#include "types.h"

namespace Solang
{

class Buffer
{
    public:
        Buffer();
        Buffer( const GeglBufferPtr &buffer );
        Buffer( const GeglRectangle & rect );
        Buffer( const Buffer &rhs );
        ~Buffer();

        Buffer &
        operator=( const Buffer &rhs );

        PixbufPtr
        get_pixbuf() throw();

        inline GeglBufferPtr
        get_buffer() throw();

        void
        set_buffer( const GeglBufferPtr &buffer );

        inline Glib::Dispatcher &
        get_refresh() throw();

        void
        open_image_file( const Glib::ustring &path,
                        EditEnginePtr engine );

        inline bool
        is_empty() const throw();

        static void
        serialize(const BufferPtr &buffer,
                    const Glib::ustring &path) throw(Error);
        static BufferPtr
        deserialize(const Glib::ustring &path) throw(Error);

    private:
        GeglBufferPtr
        create_buffer(const GeglRectangle &rect);

        static EditBits BITS_;

        GeglBufferPtr buffer_;

        Glib::Dispatcher refresh_;
};

inline GeglBufferPtr
Buffer::get_buffer() throw()
{
    return buffer_;
}

inline Glib::Dispatcher &
Buffer::get_refresh() throw()
{
    return refresh_;
}

inline bool
Buffer::is_empty() const throw()
{
    return NULL == buffer_;
}

} //namespace solang

#endif //SOLANG_BUFFER_H
