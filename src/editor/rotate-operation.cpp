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
#include <glibmm/i18n.h>

#include "rotate-operation.h"
#include "utils.h"

namespace Solang
{

RotateOperation::RotateOperation(gdouble origin_x,
                                 gdouble origin_y,
                                 const std::string & filter,
                                 bool hard_edges,
                                 gint lanczos_width,
                                 gdouble degrees) throw() :
    Operation(),
    hardEdges_(hard_edges),
    degrees_(degrees),
    originX_(origin_x),
    originY_(origin_y),
    lanczosWidth_(lanczos_width),
    filter_(filter),
    signalReady_()
{
}

RotateOperation::~RotateOperation() throw()
{
}

Glib::ustring
RotateOperation::get_description() const throw()
{
    return Glib::ustring(_("Rotating..."));
}

NodePtr
RotateOperation::get_node(const BufferPtr & buffer,
                          const NodePtr & root) const throw()
{
    const NodePtr operation = Gegl::Node::create();
    operation->set("format", babl_format("RGB u8"));

    const NodePtr rotate = root->new_child("operation",
                                           "gegl:rotate");
    gegl_node_set(rotate->gobj(),
                  "origin-x", originX_,
                  "origin-y", originY_,
                  "filter", filter_.c_str(),
                  "hard-edges", hardEdges_,
                  "lanczos-width", lanczosWidth_,
                  "degrees", degrees_,
                  NULL);

    const Gegl::Rectangle & extent = buffer->get_extent();
    const gint height = extent.gobj()->height;
    const gint width = extent.gobj()->width;

    gint translate_x;
    gint translate_y;

    if (is_equal(-90.0, degrees_)) // clockwise
    {
        translate_x = height;
        translate_y = 0;
    }
    else if (is_equal(90.0, degrees_)) // counter-clockwise
    {
        translate_x = 0;
        translate_y = width;
    }

    const NodePtr translate = operation->new_child("operation",
                                                   "gegl:translate");
    gegl_node_set(translate->gobj(),
                  "filter", "nearest",
                  "x", static_cast<gdouble>(translate_x),
                  "y", static_cast<gdouble>(translate_y),
                  NULL);

    const NodePtr crop = operation->new_child("operation",
                                              "gegl:crop");
    gegl_node_set(crop->gobj(),
                  "x", 0.0,
                  "y", 0.0,
                  "width", static_cast<gdouble>(height),
                  "height", static_cast<gdouble>(width),
                  NULL);

    const NodePtr input = operation->get_input_proxy("input");
    const NodePtr output = operation->get_output_proxy("output");

    input->link(rotate)->link(translate)->link(crop)->link(output);

    return operation;
}

RotateOperation::SignalReady &
RotateOperation::signal_ready() throw()
{
    return signalReady_;
}

void
RotateOperation::set_hard_edges(bool hard_edges) throw()
{
    hardEdges_ = hard_edges;
}

void
RotateOperation::set_origin_x(gdouble origin_x) throw()
{
    originX_ = origin_x;
}

void
RotateOperation::set_origin_y(gdouble origin_y) throw()
{
    originY_ = origin_y;
}

void
RotateOperation::set_degrees(gdouble degrees) throw()
{
    degrees_ = degrees;
}

void
RotateOperation::set_lanczos_width(gdouble lanczos_width) throw()
{
    lanczosWidth_ = lanczos_width;
}

void
RotateOperation::set_filter(const std::string & filter) throw()
{
    filter_ = filter;
}

} // namespace Solang
