/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * directory-storage.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * directory-storage.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * directory-storage.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_DIRECTORY_STORAGE_H
#define SOLANG_DIRECTORY_STORAGE_H

#include <glibmm.h>

#include "storage.h"
#include "types.h"

namespace Solang
{

class Photo;
//class PhotoList;

class DirectoryStorage :
    public Storage 
{
    private:
            Glib::ustring path_;
            Glib::ustring formatString_;
            bool doNotCopy_;

    public:
            DirectoryStorage(
                        ThumbnailStore &store,
                        const DatabasePtr &db,
                        const Glib::ustring &path);

            virtual ~DirectoryStorage() throw();

            virtual void init(Application & application) throw(Error);

            virtual void save(const PhotoPtr &) throw(Error);
            virtual void save(const PhotoPtr &, bool) throw(Error);

            //The following should return the path of the file
            // in the local disk
            // If required it will download from the storage
            // For raw files, the thumbnail will be extracted
            // and path for the same will be returned
            virtual Glib::ustring retrieve(const Photo &) throw(Error);

            virtual void downloadToDisk(
                        const PhotoList &,
                        ProgressObserverPtr &observer) throw(Error);

            virtual void remove(const PhotoPtr &photo) throw(Error);

            virtual void remove(const PhotoList &photos) throw(Error);

            virtual std::string get_storage_uri_prefix() throw();

            virtual void final(Application & application) throw(Error);

            void
            set_target_path_format(const Glib::ustring &) throw();

            Glib::ustring
            get_target_path_format() const throw();

            void
            set_do_not_copy( bool ) throw();

    protected:
};

} // namespace Solang

#endif // SOLANG_DIRECTORY_STORAGE_H
