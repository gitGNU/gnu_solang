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

extern "C"
{
#include <babl/babl.h>
}

#include <geglmm.h>
#include <geglmm/buffer.h>
#include <geglmm/processor.h>

#include "operation.h"
#include "progress-observer.h"

namespace Solang
{

Operation::Operation() throw() :
    IOperation()
{
}

Operation::~Operation() throw()
{
}

BufferPtr
Operation::apply(const BufferPtr & buffer,
                 const ProgressObserverPtr & observer) throw()
{
    if (0 != observer)
    {
        observer->set_event_description(get_description());
        observer->set_num_events(100);
        observer->set_current_events(0);
    }

    const NodePtr root = Gegl::Node::create();
    root->set("format", babl_format("RGB u8"));

    const NodePtr buffer_source = root->new_child(
                                      "operation",
                                      "gegl:buffer-source");
    gegl_node_set(buffer_source->gobj(),
                  "buffer", buffer->gobj(),
                  NULL);

    const NodePtr operation = get_node(root);

    const NodePtr buffer_sink = root->new_child("operation",
                                                "gegl:buffer-sink");
    GeglBuffer * output_buffer;
    gegl_node_set(buffer_sink->gobj(),
                  "buffer", &output_buffer,
                  NULL);

    buffer_source->link(operation)->link(buffer_sink);

    gdouble progress;
    GeglProcessor * const processor = gegl_node_new_processor(
                                          buffer_sink->gobj(), 0);

    while (true == gegl_processor_work(processor, &progress))
    {
        if (0 != observer)
        {
            observer->set_current_events(static_cast<guint64>(
                                             progress * 100.0));
            if (true == observer->get_stop())
            {
                break;
            }
        }
    }
    g_object_unref(processor);

    if (0 != observer)
    {
        observer->reset();
    }

    return Glib::wrap(output_buffer, false);
}

} // namespace Solang
