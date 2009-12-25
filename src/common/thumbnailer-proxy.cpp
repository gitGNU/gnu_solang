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

#include <iostream>

#include "solang-marshal.h"
#include "org-freedesktop-thumbnails-thumbnailer1-dbus-bindings.h"
#include "thumbnailer-proxy.h"

namespace Solang
{

const std::string ThumbnailerProxy::INTERFACE
    = "org.freedesktop.thumbnails.Thumbnailer1";

const std::string ThumbnailerProxy::NAME
    = "org.freedesktop.thumbnails.Thumbnailer1";

const std::string ThumbnailerProxy::PATH
    = "/org/freedesktop/thumbnails/Thumbnailer1";

static UStringList
list_from_strv(const char ** strv) throw()
{
    UStringList ustrings;

    for (const char ** iter = strv; 0 != iter && 0 != *iter; iter++)
    {
        ustrings.push_back(Glib::ustring(*iter));
    }

    return ustrings;
}

static ConstCharPtrPtr
strv_from_list(const UStringList & ustrings) throw()
{
    ConstCharPtrPtr strv(new const char * [ustrings.size() + 1]);

    guint i = 0;
    for (UStringList::const_iterator iter = ustrings.begin();
         ustrings.end() != iter;
         iter++, i++)
    {
        (&*strv)[i] = iter->c_str();
    }
    (&*strv)[i] = 0;

    return strv;
}

static void
signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_error_cb(
    DBusGProxy * proxy,
    guint handle,
    const char ** uris,
    gint code,
    const gchar * message,
    gpointer user_data)
{
    if (0 == user_data)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << "Not an instance of ThumbnailerProxy::SlotSignalReady"
                  << std::endl;
        return;
    }

    ThumbnailerProxy::SlotSignalError * const slot
        = static_cast<ThumbnailerProxy::SlotSignalError *>(user_data);
    UStringList ustrings = list_from_strv(uris);
    Glib::ustring ustring = (0 == message) ? Glib::ustring(message)
                                           : "";
    (*slot)(handle, ustrings, code, ustring);
}

static void
signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_finished_cb(
    DBusGProxy * proxy,
    guint handle,
    gpointer user_data)
{
}

static void
signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_ready_cb(
    DBusGProxy * proxy,
    guint handle,
    const char ** uris,
    gpointer user_data)
{
    if (0 == user_data)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << "Not an instance of ThumbnailerProxy::SlotSignalReady"
                  << std::endl;
        return;
    }

    ThumbnailerProxy::SlotSignalReady * const slot
        = static_cast<ThumbnailerProxy::SlotSignalReady *>(user_data);
    UStringList ustrings = list_from_strv(uris);

    (*slot)(handle, ustrings);
}

static void
signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_started_cb(
    DBusGProxy * proxy,
    guint handle,
    gpointer user_data)
{
    if (0 == user_data)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << "Not an instance of ThumbnailerProxy::SlotSignalStarted"
                  << std::endl;
        return;
    }

    ThumbnailerProxy::SlotSignalStarted * const slot
        = static_cast<ThumbnailerProxy::SlotSignalStarted *>(user_data);

    (*slot)(handle);
}

static void
signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_queue_reply(
    DBusGProxy * proxy,
    guint handle,
    GError * error,
    gpointer user_data)
{
    if (0 != error)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << error->message << std::endl;
        g_error_free(error);
        return;
    }

    if (0 == user_data)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << "Not an instance of ThumbnailerProxy::SlotAsyncReply"
                  << std::endl;
        return;
    }

    ThumbnailerProxy::SlotAsyncQueue * const slot
        = static_cast<ThumbnailerProxy::SlotAsyncQueue *>(user_data);

    if (0 != slot)
    {
        (*slot)(handle);
    }

    delete slot;
}

static void
signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_dequeue_reply(
    DBusGProxy * proxy,
    GError * error,
    gpointer user_data)
{
    if (0 != error)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << error->message << std::endl;
        g_error_free(error);
        return;
    }

    if (0 == user_data)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << "Not an instance of ThumbnailerProxy::SlotAsyncReady"
                  << std::endl;
        return;
    }

    ThumbnailerProxy::SlotAsyncDequeue * const slot
        = static_cast<ThumbnailerProxy::SlotAsyncDequeue *>(user_data);

    if (0 != slot)
    {
        (*slot)();
    }

    delete slot;
}

static void
slot_signal_error_destroy(gpointer data, GClosure * closure)
{
    if (0 == data)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << "Not an instance of ThumbnailerProxy::SlotSignalError"
                  << std::endl;
        return;
    }

    ThumbnailerProxy::SlotSignalError * const slot
        = static_cast<ThumbnailerProxy::SlotSignalError *>(data);

    delete slot;
}

static void
slot_signal_ready_destroy(gpointer data, GClosure * closure)
{
    if (0 == data)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << "Not an instance of ThumbnailerProxy::SlotSignalReady"
                  << std::endl;
        return;
    }

    ThumbnailerProxy::SlotSignalReady * const slot
        = static_cast<ThumbnailerProxy::SlotSignalReady *>(data);

    delete slot;
}

ThumbnailerProxy::ThumbnailerProxy() throw() :
    serviceAvailable_(false),
    thumbnailerProxy_(0)
{
    GError * error = 0;
    DBusGConnection * const connection = dbus_g_bus_get(
                                             DBUS_BUS_SESSION,
                                             &error);

    if (0 != error)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << error->message << std::endl;
        g_error_free(error);
    }

    serviceAvailable_ = true;

    thumbnailerProxy_ = dbus_g_proxy_new_for_name(connection,
                                                  NAME.c_str(),
                                                  PATH.c_str(),
                                                  INTERFACE.c_str());
    dbus_g_connection_unref(connection);

    dbus_g_object_register_marshaller(
        solang_marshal_VOID__UINT_POINTER_UINT_STRING,
        G_TYPE_NONE,
        G_TYPE_UINT, 
        G_TYPE_STRV, 
        G_TYPE_UINT, 
        G_TYPE_STRING,
        G_TYPE_INVALID);

    dbus_g_object_register_marshaller(
        static_cast<GClosureMarshal>(
            g_cclosure_marshal_VOID__UINT_POINTER),
        G_TYPE_NONE,
        G_TYPE_UINT,
        G_TYPE_STRV,
        G_TYPE_INVALID);

    dbus_g_proxy_add_signal(thumbnailerProxy_,
                            "Error",
                            G_TYPE_UINT,
                            G_TYPE_STRV,
                            G_TYPE_UINT,
                            G_TYPE_STRING,
                            G_TYPE_INVALID);

    dbus_g_proxy_add_signal(thumbnailerProxy_,
                            "Finished",
                            G_TYPE_UINT,
                            G_TYPE_INVALID);

    dbus_g_proxy_add_signal(thumbnailerProxy_,
                            "Ready",
                            G_TYPE_UINT,
                            G_TYPE_STRV,
                            G_TYPE_INVALID);

    dbus_g_proxy_add_signal(thumbnailerProxy_,
                            "Started",
                            G_TYPE_UINT,
                            G_TYPE_INVALID);
}

ThumbnailerProxy::~ThumbnailerProxy() throw()
{
    g_object_unref(thumbnailerProxy_);
}

guint
ThumbnailerProxy::queue(const UStringList & uris,
                        const UStringList & mime_types,
                        const std::string & flavour,
                        const std::string & scheduler,
                        guint handle_to_dequeue) throw(Glib::Error)
{
    ConstCharPtrPtr uri_v = strv_from_list(uris);
    ConstCharPtrPtr mime_type_v = strv_from_list(uris);
    GError * error = 0;
    guint handle;

    org_freedesktop_thumbnails_Thumbnailer1_queue(
        thumbnailerProxy_,
        &*uri_v,
        &*mime_type_v,
        flavour.c_str(),
        scheduler.c_str(),
        handle_to_dequeue,
        &handle,
        &error);
    if (0 != error)
    {
        Glib::Error::throw_exception(error);
    }

    return handle;
}

void
ThumbnailerProxy::queue_async(const UStringList & uris,
                              const UStringList & mime_types,
                              const std::string & flavour,
                              const std::string & scheduler,
                              guint handle_to_dequeue,
                              const SlotAsyncQueue & slot) throw()
{
    ConstCharPtrPtr uri_v = strv_from_list(uris);
    ConstCharPtrPtr mime_type_v = strv_from_list(mime_types);
    SlotAsyncQueue * const slot_copy = new SlotAsyncQueue(slot);

    org_freedesktop_thumbnails_Thumbnailer1_queue_async(
        thumbnailerProxy_,
        &*uri_v,
        &*mime_type_v,
        flavour.c_str(),
        scheduler.c_str(),
        handle_to_dequeue,
        signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_queue_reply,
        slot_copy);
}

void
ThumbnailerProxy::dequeue(guint handle) throw(Glib::Error)
{
    GError * error = 0;

    org_freedesktop_thumbnails_Thumbnailer1_dequeue(thumbnailerProxy_,
                                                    handle,
                                                    &error);
    if (0 != error)
    {
        Glib::Error::throw_exception(error);
    }
}

void
ThumbnailerProxy::dequeue_async(guint handle,
                                const SlotAsyncDequeue & slot) throw()
{
    SlotAsyncDequeue * const slot_copy = new SlotAsyncDequeue(slot);

    org_freedesktop_thumbnails_Thumbnailer1_dequeue_async(
        thumbnailerProxy_,
        handle,
        signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_dequeue_reply,
        slot_copy);
}

void
ThumbnailerProxy::error_connect(const SlotSignalError & slot) throw()
{
    SlotSignalError * const slot_copy = new SlotSignalError(slot);

    dbus_g_proxy_connect_signal(
        thumbnailerProxy_,
        "Error",
        G_CALLBACK(signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_error_cb),
        slot_copy,
        slot_signal_error_destroy);
}

void
ThumbnailerProxy::ready_connect(const SlotSignalReady & slot) throw()
{
    SlotSignalReady * const slot_copy = new SlotSignalReady(slot);

    dbus_g_proxy_connect_signal(
        thumbnailerProxy_,
        "Ready",
        G_CALLBACK(signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_ready_cb),
        slot_copy,
        slot_signal_ready_destroy);
}

void
ThumbnailerProxy::started_connect(const SlotSignalStarted & slot) throw()
{
    SlotSignalStarted * const slot_copy = new SlotSignalStarted(slot);

    dbus_g_proxy_connect_signal(
        thumbnailerProxy_,
        "Started",
        G_CALLBACK(signal_proxy_org_freedesktop_thumbnails_Thumbnailer1_started_cb),
        slot_copy,
        slot_signal_ready_destroy);
}

ThumbnailerProxy::operator bool() const throw()
{
    return serviceAvailable_;
}

} // namespace Solang
