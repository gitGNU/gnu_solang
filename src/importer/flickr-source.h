/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * flickr-source.h
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * flickr-source.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * flickr-source.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_FLICKR_SOURCE_H
#define SOLANG_FLICKR_SOURCE_H

#include <list>
#include <string>

#include <flickcurl.h>
#include <glibmm.h>
#include <gtkmm.h>

#include "photo-source.h"
#include "types.h"

namespace Solang
{

class Application;
class FlickrChooserButton;
class FlickrInitializer;

class FlickrSource :
    public PhotoSource 
{
public:
    FlickrSource() throw();

    virtual
    ~FlickrSource() throw();

    virtual void
    init(Application & application) throw();

    virtual void
    final(Application & application) throw();

    virtual PhotoPtr
    import(const PhotoPtr & photo, const IStoragePtr & storage,
           const TagList &tags, Database & db,
           const ProgressObserverPtr & observer) throw();

    virtual PhotoList
    import(const PhotoList & photos, const IStoragePtr & storage,
           const TagList &tags, Database & db,
           const ProgressObserverPtr & observer) throw();

    virtual PhotoList
    import(const IStoragePtr & storage, const TagList &tags,
           Database & db, const ProgressObserverPtr & observer) throw();

    virtual sigc::signal<void, bool> &
    init_end() throw();

    void
    download_photos(PhotoList & files) throw();

    virtual void
    read_selection() throw();

    virtual Gtk::Widget &
    get_browser() throw();

    flickcurl *
    get_flickr_session() throw();

    virtual Glib::ustring
    get_label() const throw();

    virtual Glib::ustring
    get_name() const throw();

    virtual Gtk::StockID
    get_stock_id() const throw();

protected:
    void
    on_init_end(bool status,
                FlickrInitializerPtr & flickr_initializer) throw();

    FlickrChooserButton * flickrChooserButton_;

    FlickrContextPtr flickrContext_;

    std::list<std::string> uris_;

    sigc::signal<void, bool> initEnd_;

    sigc::connection signalInitEnd_;

private:
};

} // namespace Solang

#endif // SOLANG_FLICKR_SOURCE_H
