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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H

#include <gegl.h>

#include "edit-engine.h"
#include "operation.h"

namespace Solang
{

EditEngine::EditEngine()
    :observer_(),
    rootNode_( NULL )
{
}

EditEngine::~EditEngine() throw()
{
    if( NULL != rootNode_ )
    {
        final();
    }
}

void
EditEngine::init( const ProgressObserverPtr &observer ) throw()
{
    observer_ = observer;
    //rootNode_ = gegl_node_new();
}

BufferPtr
EditEngine::apply( const OperationPtr &op,
                const BufferPtr &buffer ) throw(Error)
try
{
    return op->apply( observer_, buffer );
}
catch(Error &e)
{
    e.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

BufferPtr
EditEngine::revert( OperationPtr &op ) throw(Error)
try
{
    return op->revert( observer_ );
}
catch(Error &e)
{
    e.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

void
EditEngine::final() throw()
{
    g_object_unref( rootNode_ );
    rootNode_ = NULL;
}

void
EditEngine::set_root_node( GeglNodePtr root ) throw()
{
    rootNode_ = root;
}

} //namespace Solang
