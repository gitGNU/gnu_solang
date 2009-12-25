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

#ifndef SOLANG_EXIF_DATA_KEY_H
#define SOLANG_EXIF_DATA_KEY_H

#include "photo-search-criteria.h"

namespace Solang
{

class ExifDataKey :
    public PhotoSearchCriteria
{
    private:
        Glib::ustring title_;
        Glib::ustring key_;
        Glib::ustring value_;

    public:
        ExifDataKey( const Glib::ustring &title,
                     const Glib::ustring & key,
                     const Glib::ustring & value )
            :title_( title ),
            key_( key ),
            value_( value )
        {
        }

        ~ExifDataKey() throw()
        {
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

#endif // SOLANG_EXIF_DATA_KEY_H
