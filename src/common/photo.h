/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * photo.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * photo.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * photo.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_PHOTO_H
#define SOLANG_PHOTO_H

#include <tr1/memory>

#include <glibmm.h>

#include "db-object.h"
#include "exif-data.h"
#include "modification-date.h"
#include "thumbnail.h"
#include "types.h"

namespace Solang
{

class Storage;

class Photo :
    public DBObject
{
    public:
        Photo() throw();
        ~Photo() throw();

        inline gint64 get_photo_id() const throw();
        void set_photoId_( gint64 photoId ) throw();

        inline const Glib::ustring &get_uri() const throw();
        void set_uri( const Glib::ustring &uri );

        inline const Glib::ustring 
        & get_disk_file_path() const throw();

        inline const ModificationDate &
        get_modification_date() const throw();

        void
        set_modification_date( const ModificationDate &modDate );
#if 0   
        inline const Glib::ustring &
        get_thumbnail_path() const;

        void
        set_thumbnail_path(const Glib::ustring & path);
            
        inline const Resolution &
        get_thumbnail_resolution() const throw();

        void
        set_thumbnail_resolution(const Resolution & resolution);
#endif
        inline const Thumbnail &
        get_thumbnail() const throw();

        void
        set_thumbnail( const Thumbnail &thumb ) throw();

        inline const ExifData &
        get_exif_data() const throw();

        void 
        set_exif_data( const ExifData &exif ) throw();
#if 0
        void
        generate_thumbnail(const Resolution & new_resolution,
                 const DatabasePtr & db) throw(Error);

        void
        generate_thumbnail() throw(Error);

        void
        generate_thumbnail(
                        const Exiv2::ExifData & exifData) throw(Error);
#endif
        void   set_disk_file_path(const IStoragePtr & storage);
        void   set_disk_file_path(const Glib::ustring &disk_file_path);
        

        //Overrides from DBObject
//        virtual Glib::ustring getCreateSQL() throw(Error);
        virtual void insert(
                DataModelPtr &model, gint32 lastIndex) throw(Error);
        virtual void update(
                DataModelPtr &model, gint32 row) throw(Error);

//        void create(Database &db, gint32 row) throw(Error);

        virtual void create( 
                DataModelPtr& dataModel, gint32 row) throw(Error);
        
        virtual Glib::ustring
        get_db_object_type_name() const throw();

    private:
        void
        make_thumb_path() throw(Error);

    private:
        static const gint32 PHOTOID_COL;
        static const gint32 URI_COL;

        gint64 photoId_;
        Glib::ustring uri_; //storage uri
        Glib::ustring diskFilePath_;    

        //Date
        ModificationDate modDate_;

        //Thumbnail
        Thumbnail thumbnail_;
        //Exif
        ExifData exifData_;


};

inline gint64 
Photo::get_photo_id() const throw()
{
    return photoId_;
}

inline const Glib::ustring &
Photo::get_uri() const throw()
{
    return uri_;
}
        
inline const Thumbnail &
Photo::get_thumbnail() const throw()
{
    return thumbnail_;
}

inline const ExifData &
Photo::get_exif_data() const throw()
{
    return exifData_;
}

inline const Glib::ustring &
Photo::get_disk_file_path() const throw()
{
    return diskFilePath_;
}

inline const ModificationDate &
Photo::get_modification_date() const throw()
{
    return modDate_;
}

} // namespace Solang

#endif // SOLANG_PHOTO_H
