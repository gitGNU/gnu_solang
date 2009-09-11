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
#include "contrast-operation.h"

namespace Solang
{

ContrastOperation::ContrastOperation( const EditEnginePtr &engine,
                    double contrast )
    :Filter( engine ),
    contrast_( contrast )
{
    GeglNodePtr pAction = gegl_node_new_child(
                            get_engine()->get_root_node(),
                            "operation", "gegl:brightness-contrast",
							"brightness", 0.0,
                            "contrast", 1.0 + contrast,
                            NULL);
    set_execution_node( pAction );
}

ContrastOperation::~ContrastOperation() throw()
{
}

Glib::ustring
ContrastOperation::get_name() const throw()
{
    return _("Contrast");
}

Glib::ustring
ContrastOperation::get_description() const throw()
{
    return _("Change image contrast");
}


} //namespace Solang
