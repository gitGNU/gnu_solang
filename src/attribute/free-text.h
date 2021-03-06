/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2010 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_FREE_TEXT_H
#define SOLANG_FREE_TEXT_H

#include <glibmm.h>

#include "i-photo-search-criteria.h"

namespace Solang
{

class FreeText :
    public IPhotoSearchCriteria
{
    private:
        Glib::ustring text_;

    public:
        FreeText(const Glib::ustring & text) throw();

        virtual
        ~FreeText() throw();

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

        const Glib::ustring &
        get_text() const throw();
};

} // namespace Solang

#endif // SOLANG_FREE_TEXT_H
