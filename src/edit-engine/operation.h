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

#include "edit-types.h"
#include "error.h"
#include "non-copyable.h"

#ifndef SOLANG_OPERATION_H
#define SOLANG_OPERATION_H

namespace Solang
{

class Operation
        :public NonCopyable
{
    public:
        Operation(
            const EditEnginePtr &engine,
            const FilterPtr &filter,
            const BufferPtr &buffer );

        ~Operation() throw();

        BufferPtr
        apply( const ProgressObserverPtr &observer) throw(Error);

        BufferPtr
        revert( const ProgressObserverPtr &observer) throw(Error);

    private:
        EditEnginePtr engine_;
        FilterPtr filter_;
        BufferPtr original_;
};

} //namespace Solang
#endif //SOLANG_OPERATION_H
