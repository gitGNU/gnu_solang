/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
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

#include "database.h"
#include "delete-action.h"

namespace Solang
{

DeleteAction::DeleteAction( const Glib::ustring &name,
                        const DBObjectPtr &object ) throw()
    :commandName_( name ),
    commands_(),
    object_( object )
{
}

DeleteAction::DeleteAction( const DeleteAction &rhs ) throw()
    : commandName_( rhs.commandName_ ),
    commands_( rhs.commands_ ),
    object_( rhs.object_ )
{
}

DeleteAction::~DeleteAction() throw()
{
}

DeleteAction &
DeleteAction::operator = ( const DeleteAction &rhs ) throw()
{
    if( this != &rhs )
    {
        commandName_ = rhs.commandName_;
        commands_ = rhs.commands_;
        object_ = rhs .object_;
    }
    return *this;
}

void
DeleteAction::add_command( const Glib::ustring &sql ) throw()
{
    commands_.push_back( sql );
    return;
}

void
DeleteAction::execute( Database &db ) throw()
{
    if( object_ && !object_->get_is_deleted() )
    {
        //This object has been undeleted
        return;
    }
    for( std::vector<Glib::ustring>::iterator sql = commands_.begin();
                                sql != commands_.end(); sql++ )
    {
        db.run_sql( *sql );
    }

    if( object_ )
        object_->remove_physical_existence();

    return;
}

} //namespace Solang
