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

#include "tracker-client.h"

namespace Solang
{

static void
tracker_client_append_result_foreach(gpointer data,
                                     gpointer user_data)
{
    if (0 == user_data)
    {
        std::cerr << G_STRLOC << ", " << G_STRFUNC << ": "
                  << "Not an instance of std::vector<UStringList>"
                  << std::endl;
        return;
    }

    const GStrv element = static_cast<GStrv>(data);
    std::vector<UStringList> * const resultv
        = static_cast<std::vector<UStringList> *>(user_data);

    UStringList strings;
    for (gint i = 0; 0 != element[i]; i++)
    {
        strings.push_back(std::string(element[i]));
    }
    resultv->push_back(strings);
}

static void
signal_proxy_tracker_reply_gptrarray(GPtrArray * result,
                                     GError * error,
                                     gpointer user_data) throw()
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
                  << "Not an instance of TrackerClient::SlotAsyncReply"
                  << std::endl;
        return;
    }

    TrackerClient::SlotAsyncReply * const slot
        = static_cast<TrackerClient::SlotAsyncReply *>(user_data);

    std::vector<UStringList> resultv;

    g_ptr_array_foreach (result,
                         tracker_client_append_result_foreach,
                         &resultv);

    g_ptr_array_foreach (result, (GFunc) g_strfreev, NULL);
    g_ptr_array_free (result, TRUE);

    if (0 != slot)
    {
        (*slot)(resultv);
    }

    delete slot;
}

static void
signal_proxy_tracker_reply_void(GError * error, gpointer user_data)
                                throw()
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
                  << "Not an instance of TrackerClient::SlotAsyncReady"
                  << std::endl;
        return;
    }

    TrackerClient::SlotAsyncReady * const slot
        = static_cast<TrackerClient::SlotAsyncReady *>(user_data);

    if (0 != slot)
    {
        (*slot)();
    }

    delete slot;
}

TrackerClient::TrackerClient() :
    trackerClient_(tracker_connect(TRUE, -1))
{
}

TrackerClient::~TrackerClient()
{
    tracker_disconnect(trackerClient_);
}

std::vector<UStringList>
TrackerClient::sparql_query(const std::string & query) const
                            throw(Glib::Error)
{
    GError * error = 0;
    GPtrArray * const result = tracker_resources_sparql_query(
                                   trackerClient_,
                                   query.c_str(),
                                   &error);
    if (0 != error)
    {
        Glib::Error::throw_exception(error);
    }

    std::vector<UStringList> resultv;

    g_ptr_array_foreach (result,
                         tracker_client_append_result_foreach,
                         &resultv);

    g_ptr_array_foreach (result, (GFunc) g_strfreev, NULL);
    g_ptr_array_free (result, TRUE);

    return resultv;
}

guint
TrackerClient::sparql_query_async(const std::string & query,
                                  const SlotAsyncReply & slot) const
                                  throw()
{
    SlotAsyncReply * const slot_copy = new SlotAsyncReply(slot);

    return tracker_resources_sparql_query_async(
               trackerClient_,
               query.c_str(),
               signal_proxy_tracker_reply_gptrarray,
               slot_copy);
}

void
TrackerClient::sparql_update(const std::string & query) const
                             throw(Glib::Error)
{
    GError * error = 0;
    tracker_resources_sparql_update(trackerClient_,
                                    query.c_str(),
                                    &error);
    if (0 != error)
    {
        Glib::Error::throw_exception(error);
    }
}

guint
TrackerClient::sparql_update_async(const std::string & query,
                                   const SlotAsyncReady & slot) const
                                   throw()
{
    SlotAsyncReady * const slot_copy = new SlotAsyncReady(slot);

    return tracker_resources_sparql_update_async(
               trackerClient_,
               query.c_str(),
               signal_proxy_tracker_reply_void,
               slot_copy);
}

} // namespace Solang
