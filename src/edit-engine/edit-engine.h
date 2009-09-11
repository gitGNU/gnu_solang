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

#ifndef SOLANG_EDIT_ENGINE_H
#define SOLANG_EDIT_ENGINE_H

#include "error.h"
#include "edit-types.h"
#include "non-copyable.h"
#include "types.h"

namespace Solang
{

class EditEngine
        : public NonCopyable
{
    public:
        EditEngine();
        ~EditEngine() throw();

        void
        init(const ProgressObserverPtr &observer) throw();

        BufferPtr
        apply( OperationPtr &op ) throw(Error);

        BufferPtr
        revert( OperationPtr &op ) throw(Error);

        void
        final() throw();

        inline GeglNodePtr
        get_root_node() throw();

        void
        set_root_node( GeglNodePtr root ) throw();

        inline ProgressObserverPtr
        get_observer() throw();

    private:
        ProgressObserverPtr observer_;
        GeglNodePtr rootNode_;
};

inline ProgressObserverPtr
EditEngine::get_observer() throw()
{
    return observer_;
}

inline GeglNodePtr
EditEngine::get_root_node() throw()
{
    return rootNode_;
}

} //namespace Solang

#endif //SOLANG_EDIT_ENGINE_H
