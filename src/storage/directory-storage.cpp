/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * directory-storage.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * directory-storage.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * directory-storage.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <iostream>
#include <cstdio>
#include <giomm.h>
#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "directory-storage.h"
#include "exif-data.h"


namespace Solang
{

DirectoryStorage::DirectoryStorage(
                    ThumbnailStore &store, 
                    const DatabasePtr &db,
                    const Glib::ustring &path)
    :Storage( store, db ),
    path_( path )
{
}

DirectoryStorage::~DirectoryStorage() throw()
{
}

void DirectoryStorage::init(Application & application) throw(Error)
{
}

void DirectoryStorage::save(const PhotoPtr &photo) throw(Error)
{
    //Steps:
    // If exif is present
    //Read date and time 
    //else use modification date
    //Create directory and copy file
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(
                                photo->get_disk_file_path().c_str());
    if(image.get() != 0);
    image->readMetadata();

    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        //No exif data 
    }

    Exiv2::ExifKey key("Exif.Photo.DateTimeOriginal");
    Exiv2::ExifData::iterator dateTime = exifData.findKey( key );
    guint32 year=0;
    guint32 month=0;
    guint32 day=0;
    if( dateTime != exifData.end() )
    {
        std::cout<<(*dateTime).toString()<<std::endl;
        Glib::ustring date = (*dateTime).toString();
        //TBD CHECK
        sscanf(date.c_str(), "%u:%u:%u", &year, &month, &day );
    }    
    else
    {
        //Use stat
        struct stat info;
        if( -1 == stat( photo->get_disk_file_path().c_str(), &info ) )
        {
            //TBD::Error
        }
        
        struct tm timeInfo;
        if( NULL == gmtime_r(&info.st_mtime, &timeInfo) )
        {
            //TBD::Error
        }

        year = timeInfo.tm_year;
        month = timeInfo.tm_mon;
        day = timeInfo.tm_mday;
        
    }

    std::ostringstream sout;

    sout<<"/"<<year<<"/"<<month<<"/"<<day;

    Glib::ustring filePath = path_ + sout.str();

    {
        Glib::RefPtr<Gio::File> destPath 
                            = Gio::File::create_for_path( filePath );
        
        if( !destPath->query_exists() )
        {
        
#ifdef GLIBMM_EXCEPTIONS_ENABLED
            try
            {
                destPath->make_directory_with_parents();
            }
            catch( Glib::Error &)
            {
                //TBD::Error
            }
#else
            std::auto_ptr<Glib::Error> error;
            destPath->make_directory_with_parents(error);
#endif
        }
    }

    Glib::RefPtr<Gio::File> src = Gio::File::create_for_path(
                                    (*photo).get_disk_file_path() );
    filePath += "/";
    filePath += src->get_basename();    
    Glib::RefPtr<Gio::File> dest 
                        = Gio::File::create_for_path( filePath  );
    Glib::RefPtr<Gio::Cancellable> canc = Gio::Cancellable::create();

    //src->copy_async( dest, NULL, canc );
    src->copy( dest, Gio::FILE_COPY_OVERWRITE );

    Glib::ustring uri = get_storage_uri_prefix() + ":";
    uri += sout.str() + "/";
    uri += src->get_basename();

    photo->set_uri( uri );
    photo->set_disk_file_path(filePath);

    //Set modification date
    ModificationDate date( day, month, year);
    photo->set_modification_date( date );
    

    //Now generate thumbnails

    Glib::ustring tPath = store_.get_path() + sout.str();
    tPath += "/";
    tPath += src->get_basename() + "-thumb" ;

    Thumbnail thumb;
    thumb.set_path( tPath );
    thumb.generate( exifData, *photo );

    tPath += exifData.thumbnailExtension();

    thumb.set_path( tPath );
    photo->set_thumbnail( thumb );

    {
        //create Exif 
        ExifData exif( exifData );
        photo->set_exif_data( exif );
    }

    photo->save( *db_ );
    return;
}


Glib::ustring DirectoryStorage::retrieve(
                            const Photo &photo) throw(Error)
{
    //Just retrive path from uri and send back
    return (!photo.get_disk_file_path().empty() ) 
                    ? (path_+ "/" + photo.get_uri().substr(
                                     photo.get_uri().find(":") + 1, 
                                     photo.get_uri().length() ) )
                    : photo.get_disk_file_path();
}
                

void DirectoryStorage::downloadToDisk(
                        const PhotoList &, 
                        ProgressObserverPtr &observer) throw(Error)
{
    //Nothing to do
}


void DirectoryStorage::remove(const PhotoPtr &photo) throw(Error)
{
    Glib::ustring path = retrieve( *photo );
    Glib::RefPtr<Gio::File> file = Gio::File::create_for_path( path );
    file->remove();
    return;
    
}


void DirectoryStorage::remove(const PhotoList &photos) throw(Error)
{
    for(PhotoList::const_iterator photo = photos.begin();
                                photo != photos.end(); photo++)
    {
        remove( *photo );
    }
    return ;
}

std::string DirectoryStorage::get_storage_uri_prefix() throw()
{
    return "file";
}

void DirectoryStorage::final(Application & application) throw(Error)
{
    
}

} // namespace Solang
