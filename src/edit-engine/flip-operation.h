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

#include "filter.h"

#ifndef SOLANG_FLIP_OPERATION_H
#define SOLANG_FLIP_OPERATION_H

namespace Solang
{

class FlipOperation
        : public Filter
{
    public:
        enum Direction
        {
            HORIZONTAL,
            VERTICAL
        };

    public:
        FlipOperation( const EditEnginePtr &engine,
                  const Direction direction);

        FlipOperation( const EditEnginePtr &engine,
                  const Glib::ustring &xmlPath );

        ~FlipOperation() throw();

        virtual Glib::ustring
        get_name() const throw();

        virtual Glib::ustring
        get_description() const throw();

    private:
       Direction direction_;
};


}//namespace Solang

#endif //SOLANG_FLIP_OPERATION_H
