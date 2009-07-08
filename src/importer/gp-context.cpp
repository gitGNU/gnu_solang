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
#endif // HAVE_CONFIG_H

#include "gp-context.h"

namespace Solang
{

GPhotoContext::GPhotoContext()
    :camera_( NULL ),
    context_( NULL ),
    portinfoList_( NULL ),
    abilitiesList_( NULL ),
    cameras_( NULL )
{
}

GPhotoContext::~GPhotoContext()
{
    cleanup();
}

void
GPhotoContext::cleanup()
{
    if( camera_ )
    {
        gp_camera_free( camera_ );
        camera_ = NULL;
    }
    if( context_ )
    {
        gp_context_unref( context_ );
        context_ = NULL;
    }
    if( portinfoList_ )
    {
        gp_port_info_list_free( portinfoList_ );
        portinfoList_ = NULL;
    }
    if( abilitiesList_ )
    {
        gp_abilities_list_free( abilitiesList_ );
        abilitiesList_ = NULL;
    }
    if( cameras_ )
    {
        gp_list_free( cameras_ );
        cameras_ = NULL;
    }
}

} // namespace Solang
