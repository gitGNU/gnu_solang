/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2010 Debarshi Ray <rishi@gnu.org>
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

#include <geglmm.h>
#include <geglmm/buffer.h>

#include "buffer-pixbuf-converter.h"

namespace Solang
{

BufferPixbufConverter::BufferPixbufConverter() throw() :
    std::unary_function<const BufferPtr &, PixbufPtr>()
{
}

BufferPixbufConverter::BufferPixbufConverter(
    const BufferPixbufConverter & source) throw() :
    std::unary_function<const BufferPtr &, PixbufPtr>(source)
{
}

BufferPixbufConverter::~BufferPixbufConverter() throw()
{
}

BufferPixbufConverter &
BufferPixbufConverter::operator=(const BufferPixbufConverter & source)
                                 throw()
{
    if (this != &source)
    {
        std::unary_function<const BufferPtr &, PixbufPtr>
            ::operator=(source);
    }

    return *this;
}

PixbufPtr
BufferPixbufConverter::operator()(const BufferPtr & buffer) throw()
{
    const NodePtr root = Gegl::Node::create();
    root->set("format", babl_format("RGB u8"));

    const NodePtr buffer_source = root->new_child(
                                      "operation",
                                      "gegl:buffer-source");
    gegl_node_set(buffer_source->gobj(),
                  "buffer", buffer->gobj(),
                  NULL);

    const NodePtr save_pixbuf = root->new_child("operation",
                                         "gegl:save-pixbuf");
    GdkPixbuf * pixbuf;
    gegl_node_set(save_pixbuf->gobj(), "pixbuf", &pixbuf, NULL);

    buffer_source->link(save_pixbuf);
    save_pixbuf->process();

    return Glib::wrap(pixbuf, false);
}

} // namespace Solang
