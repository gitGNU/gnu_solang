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

#ifndef SOLANG_THUMBNAILER_PROXY_H
#define SOLANG_THUMBNAILER_PROXY_H

#include <string>

#include <dbus/dbus-glib.h>
#include <sigc++/sigc++.h>

#include "types.h"

namespace Solang
{

class ThumbnailerProxy
{
    public:
        typedef sigc::slot<void> SlotAsyncDequeue;

        typedef sigc::slot<void, guint> SlotAsyncQueue;

        typedef sigc::slot<void,
                           guint,
                           UStringList &,
                           guint,
                           Glib::ustring &>
            SlotSignalError;

        typedef sigc::slot<void, guint, UStringList &>
            SlotSignalReady;

        typedef sigc::slot<void, guint> SlotSignalStarted;

        ThumbnailerProxy() throw();

        ~ThumbnailerProxy() throw();

        guint
        queue(const UStringList & uris,
              const UStringList & mime_types,
              const std::string & flavour,
              const std::string & scheduler,
              guint handle_to_dequeue) throw(Glib::Error);

        void
        queue_async(const UStringList & uris,
                    const UStringList & mime_types,
                    const std::string & flavour,
                    const std::string & scheduler,
                    guint handle_to_dequeue,
                    const SlotAsyncQueue & slot) throw();

        void
        dequeue(guint handle) throw(Glib::Error);

        void
        dequeue_async(guint handle,
                      const SlotAsyncDequeue & slot) throw();

        void
        error_connect(const SlotSignalError & slot) throw();

        void
        ready_connect(const SlotSignalReady & slot) throw();

        void
        started_connect(const SlotSignalStarted & slot) throw();

        operator bool() const throw();

    private:
        static const std::string INTERFACE;

        static const std::string NAME;

        static const std::string PATH;

        bool serviceAvailable_;

        DBusGProxy * thumbnailerProxy_;
};

} // namespace Solang

#endif // SOLANG_THUMBNAILER_PROXY_H
