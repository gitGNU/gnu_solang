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

#ifndef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H

#include <gegl.h>
#include <glibmm/i18n.h>

#include "edit-engine.h"
#include "flip-operation.h"

namespace Solang
{

FlipOperation::FlipOperation( const EditEnginePtr &engine,
                    const FlipOperation::Direction direction )
    :Filter( engine ),
    direction_( direction )
{
    std::string axis = ( HORIZONTAL == direction )
                        ? "y" : "x";
    GeglNodePtr pAction = gegl_node_new_child(
                            get_engine()->get_root_node(),
                            "operation", "gegl:reflect",
                            axis.c_str(), 1.0,
                            NULL);
    set_execution_node( pAction );
}

FlipOperation::~FlipOperation() throw()
{
}

Glib::ustring
FlipOperation::get_name() const throw()
{
    return _("Flip");
}

Glib::ustring
FlipOperation::get_description() const throw()
{
    return _("Flip image");
}


} //namespace Solang
