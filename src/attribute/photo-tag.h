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

#ifndef SOLANG_PHOTO_TAG_H
#define SOLANG_PHOTO_TAG_H

#include <exception>

#include <glibmm.h>

#include "db-object.h"
#include "photo-search-criteria.h"

namespace Solang
{

//A photo - to - tag association

class PhotoTag :
    public DBObject
{
    private:
        static const gint32 PHOTOID_COL;
        static const gint32 TAGID_COL;

    private:
        gint32        photoId_;
        gint32        tagId_;

    public:
        PhotoTag();
        PhotoTag( gint32 photoId, gint32 tagId );
        ~PhotoTag() throw();

        inline gint32
        get_photoId_() const throw();

        void
        set_photoId_( gint32 photoId );

        inline gint32
        get_tagId_() const throw();

        void
        set_tagId_( gint32 tagId );

        virtual void
        insert(DataModelPtr & model, gint32 lastIndex) throw(Error);

        virtual void
        update(DataModelPtr & model, gint32 row) throw(Error);

        virtual void
        create(const DataModelPtr & dataModel, gint32 row)
               throw(Error);
        
        virtual Glib::ustring
        get_db_object_type_name() const throw();

        virtual DeleteActionPtr
        get_delete_action() throw();

        virtual Glib::ustring
        getQueryCriteria() const;
};

inline gint32
PhotoTag::get_photoId_() const throw()
{
    return photoId_;
}

inline gint32
PhotoTag::get_tagId_() const throw()
{
    return tagId_;
}

} // namespace Solang

#endif // SOLANG_PHOTO_TAG_H
