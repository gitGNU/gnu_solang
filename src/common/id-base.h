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

#ifndef SOLANG_ID_BASE_H
#define SOLANG_ID_BASE_H

#include <glibmm.h>

#include "id-repo.h"

namespace Solang
{

template <typename T >
class IDBase
{
    private:
        static const gint32 id_;

    public:
        static inline gint32
        get_id();

};

template <typename T>
inline gint32
IDBase<T>::get_id()
{
    return IDBase<T>::id_;
}

template <typename T>
const gint32 IDBase<T>::id_ = IdRepo::getValue();

} // namespace Solang

#endif //SOLANG_ID_BASE_H
