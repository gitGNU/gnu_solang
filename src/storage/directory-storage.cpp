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
#include "config.h"
#endif // HAVE_CONFIG_H

#include <iostream>
#include <cstdio>
#include <giomm.h>
#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "content-type-repo.h"
#include "directory-storage.h"
#include "exif-data.h"
#include "token-replacer.h"


namespace Solang
{

typedef TokenReplacer<3> FormatStringCreator;

const FormatStringCreator::ReplacementEntry replacementEntries[] = {
    { "<DAY>",   "%1" },
    { "<MONTH>", "%2" },
    { "<YEAR>",  "%3" }
};

FormatStringCreator replacer( replacementEntries );


DirectoryStorage::DirectoryStorage(
                    ThumbnailStore &store, 
                    const DatabasePtr &db,
                    const Glib::ustring &path)
    :Storage( store, db ),
    path_( path ),
    doNotCopy_( true )
{
    set_target_path_format("<YEAR>/<MONTH>/<DAY>");
}

DirectoryStorage::~DirectoryStorage() throw()
{
}

void DirectoryStorage::init(Application & application) throw(Error)
{
}

void DirectoryStorage::save(const PhotoPtr &photo) throw(Error)
{
    try
    {
        save( photo, false );
    }
    catch( Error &e )
    {
        e.add_call_info( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void DirectoryStorage::save(const PhotoPtr &photo, bool move) throw(Error)
{
    //Steps:
    // If exif is present
    //Read date and time 
    //else use modification date
    //Create directory and copy file
    guint32 year=0;
    guint32 month=0;
    guint32 day=0;
    Exiv2::Image::AutoPtr image;
    Exiv2::ExifData exifData;
    try
    {
        image = Exiv2::ImageFactory::open(
                           photo->get_disk_file_path().c_str());
        if(image.get() != 0)
        {
            image->readMetadata();
            exifData = image->exifData();
            if (exifData.empty()) {
                //No exif data
            }

            Exiv2::ExifKey key("Exif.Photo.DateTimeOriginal");
            Exiv2::ExifData::iterator dateTime = exifData.findKey(
                                                     key );
            if( dateTime != exifData.end() )
            {
                std::cout<<(*dateTime).toString()<<std::endl;
                Glib::ustring date = (*dateTime).toString();
                //TBD CHECK
                sscanf(date.c_str(), "%u:%u:%u", &year, &month, &day );
            }
        }
    }
    catch (...)
    {
    }

    if (exifData.empty())
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


    Glib::ustring subPath = Glib::ustring::compose(
                            get_target_path_format(),
                            day, month, year );

    Glib::ustring filePath;

    if( !doNotCopy_ )
    {
        filePath = path_ + "/" + subPath;
    }
    else
    {
        filePath = (*photo).get_disk_file_path();
    }

    FilePtr src = Gio::File::create_for_path(
                      photo->get_disk_file_path());
    if( !doNotCopy_ )
    {

        {
            FilePtr destPath = Gio::File::create_for_path( filePath );

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

        filePath += "/";
        filePath += src->get_basename();
        FilePtr dest = Gio::File::create_for_path( filePath  );
        Glib::RefPtr<Gio::Cancellable> canc
                                    = Gio::Cancellable::create();

        //src->copy_async( dest, NULL, canc );
        try
        {
            if( true == move )
                src->move( dest, Gio::FILE_COPY_OVERWRITE );
            else
                src->copy( dest, Gio::FILE_COPY_OVERWRITE );
        }
        catch( Glib::Error &e )
        {
            std::cerr<<"Error:"<<e.what()<<std::endl;
            //TBD::Error
        }
    }

    Glib::ustring uri = get_storage_uri_prefix() + ":";
    uri += filePath;
    Glib::ustring contentType
        = ContentTypeRepo::instance()->get_content_type(
              photo->get_disk_file_path() );

    photo->set_uri( uri );
    photo->set_content_type( contentType );
    photo->set_disk_file_path(filePath);

    //Set modification date
    ModificationDate date( day, month, year);
    photo->set_modification_date( date );
    

    //Now generate thumbnails

    Glib::ustring tPath = store_.get_path() + subPath;
    tPath += "/";
    tPath += src->get_basename() + "-thumb" ;

    Thumbnail thumb;
    thumb.set_path( tPath );
    thumb.generate( exifData, photo );

    //tPath += exifData.thumbnailExtension();
    tPath += ".jpg";

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
    //Just retrieve path from uri and send back
    return (photo.get_disk_file_path().empty() )
                    ? (photo.get_uri().substr(
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
    FilePtr file = Gio::File::create_for_path( path );
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

void
DirectoryStorage::set_target_path_format(
                                const Glib::ustring &fmt) throw()
{
    formatString_ = replacer.extract( fmt );
}

Glib::ustring
DirectoryStorage::get_target_path_format() const throw()
{
    return formatString_;
}

void
DirectoryStorage::set_do_not_copy( bool value ) throw()
{
    doNotCopy_ = value;
}

} // namespace Solang
