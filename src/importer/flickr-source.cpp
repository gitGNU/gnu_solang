/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * flickr-source.cpp
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * flickr-source.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * flickr-source.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "flickr-source.h"
#include "progress-observer.h"

namespace Solang
{

FlickrSource::FlickrSource() throw() :
    PhotoSource(),
    comboBoxEntry_()
{
}

FlickrSource::~FlickrSource() throw()
{
}

PhotoPtr
FlickrSource::import(const PhotoPtr & photo,
                     const IStoragePtr & storage,
                     const TagList &tags, Database & db,
                     const ProgressObserverPtr & observer) throw()
{
	return PhotoPtr(photo);
}

PhotoList
FlickrSource::import(const PhotoList & photos,
                     const IStoragePtr & storage,
                     const TagList &tags, Database & db,
                     const ProgressObserverPtr & observer) throw()
{
    observer->set_num_events(photos.size());
    observer->set_event_description("Importing photos");

	PhotoList imported_photos;
    return imported_photos;
}

PhotoList
FlickrSource::import(const IStoragePtr & storage, const TagList &tags,
                     Database & db,
                     const ProgressObserverPtr & observer) throw()
{
    PhotoList files;
    return import(files, storage, tags, db, observer);
}

Gtk::Widget &
FlickrSource::get_browser() throw()
{
    return comboBoxEntry_;
}

Glib::ustring
FlickrSource::get_label() const throw()
{
    return Glib::ustring("F_lickr...");
}

Glib::ustring
FlickrSource::get_name() const throw()
{
    return Glib::ustring("Flickr");
}

Gtk::StockID
FlickrSource::get_stock_id() const throw()
{
    return Gtk::StockID();
}

} // namespace Solang
