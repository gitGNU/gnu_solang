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

#ifndef SOLANG_CONSOLE_RENDERER_H
#define SOLANG_CONSOLE_RENDERER_H

#include "i-renderer.h"
#include "types.h"

namespace Solang
{

class ConsoleRenderer :
    public IRenderer
{
    public:
        ConsoleRenderer() throw();

        virtual
        ~ConsoleRenderer() throw();

        virtual void
        init(Application & application) throw();

        virtual void
        render(const PhotoPtr & photo) throw();

        virtual void
        render(const PhotoList & photos) throw();

        virtual void
        final(Application & application) throw();

        virtual PhotoList
        get_current_selection() throw()
        {
            return PhotoList();
        }

        virtual std::string
        get_name() const throw();

        virtual void
        receive_plugin(IPlugin & plugin) throw();

    protected:

    private:
};

} //namespace Solang

#endif // SOLANG_CONSOLE_RENDERER_H
