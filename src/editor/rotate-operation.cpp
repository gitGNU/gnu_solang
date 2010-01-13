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
#include <glibmm/i18n.h>

#include "rotate-operation.h"

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
RotateOperation::get_node(const NodePtr & root) const throw()
{
    const NodePtr operation = root->new_child("operation",
                                              "gegl:rotate");
    gegl_node_set(operation->gobj(),
                  "origin-x", originX_,
                  "origin-y", originY_,
                  "filter", filter_.c_str(),
                  "hard-edges", hardEdges_,
                  "lanczos-width", lanczosWidth_,
                  "degrees", degrees_,
                  NULL);

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
