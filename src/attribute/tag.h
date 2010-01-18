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

#ifndef SOLANG_TAG_H
#define SOLANG_TAG_H

#include <exception>

#include <glibmm.h>

#include "db-object.h"
#include "photo-search-criteria.h"
#include "types.h"

namespace Solang
{

class Database;

class Tag :
    public DBObject,
    public PhotoSearchCriteria 
{
    private:
        Glib::ustring name_;
        Glib::ustring  description_;
        Glib::ustring  iconPath_;
        std::string urn_;

    public:
        Tag(const Glib::ustring & name,
            const Glib::ustring & description,
            const std::string & urn = std::string())
            throw();

        virtual
        ~Tag() throw();

        virtual void
        delete_async(Database & database, const SlotAsyncReady & slot)
                     const throw();

        virtual Glib::ustring
        get_delete_query() const throw();

        virtual Glib::ustring
        get_save_query() const throw();

        virtual void
        save_async(Database & database, const SlotAsyncReady & slot)
                   const throw();

        void
        edit_async(const Glib::ustring & name,
                   const Glib::ustring & description,
                   Database & database,
                   const SlotAsyncReady & slot) throw();

        Glib::ustring
        get_edit_query(const Glib::ustring & name,
                       const Glib::ustring & description) const
                       throw();

        inline const Glib::ustring &
        get_name() const throw();

        void
        set_name(const Glib::ustring & name) throw();

        inline const Glib::ustring &
        get_description() const throw();

        void
        set_description(const Glib::ustring & description) throw();

        inline const Glib::ustring &
        get_icon_path() const;

        void
        set_icon_path(const Glib::ustring & icon_path);

        virtual Glib::ustring
        get_query_criteria() const throw();

        virtual DeleteActionPtr
        get_delete_action() throw();

        virtual ClubbingOperationType
        get_clubbing_type() const throw();

        virtual gint32
        get_id() const throw();

        virtual Glib::ustring
        get_criteria_description() const throw();

        virtual Glib::ustring
        get_criteria_icon_path() const throw();

        const std::string &
        get_urn() const throw();
};

inline const Glib::ustring &
Tag::get_name() const throw()
{
    return name_;
}

inline const Glib::ustring &
Tag::get_description() const throw()
{
    return description_;
}

inline const Glib::ustring &
Tag::get_icon_path() const
{
    return iconPath_;
}

} // namespace Solang

#endif // SOLANG_TAG_H
