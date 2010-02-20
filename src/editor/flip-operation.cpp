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

#include "flip-operation.h"
#include "utils.h"

namespace Solang
{

FlipOperation::FlipOperation(gdouble origin_x,
                             gdouble origin_y,
                             const std::string & filter,
                             bool hard_edges,
                             gint lanczos_width,
                             gdouble x,
                             gdouble y) throw() :
    Operation(),
    hardEdges_(hard_edges),
    originX_(origin_x),
    originY_(origin_y),
    x_(x),
    y_(y),
    lanczosWidth_(lanczos_width),
    filter_(filter),
    signalReady_()
{
}

FlipOperation::~FlipOperation() throw()
{
}

Glib::ustring
FlipOperation::get_description() const throw()
{
    return Glib::ustring(_("Flipping..."));
}

NodePtr
FlipOperation::get_node(const BufferPtr & buffer,
                        const NodePtr & root) const throw()
{
    const NodePtr operation = Gegl::Node::create();
    operation->set("format", babl_format("RGB u8"));

    const NodePtr reflect = operation->new_child("operation",
                                                 "gegl:reflect");
    gegl_node_set(reflect->gobj(),
                  "origin-x", originX_,
                  "origin-y", originY_,
                  "filter", filter_.c_str(),
                  "hard-edges", hardEdges_,
                  "lanczos-width", lanczosWidth_,
                  "x", x_,
                  "y", y_,
                  NULL);

    const Gegl::Rectangle & extent = buffer->get_extent();
    const gint height = extent.gobj()->height;
    const gint width = extent.gobj()->width;

    gint translate_x;
    gint translate_y;

    if (is_zero(x_) && !is_zero(y_)) // horizontal
    {
        translate_x = width;
        translate_y = 0;
    }
    else if (!is_zero(x_) && is_zero(y_)) // vertical
    {
        translate_x = 0;
        translate_y = height;
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
                  "width", static_cast<gdouble>(width),
                  "height", static_cast<gdouble>(height),
                  NULL);

    const NodePtr input = operation->get_input_proxy("input");
    const NodePtr output = operation->get_output_proxy("output");

    input->link(reflect)->link(translate)->link(crop)->link(output);

    return operation;
}

FlipOperation::SignalReady &
FlipOperation::signal_ready() throw()
{
    return signalReady_;
}

void
FlipOperation::set_hard_edges(bool hard_edges) throw()
{
    hardEdges_ = hard_edges;
}

void
FlipOperation::set_origin_x(gdouble origin_x) throw()
{
    originX_ = origin_x;
}

void
FlipOperation::set_origin_y(gdouble origin_y) throw()
{
    originY_ = origin_y;
}

void
FlipOperation::set_x(gdouble x) throw()
{
    x_ = x;
}

void
FlipOperation::set_y(gdouble y) throw()
{
    y_ = y;
}

void
FlipOperation::set_lanczos_width(gdouble lanczos_width) throw()
{
    lanczosWidth_ = lanczos_width;
}

void
FlipOperation::set_filter(const std::string & filter) throw()
{
    filter_ = filter;
}

} // namespace Solang
