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

#ifndef SOLANG_FILTER_H
#define SOLANG_FILTER_H

#include <glibmm.h>

#include "non-copyable.h"
#include "edit-types.h"

namespace Solang
{

class Filter
    : public NonCopyable
{
    protected:
        Filter( const EditEnginePtr &engine );
        Filter( const EditEnginePtr &engine,
                const Glib::ustring &xmlPath );

    public:
        virtual ~Filter() throw();

        virtual void
        dumpXml( const Glib::ustring &path );

        inline GeglNodePtr
        get_execution_node();

        inline EditEnginePtr
        get_engine();

        void
        set_execution_node( GeglNodePtr node );

        virtual Glib::ustring
        get_name() const throw() = 0;

        virtual Glib::ustring
        get_description() const throw() = 0;

    private:
        GeglNodePtr node_;
        EditEnginePtr engine_;
};

inline GeglNodePtr
Filter::get_execution_node()
{
    return node_;
}
inline EditEnginePtr
Filter::get_engine()
{
    return engine_;
}

} // namespace Solang
#endif //SOLANG_FILTER_H
