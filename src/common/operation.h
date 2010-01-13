/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2010 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_OPERATION_H
#define SOLANG_OPERATION_H

#include "i-operation.h"

namespace Solang
{

class Operation :
    public IOperation
{
    public:
        virtual BufferPtr
        apply(const BufferPtr & buffer,
              const ProgressObserverPtr & observer) throw();

    protected:
        virtual
        ~Operation() throw() = 0;

        Operation() throw();
};

} // namespace Solang

#endif // SOLANG_OPERATION_H
