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

#ifndef SOLANG_DELETE_ACTION_H
#define SOLANG_DELETE_ACTION_H

#include <vector>

#include <glibmm.h>

#include "types.h"

namespace Solang
{

class DeleteAction
{
    public:

        DeleteAction( const Glib::ustring &name,
                        const DBObjectPtr &object ) throw();
        DeleteAction( const DeleteAction &rhs ) throw();
        virtual ~DeleteAction() throw();

        DeleteAction &
        operator = ( const DeleteAction &rhs ) throw();

        void
        add_command( const Glib::ustring &sql ) throw();

        inline const Glib::ustring &
        get_command_name() const throw();

        void
        execute( Database &db ) throw();

    private:
        Glib::ustring commandName_;
        std::vector<Glib::ustring> commands_;
        DBObjectPtr object_;

};

inline const Glib::ustring &
DeleteAction::get_command_name() const throw()
{
    return commandName_;
}

} // namespace Solang

#endif //SOLANG_DELETE_ACTION_H
