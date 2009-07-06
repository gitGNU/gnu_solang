/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * directory-source.cpp
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * file-system-source.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * file-system-source.cpp is distributed in the hope that it will be useful, but
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

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>

#include <glibmm/i18n.h>

#include "content-type-repo.h"
#include "directory-source.h"
#include "i-storage.h"
#include "photo-source-enums.h"
#include "photo-tag.h"
#include "progress-observer.h"
#include "tag.h"
#include "types.h"

namespace Solang
{

DirectorySource::DirectorySource() throw() :
    PhotoSource(),
    filename_(),
    fileChooserButton_(0),
    initEnd_()
{
}

DirectorySource::~DirectorySource() throw()
{
}

void
DirectorySource::init(Application & application) throw()
{
    initEnd_.emit(true);
}

void
DirectorySource::final(Application & application) throw()
{
}

PhotoPtr
DirectorySource::import(const PhotoPtr & photo,
                        const IStoragePtr & storage,
                        const TagList & tags, Database & db,
                        const ProgressObserverPtr & observer) throw()
{
    if (photo)
    {
        storage->save(photo);
        for( TagList::const_iterator it = tags.begin();
                            it != tags.end(); it++ )
        {
            PhotoTag pt(
                    photo->get_photo_id(), (*it)->get_tag_id());
            pt.save(db);
        }
    }

    return PhotoPtr(photo);
}

PhotoList
DirectorySource::import(const PhotoList & photos,
                        const IStoragePtr & storage,
                        const TagList & tags, Database & db,
                        const ProgressObserverPtr & observer) throw()
{
    observer->set_event_description(_("Importing Photos"));
    observer->set_num_events(photos.size());

    PhotoList imported_photos;
    for (PhotoList::const_iterator it = photos.begin();
                                   it != photos.end(); it++ )
    {
        if( observer )
        {
            if( observer->get_stop() )
            {
                return imported_photos;
            }
        }
        PhotoPtr photo = (*it);
        imported_photos.push_back(import(photo, storage, tags, db,
                                         observer));
        observer->receive_event_notifiation();
    }

    return imported_photos;
}

PhotoList
DirectorySource::import(
                    const IStoragePtr & storage, const TagList & tags,
                    Database & db,
                    const ProgressObserverPtr & observer) throw()
{
    PhotoList files;
    create_photo_list(files, filename_, filename_);
    return import(files, storage, tags, db, observer);
}

void DirectorySource::create_photo_list(PhotoList & list,
                                        const Glib::ustring & dir,
                                        const Glib::ustring & prefix)
                                        throw()
{
    Glib::Dir directory( dir );
    for( Glib::Dir::iterator entry = directory.begin();
             directory.end() != entry; entry++ )
    {
        std::string entryPath = dir + "/" + *entry;
        FilePtr entryFile = Gio::File::create_for_path( entryPath );
        if( Gio::FILE_TYPE_DIRECTORY == entryFile->query_file_type())
        {
            create_photo_list( list, entryPath, entryPath );
        }
        else
        {
            Glib::ustring contentType
                = ContentTypeRepo::instance()->get_content_type(
                                                   entryPath );
            if( contentType.find("image/") == 0 )
            {
                PhotoPtr photo(new Photo());
                list.push_back( photo );
                photo->set_disk_file_path( entryPath );
            }
        }
    }
}

sigc::signal<void, bool> &
DirectorySource::init_end() throw()
{
    return initEnd_;
}

void
DirectorySource::read_selection() throw()
{
    filename_ = fileChooserButton_->get_filename();
}

Gtk::Widget &
DirectorySource::get_browser() throw()
{
    fileChooserButton_ = new Gtk::FileChooserButton(
                             _("Select Folder"),
                             Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

    return *Gtk::manage(fileChooserButton_);
}

Glib::ustring
DirectorySource::get_label() const throw()
{
    return Glib::ustring(_("_Folder..."));
}

Glib::ustring
DirectorySource::get_name() const throw()
{
    return Glib::ustring("Folder");
}

gint
DirectorySource::get_options() const throw()
{
    return PHOTO_SOURCE_COPY_PHOTOS_OPTIONAL
           | PHOTO_SOURCE_INCLUDE_SUBFOLDERS_OPTIONAL;
}

Gtk::StockID
DirectorySource::get_stock_id() const throw()
{
    return Gtk::Stock::DIRECTORY;
}

} // namespace Solang
