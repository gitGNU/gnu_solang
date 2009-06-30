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

#ifndef SOLANG_PROGRESS_OBSERVER_H
#define SOLANG_PROGRESS_OBSERVER_H

#include <tr1/memory>

#include <glibmm.h>

namespace Solang
{

class ProgressObserver
{
    public:
        ProgressObserver() throw();

        ProgressObserver(const ProgressObserver & source) throw();

        ProgressObserver &
        operator=(const ProgressObserver & source) throw();

        ~ProgressObserver() throw();

        inline guint64
        get_num_events() const throw();

        void
        set_num_events(guint64 numEvents) throw();

        inline guint64
        get_current_events() const throw();

        void
        set_current_events(guint64 current_events) throw();

        inline const Glib::ustring &
        get_event_description() const throw();

        void
        set_event_description(const Glib::ustring & event_description)
                              throw();

        inline bool
        get_stop() const throw();

        void
        set_stop(bool value) throw();

        void
        receive_event_notifiation() throw();

        inline Glib::Dispatcher &
        description_changed() throw();

        inline Glib::Dispatcher &
        progress() throw();

        void 
        reset() throw();

    private:
        guint64 numEvents_;

        guint64 currentEvents_;

        Glib::Dispatcher descriptionChanged_;

        Glib::Dispatcher progress_;

        mutable Glib::Mutex mutex_;

        Glib::ustring eventDescription_;

        bool stop_;
};


inline guint64
ProgressObserver::get_num_events() const throw()
{
    Glib::Mutex::Lock lock(mutex_);
    return numEvents_;
}

inline guint64
ProgressObserver::get_current_events() const throw()
{
    Glib::Mutex::Lock lock(mutex_);
    return currentEvents_;
}

inline const Glib::ustring &
ProgressObserver::get_event_description() const throw()
{
    Glib::Mutex::Lock lock(mutex_);
    return eventDescription_;
}

inline bool
ProgressObserver::get_stop() const throw()
{
    Glib::Mutex::Lock lock(mutex_);
    return stop_;
}

inline Glib::Dispatcher &
ProgressObserver::description_changed() throw()
{
    Glib::Mutex::Lock lock(mutex_);
    return descriptionChanged_;
}

inline Glib::Dispatcher &
ProgressObserver::progress() throw()
{
    Glib::Mutex::Lock lock(mutex_);
    return progress_;
}

} // namespace Solang

#endif // SOLANG_PROGRESS_OBSERVER_H
