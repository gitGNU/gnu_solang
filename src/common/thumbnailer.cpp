/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#include <utility>

#include <glibmm.h>

#include "photo.h"
#include "thumbnailer.h"

namespace Solang
{

Thumbnailer::Thumbnailer() throw() :
    map_(),
    pendingList_(),
    thumbnailerProxy_()
{
    thumbnailerProxy_.error_connect(
        sigc::mem_fun(*this,
                      &Thumbnailer::on_signal_error));
    thumbnailerProxy_.ready_connect(
        sigc::mem_fun(*this,
                      &Thumbnailer::on_signal_ready));
    thumbnailerProxy_.started_connect(
        sigc::mem_fun(*this,
                      &Thumbnailer::on_signal_started));
}

Thumbnailer::~Thumbnailer() throw()
{
    signalTimeout_.disconnect();
}

void
Thumbnailer::on_async_queue(guint handle, const PhotoList & photos)
                            throw()
{
    std::map<Glib::ustring, PhotoPtr> m;

    for (PhotoList::const_iterator iter = photos.begin();
         photos.end() != iter;
         iter++)
    {
        const PhotoPtr photo = *iter;
        m.insert(std::make_pair(photo->get_uri(), photo));
    }

    map_.insert(std::make_pair(handle, m));
}

void
Thumbnailer::on_signal_error(guint handle,
                             UStringList & uris,
                             guint code,
                             Glib::ustring & message) throw()
{
}

void
Thumbnailer::on_signal_ready(guint handle, UStringList & uris)
                             throw()
{
    if (map_.end() == map_.find(handle))
    {
        return;
    }

    std::map<Glib::ustring, PhotoPtr> & m = map_[handle];

    for (UStringList::const_iterator iter = uris.begin();
         uris.end() != iter;
         iter++)
    {
        const Glib::ustring & uri = *iter;

        if (m.end() == m.find(uri))
        {
            continue;
        }
        m[uri]->set_thumbnail_state(Photo::THUMBNAIL_STATE_READY);
    }
}

void
Thumbnailer::on_signal_started(guint handle) throw()
{
}

bool
Thumbnailer::process() throw()
{
    if (true == pendingList_.empty())
    {
        return true;
    }

    UStringList mime_types;
    UStringList uris;

    for (PhotoList::const_iterator iter = pendingList_.begin();
         pendingList_.end() != iter;
         iter++)
    {
        mime_types.push_back((*iter)->get_content_type());
        uris.push_back((*iter)->get_uri());
    }

    thumbnailerProxy_.queue_async(
        uris,
        mime_types,
        "normal",
        "forward",
        0,
        sigc::bind(sigc::mem_fun(*this,
                                 &Thumbnailer::on_async_queue),
                   pendingList_));
    pendingList_.clear();

    return true;
}

void
Thumbnailer::push(const PhotoPtr & photo) throw()
{
    pendingList_.push_back(photo);

    if (0 == signalTimeout_)
    {
        signalTimeout_
            = Glib::signal_timeout().connect(
                  sigc::mem_fun(*this,
                                &Thumbnailer::process),
                  100,
                  Glib::PRIORITY_DEFAULT);
    }
}

} // namespace Solang
