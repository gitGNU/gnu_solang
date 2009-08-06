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

#include <iostream>

#include "console-renderer.h"
#include "i-plugin.h"
#include "i-renderer-selector.h"
#include "photo.h"

namespace Solang
{

ConsoleRenderer::ConsoleRenderer() throw() :
    IRenderer()
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
    std::cout << "Photo Path:" << photo->get_disk_file_path()
              << std::endl;
    std::cout << "Photo Path:" << photo->get_uri() << std::endl;
}

void
ConsoleRenderer::render(const PhotoList & photos) throw()
{
    for (PhotoList::const_iterator photo = photos.begin();
         photo != photos.end(); photo++)
    {
        render(*photo);
    }
}

void
ConsoleRenderer::final(Application & application) throw()
{
}

void
ConsoleRenderer::present() throw()
{
}

void
ConsoleRenderer::receive_plugin(IPlugin & plugin) throw()
{
    plugin.visit_renderer(*this);
}

IRendererPtr
ConsoleRenderer::receive_selector(IRendererSelector & selector,
                                  const IRendererPtr & renderer)
                                  throw()
{
    return selector.select(*this, renderer);
}

} // namespace Solang
