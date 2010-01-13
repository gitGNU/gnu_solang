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

#include "buffer-maker.h"

namespace Solang
{

BufferMaker::BufferMaker() throw() :
    std::unary_function<const std::string &, BufferPtr>()
{
}

BufferMaker::BufferMaker(
    const BufferMaker & source) throw() :
    std::unary_function<const std::string &, BufferPtr>(source)
{
}

BufferMaker::~BufferMaker() throw()
{
}

BufferMaker &
BufferMaker::operator=(const BufferMaker & source) throw()
{
    if (this != &source)
    {
        std::unary_function<const std::string &, BufferPtr>
            ::operator=(source);
    }

    return *this;
}

BufferPtr
BufferMaker::operator()(const std::string & path) throw()
{
    const NodePtr root = Gegl::Node::create();
    root->set("format", babl_format("RGB u8"));

    const NodePtr load = root->new_child("operation",
                                         "gegl:load");
    gegl_node_set(load->gobj(), "path", path.c_str(), NULL);

    const NodePtr buffer_sink = root->new_child("operation",
                                                "gegl:buffer-sink");
    GeglBuffer * buffer;
    gegl_node_set(buffer_sink->gobj(), "buffer", &buffer, NULL);

    load->link(buffer_sink);
    buffer_sink->process();

    return Glib::wrap(buffer, false);
}

} // namespace Solang
