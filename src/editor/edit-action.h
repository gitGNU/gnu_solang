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

#ifndef SOLANG_I_EDIT_ACTION_H
#define SOLANG_I_EDIT_ACTION_H

#include "error.h"
#include "non-copyable.h"
#include "types.h"

namespace Solang
{

class EditAction :
        public NonCopyable
{
    protected:
        EditAction();

    public:
        virtual ~EditAction() throw();

        virtual void execute(
                    EditablePhoto &photo) throw(Error) = 0;

        virtual void reverse(
                    EditablePhoto &photo) throw(Error) = 0;

        virtual EditActionPtr clone() = 0;
};

} //namespace Solang

#endif // SOLANG_I_EDIT_ACTION_H
