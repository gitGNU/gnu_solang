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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <algorithm>
#include <functional>

#include "application.h"
#include "renderer-registry.h"

namespace Solang
{

template<typename T>
class Initializer :
    public std::unary_function<T &, void>
{
    public:
        Initializer(Application * const & application) throw();

        Initializer(const Initializer<T> & source) throw();
    
        ~Initializer() throw();

        Initializer<T> &
        operator=(const Initializer<T> & source) throw();

        void
        operator()(T & x) throw();

    protected:
        Application * application_;
};

template<typename T>
Initializer<T>::Initializer(Application * const & application) throw() :
    std::unary_function<T &, void>(),
    application_(application)
{
}

template<typename T>
Initializer<T>::Initializer(const Initializer<T> & source) throw() :
    std::unary_function<T &, void>(source),
    application_(source.application_)
{
}

template<typename T>
Initializer<T>::~Initializer() throw()
{
}

template<typename T>
Initializer<T> &
Initializer<T>::operator=(const Initializer<T> & source) throw()
{
    if (this != &source)
    {
        std::unary_function<T &, void>::operator=(source);
        application_ = source.application_;
    }

    return *this;
}

template<typename T>
void
Initializer<T>::operator()(T & x) throw()
{
    x->init(*application_);
}

template<typename T>
class Finalizer :
    public std::unary_function<T &, void>
{
    public:
        Finalizer(Application * const & application) throw();

        Finalizer(const Finalizer<T> & source) throw();
    
        ~Finalizer() throw();

        Finalizer<T> &
        operator=(const Finalizer<T> & source) throw();

        void
        operator()(T & x) throw();

    protected:
        Application * application_;
};

template<typename T>
Finalizer<T>::Finalizer(Application * const & application) throw() :
    std::unary_function<T &, void>(),
    application_(application)
{
}

template<typename T>
Finalizer<T>::Finalizer(const Finalizer<T> & source) throw() :
    std::unary_function<T &, void>(source),
    application_(source.application_)
{
}

template<typename T>
Finalizer<T>::~Finalizer() throw()
{
}

template<typename T>
Finalizer<T> &
Finalizer<T>::operator=(const Finalizer<T> & source) throw()
{
    if (this != &source)
    {
        std::unary_function<T &, void>::operator=(source);
        application_ = source.application_;
    }

    return *this;
}

template<typename T>
void
Finalizer<T>::operator()(T & x) throw()
{
    x->final(*application_);
}

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
RendererRegistry::init(Application & application) throw()
{
    std::for_each(renderers_.begin(), renderers_.end(),
                  Initializer<IRendererPtr>(&application));
}

void
RendererRegistry::final(Application & application) throw()
{
    std::for_each(renderers_.rbegin(), renderers_.rend(),
                  Finalizer<IRendererPtr>(&application));
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
