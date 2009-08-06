/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_RENDERER_SELECTOR_H
#define SOLANG_RENDERER_SELECTOR_H

#include "i-renderer-selector.h"

namespace Solang
{

template <typename T>
class RendererSelector :
    public IRendererSelector
{
    public:
        RendererSelector() throw();

        virtual
        ~RendererSelector() throw();

        virtual IRendererPtr
        select(T & t_renderer, const IRendererPtr & renderer) throw();
};

template <typename T>
RendererSelector<T>::RendererSelector() throw() :
    IRendererSelector()
{
}

template <typename T>
RendererSelector<T>::~RendererSelector() throw()
{
}

template <typename T>
IRendererPtr
RendererSelector<T>::select(T & t_renderer,
                            const IRendererPtr & renderer) throw()
{
    return renderer;
}

} // namespace Solang

#endif // SOLANG_RENDERER_SELECTOR_H
