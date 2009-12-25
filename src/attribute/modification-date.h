/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_MODIFICATION_DATE_H
#define SOLANG_MODIFICATION_DATE_H

#include "photo-search-criteria.h"

namespace Solang
{

class ModificationDate
    : public PhotoSearchCriteria
{
    private:
        gint32 day_;
        gint32 month_;
        gint32 year_;    

    public:
        ModificationDate( gint32 day = 0, 
                        gint32 month = 0, 
                        gint32 year = 0)
            :day_( day ),
            month_( month ),
            year_( year )
        {
        }

        ModificationDate( const ModificationDate &rhs )
            :day_( rhs.day_ ),
            month_( rhs.month_ ),
            year_( rhs.year_ )
        {
        }

        ~ModificationDate() throw()
        {
        }

        ModificationDate &
        operator =( const ModificationDate &rhs ) throw()
        {
            if ( this != &rhs )
            {
                day_ = rhs.day_ ;
                month_ = rhs.month_ ;
                year_ = rhs.year_ ;
            }
            return *this;
        }

        bool
        operator < ( const ModificationDate &rhs ) const throw()
        {
            if( year_ < rhs.year_ )
            {
                return true;
            }
            if( year_ == rhs.year_ )
            {
                if( month_ < rhs.month_ )
                {
                    return true;
                }        
                if( month_ == rhs.month_ )
                {
                    if( day_ < rhs.day_ )
                    {
                        return true;
                    }    
                }    
            
            }
            return false;
        }
        
        bool
        operator == ( const ModificationDate &rhs ) const throw()
        {
            return day_ == rhs.day_
                    && month_ == rhs.month_
                    && year_ == rhs.year_;
        }    

        bool
        operator > ( const ModificationDate &rhs ) const throw()
        {
            return !operator == ( rhs )
                        && !operator < ( rhs );
        }    

        gint32
        get_day() const throw()
        {
            return day_;
        }

        void
        set_day( gint32 day )
        {
            day_ = day;
        }

        gint32
        get_month() const throw()
        {
            return month_;
        }

        void
        set_month( gint32 month )
        {
            month_ = month;
        }

        gint32
        get_year() const throw()
        {
            return year_;
        }

        void
        set_year( gint32 year )
        {
            year_ = year;
        }

        virtual Glib::ustring
        get_query_criteria() const throw();

        virtual ClubbingOperationType
        get_clubbing_type() const throw();

        virtual gint32
        get_id() const throw();

        virtual Glib::ustring
        get_criteria_description() const throw();

        virtual Glib::ustring
        get_criteria_icon_path() const throw();
};    

} // namespace Solang

#endif // SOLANG_MODIFICATION_DATE_H
