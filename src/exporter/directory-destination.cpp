/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#include <string>

#include <giomm.h>
#include <glib/gstdio.h>
#include <glibmm/i18n.h>

#include "directory-destination.h"
#include "photo.h"
#include "photo-destination-enums.h"
#include "progress-observer.h"

namespace Solang
{

DirectoryDestination::DirectoryDestination() throw() :
    PhotoDestination(),
    createArchive_(false),
    filename_(),
    fileChooserButton_(0),
    initEnd_()
{
}

DirectoryDestination::~DirectoryDestination() throw()
{
}

void
DirectoryDestination::init(Application & application) throw()
{
    initEnd_.emit(true);
}

void
DirectoryDestination::final(Application & application) throw()
{
}

void
DirectoryDestination::export_photo(
                          const PhotoPtr & photo,
                          const IStoragePtr & storage,
                          const ProgressObserverPtr & observer)
                          throw()
{
    photo->set_disk_file_path(storage);

    std::string path;

    try
    {
        path = Glib::filename_from_utf8(photo->get_disk_file_path());
    }
    catch (const Glib::ConvertError & e)
    {
        g_warning("%s", e.what().c_str());
        return;
    }

    const FilePtr file = Gio::File::create_for_path(path);
    const FilePtr dest = Gio::File::create_for_path(
                             filename_ + "/" + file->get_basename());

    try
    {
        file->copy(dest, Gio::FILE_COPY_NONE);
    }
    catch (const Gio::Error & e)
    {
        g_warning("%s", e.what().c_str());
    }

    observer->receive_event_notifiation();
}

void
DirectoryDestination::export_photos(
                          const PhotoList & photos,
                          const IStoragePtr & storage,
                          const ProgressObserverPtr & observer)
                          throw()
{
    observer->set_event_description(_("Exporting Photos"));
    observer->set_num_events(photos.size());

    if (true == createArchive_)
    {
        Glib::Date date;
        date.set_time_current();

        const std::string tmp_dirname = date.format_string("%Y%m%d");
        filename_ += "/";
        filename_ += tmp_dirname;

        if (false == Glib::file_test(filename_,
                                     Glib::FILE_TEST_EXISTS))
        {
            try
            {
                Gio::File::create_for_path(
                    filename_)->make_directory_with_parents();
            }
            catch (const Gio::Error & e)
            {
                g_warning("%s", e.what().c_str());
            }
        }
    }

    PhotoList::const_iterator it;

    for (it = photos.begin(); photos.end() != it; it++)
    {
        export_photo(*it, storage, observer);
    }

    if (true == createArchive_)
    {
        const std::string command_line
            = "file-roller --add-to=" + filename_ + ".zip"
              + " --add " + filename_;

        Glib::spawn_command_line_sync(command_line);
        g_remove(filename_.c_str());
    }

    return;
}

void DirectoryDestination::final() throw()
{
}

sigc::signal<void, bool> &
DirectoryDestination::init_end() throw()
{
    return initEnd_;
}

void
DirectoryDestination::read_selection() throw()
{
    try
    {
        filename_ = Glib::filename_from_utf8(
                        fileChooserButton_->get_filename());
    }
    catch (const Glib::ConvertError & e)
    {
        g_warning("%s", e.what().c_str());
        return;
    }
}

Gtk::Widget &
DirectoryDestination::get_browser() throw()
{
    fileChooserButton_ = new Gtk::FileChooserButton(
                             _("Select Folder"),
                             Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

    return *Gtk::manage(fileChooserButton_);
}

Glib::ustring
DirectoryDestination::get_label() const throw()
{
    return Glib::ustring(_("_Folder..."));
}

Glib::ustring
DirectoryDestination::get_name() const throw()
{
    return Glib::ustring("Folder");
}

gint
DirectoryDestination::get_options() const throw()
{
    return PHOTO_DESTINATION_CREATE_ARCHIVE_OPTIONAL;
}

Gtk::StockID
DirectoryDestination::get_stock_id() const throw()
{
    return Gtk::Stock::DIRECTORY;
}

void
DirectoryDestination::set_create_archive(bool value) throw()
{
    createArchive_ = value;
}

} // namespace Solang
