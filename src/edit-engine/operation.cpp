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
#include <glibmm/i18n.h>
#include <iostream>

#include "buffer.h"
#include "edit-engine.h"
#include "filter.h"
#include "operation.h"
#include "progress-observer.h"

namespace Solang
{
Operation::Operation(
            const EditEnginePtr &engine,
            const FilterPtr &filter,
            const BufferPtr &buffer )
    :engine_( engine ),
    filter_( filter ),
    original_( buffer )
{
}

Operation::~Operation() throw()
{
}

BufferPtr
Operation::apply(
            const ProgressObserverPtr &observer) throw(Error)
{
    GeglBufferPtr pBuf = gegl_buffer_dup(
                                original_->get_buffer() );
    GeglNodePtr pRoot = engine_->get_root_node();
    if( !pRoot )
    {
        pRoot = gegl_node_new();
        engine_->set_root_node( pRoot );
    }
    GeglNodePtr pInp = gegl_node_new_child( pRoot,
                        "operation", "gegl:load-buffer",
                        "buffer", pBuf,
                        NULL);
    GeglRectangle rect = gegl_node_get_bounding_box( pInp );
    GeglNodePtr pAction = filter_->get_execution_node();
    GeglBufferPtr pOutBuf = NULL;
    GeglNodePtr pOut = gegl_node_new_child( pRoot,
                        "operation", "gegl:save-buffer",
                        "buffer", &pOutBuf,
                        NULL );
    gegl_node_link_many( pInp, pAction, pOut, NULL );

    //Now process
    GeglProcessor *processor
                = gegl_node_new_processor (pOut, NULL);
    double progress = 0.0;
    observer->set_num_events( 100 );
    observer->set_event_description( _("Running Filter") );

    while ( gegl_processor_work (processor, &progress) )
    {
        observer->set_current_events( progress * 100.0 );
        observer->progress().emit();
        std::cout<<"Done:"<<progress<<std::endl;
    }

    gegl_processor_destroy (processor);
#if 0
    gegl_node_process( pOut );
#endif
    g_object_unref( pRoot );
    engine_->set_root_node( NULL );
    //original_.reset();

    return BufferPtr( new Buffer( pOutBuf ) );

}

BufferPtr
Operation::revert(
        const ProgressObserverPtr &observer) throw(Error)
{
    return original_;
}

} //namespace Solang
