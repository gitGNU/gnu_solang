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
#include <fstream>
#include <gegl.h>

#include "edit-engine.h"
#include "filter.h"

namespace Solang
{
Filter::Filter( const EditEnginePtr &engine )
    :node_( NULL ),
    engine_( engine )
{
}

Filter::Filter( const EditEnginePtr &engine,
                const Glib::ustring &xmlPath )
    :node_( NULL ),
    engine_( NULL )
{
    node_ = gegl_node_new_from_file( xmlPath.c_str() );
}

Filter::~Filter() throw()
{
    //Auto deleted by reference counting
    g_object_unref( node_ );
}

void
Filter::dumpXml( const Glib::ustring &path )
{
    std::ofstream fout( path.c_str() );
    GeglNode *pInput = gegl_node_get_input_proxy(
                        engine_->get_root_node(),
                        "proxy-input");
    gegl_node_connect_to( pInput, "proc", node_, "inp");
    GeglNode *pOutput = gegl_node_get_output_proxy(
                        engine_->get_root_node(),
                        "proxy-output");
    gegl_node_connect_to( node_, "outp", pOutput, "inp-proc");
    //gegl_node_link_many( pInput, node_, pOutput, NULL );
    char buf[ BUFSIZ ];
    gegl_node_to_xml( pOutput, buf );
    fout<<buf;
    fout.close();
    gegl_node_disconnect( node_, "inp" );
    gegl_node_disconnect( pOutput, "inp-proc" );
    g_object_unref( pInput );
    g_object_unref( pOutput );
    return;
}

void
Filter::set_execution_node( GeglNodePtr node )
{
    node_ = node;
}

} // namespace Solang
