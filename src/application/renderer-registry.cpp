/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009, 2010 Debarshi Ray <rishi@gnu.org>
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

#include <algorithm>

#include "application.h"
#include "renderer-registry.h"

namespace Solang
{

RendererRegistry::RendererRegistry() throw() :
    renderers_(),
    current_(),
    changed_()
{
}

RendererRegistry::~RendererRegistry() throw()
{
}

void
RendererRegistry::add(const IRendererPtr & renderer) throw()
{
    renderers_.push_back(renderer);
}

IRendererPtr
RendererRegistry::get_current() throw()
{
    return current_;
}

void
RendererRegistry::remove(const IRendererPtr & renderer) throw()
{
    const IRendererList::iterator iter = std::find(renderers_.begin(),
                                                   renderers_.end(),
                                                   renderer);
    if (renderers_.end() != iter)
    {
        renderers_.erase(iter);
    }
}

void
RendererRegistry::set_current(const IRendererPtr & renderer) throw()
{
    current_ = renderer;
    changed_.emit(*this);
}

sigc::signal<void, RendererRegistry &> &
RendererRegistry::changed() throw()
{
    return changed_;
}

} // namespace Solang
