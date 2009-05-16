/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
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

#include "config.h"

#include <iostream>

#include "console-renderer.h"
#include "photo.h"

namespace Solang
{

ConsoleRenderer::ConsoleRenderer() throw() :
    Renderer()
{
}

ConsoleRenderer::ConsoleRenderer(const RendererPtr & next)
    throw() :
    Renderer(next)
{
}

ConsoleRenderer::~ConsoleRenderer() throw()
{
}

void
ConsoleRenderer::init(Application & application) throw()
{
}

void
ConsoleRenderer::render(const PhotoPtr & photo) throw()
{
    std::cout<<"Photo Path:"<<photo->get_disk_file_path()<<std::endl;
    std::cout<<"Photo Path:"<<photo->get_uri()<<std::endl;
	if( next_ )
		next_->render( photo );
}

void
ConsoleRenderer::render(const PhotoList & photos) throw()
{
    for( PhotoList::const_iterator photo = photos.begin();
                        photo != photos.end(); photo++ )
    {
        render( *photo );
    }
}

void
ConsoleRenderer::final(Application & application) throw()
{
}

} // namespace Solang
