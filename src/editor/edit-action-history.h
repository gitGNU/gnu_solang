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

#ifndef SOLANG_EDIT_ACTION_HISTORY_H
#define SOLANG_EDIT_ACTION_HISTORY_H

#include "types.h"

namespace Solang
{

class EditActionHistory
{
    public:
        typedef EditActionList::size_type size_type;
    public:
        EditActionHistory();
        EditActionHistory( const EditActionHistory &rhs );
        ~EditActionHistory();

        EditActionHistory &
        operator = ( const EditActionHistory &rhs );

        void
        insert( const EditActionPtr &action );

        void
        clear();

        EditActionList
        undo_actions( size_type steps );

        EditActionList
        redo_actions( size_type steps );

        EditActionList
        get_actions_for_copy() const throw();

        inline const EditActionList &
        get_actions() const throw();

        inline bool
        is_undo_possible() const throw();

        inline bool
        is_redo_possible() const throw();

    private:
        EditActionList actions_;
        size_type numActions_;
        size_type currentActionIndex_;
};

inline const EditActionList &
EditActionHistory::get_actions() const throw()
{
    return actions_;
}

inline bool
EditActionHistory::is_undo_possible() const throw()
{
    return currentActionIndex_ != 0;
}

inline bool
EditActionHistory::is_redo_possible() const throw()
{
    return numActions_ > currentActionIndex_;
}

} // namespace Solang

#endif //SOLANG_EDIT_ACTION_HISTORY_H
