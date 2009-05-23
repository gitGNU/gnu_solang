/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * tag.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * tag.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * tag.h is distributed in the hope that it will be useful, but
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

class Tag :
    public DBObject,
    public PhotoSearchCriteria 
{
    private:
        static const gint32 TAGID_COL;
        static const gint32 NAME_COL;
        static const gint32 DESCRIPTION_COL;
        static const gint32 ICONPATH_COL;

    private:
        gint        tagId_;
        Glib::ustring  name_;
        Glib::ustring  description_;
        Glib::ustring  iconPath_;

    public:
        Tag() throw();

        Tag(gint tag_id, const Glib::ustring & name) throw();

        ~Tag() throw();

        inline gint
        get_tag_id() const throw();

        void
        set_tag_id(gint tag_id) throw();

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

        virtual void
        insert(DataModelPtr & model, gint32 lastIndex) throw(Error);

        virtual void
        update(DataModelPtr & model, gint32 row) throw(Error);

        virtual void
        create(DataModelPtr & dataModel, gint32 row) throw(Error);
        
        virtual Glib::ustring
        get_db_object_type_name() const throw();

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

inline gint
Tag::get_tag_id() const throw()
{
    return tagId_;
}

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
