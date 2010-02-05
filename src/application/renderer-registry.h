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

#ifndef SOLANG_RENDERER_REGISTRY_H
#define SOLANG_RENDERER_REGISTRY_H

#include <sigc++/sigc++.h>

#include "i-renderer.h"
#include "renderer-selector.h"
#include "types.h"

namespace Solang
{

class Application;

class RendererRegistry
{
    public:
        RendererRegistry() throw();

        ~RendererRegistry() throw();

        void
        add(const IRendererPtr & renderer) throw();

        IRendererPtr
        get_current() throw();

        void
        remove(const IRendererPtr & renderer) throw();

        void
        set_current(const IRendererPtr & renderer) throw();

        template <typename T>
        IRendererPtr
        select() throw();

        sigc::signal<void, RendererRegistry &> &
        changed() throw();

    protected:

    private:
        IRendererList renderers_;

        IRendererPtr current_;

        sigc::signal<void, RendererRegistry &> changed_;
};

template <typename T>
IRendererPtr
RendererRegistry::select() throw()
{
    RendererSelector<T> selector;
    IRendererList::iterator it;

    for (it = renderers_.begin(); renderers_.end() != it; it++)
    {
        const IRendererPtr renderer
            = (*it)->receive_selector(selector, *it);

        if (0 != renderer)
        {
            return renderer;
        }
    }

    return IRendererPtr();
}

} // namespace Solang

#endif // SOLANG_RENDERER_REGISTRY_H
