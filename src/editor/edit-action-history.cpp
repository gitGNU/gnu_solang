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

#ifdef HAVE_CONFIG_H
#include "edit-action-history.h"
#endif

#include "edit-action.h"
#include "edit-action-history.h"

namespace Solang
{

EditActionHistory::EditActionHistory()
    :actions_(),
    numActions_( 0 ),
    currentActionIndex_( 0 )
{
}

EditActionHistory::EditActionHistory( const EditActionHistory &rhs )
    :actions_( rhs.actions_ ),
    numActions_( rhs.numActions_ ),
    currentActionIndex_( rhs.currentActionIndex_ )
{
}

EditActionHistory::~EditActionHistory()
{
    clear();
}

EditActionHistory &
EditActionHistory::operator =( const EditActionHistory & rhs )
{
    if( this != &rhs )
    {
        actions_ = rhs.actions_;
        numActions_ = rhs.numActions_;
        currentActionIndex_ = rhs.currentActionIndex_;
    }
    return *this;
}

void
EditActionHistory::insert( const EditActionPtr &action )
{
    //if( numActions_ > 0 )
    {
        //If action is being taken after some undo
        //discard the changes
        if( numActions_ > currentActionIndex_ )
        {
            numActions_ = currentActionIndex_;
            actions_.erase( actions_.begin() + currentActionIndex_,
                            actions_.end() );
        }
        //else
        {
            //Otherwise, just add normally
            currentActionIndex_++;
        }
    }
    numActions_++;
    actions_.push_back( action );
}

void
EditActionHistory::clear()
{
    actions_.clear();
    numActions_ = 0;
    currentActionIndex_ = 0;
}

EditActionList
EditActionHistory::undo_actions( size_type steps )
{
    EditActionList actions;
    if( currentActionIndex_ >= steps )
    {
        size_type startIndex = currentActionIndex_ - 1;
        currentActionIndex_ -= (currentActionIndex_ >=steps)
                                ? steps : 0;
        for( size_type index = startIndex;
                    index >= currentActionIndex_; index-- )
        {
            actions.push_back( actions_[ index ] );
            if( 0 == index )
                break;
        }
    }
    return actions;
}

EditActionList
EditActionHistory::redo_actions( size_type steps )
{
    if( numActions_ >=( currentActionIndex_ + steps ) )
    {
        size_type statIndex = currentActionIndex_;
        currentActionIndex_ += steps;
        return EditActionList( actions_.begin() + statIndex,
                            actions_.begin() + currentActionIndex_);
    }
    return EditActionList();
}

EditActionList
EditActionHistory::get_actions_for_copy() const throw()
{
    EditActionList actions( actions_.begin(), actions_.end() );
    return actions;
}

} //namespace Solang
