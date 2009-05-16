/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * progress-observer.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * progress-observer.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * progress-observer.h is distributed in the hope that it will be useful, but
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
        progress() throw();

        void 
        reset() throw();

    private:
        guint64 numEvents_;

        guint64 currentEvents_;

        Glib::Dispatcher progress_;

        Glib::ustring eventDescription_;

        bool stop_;
};


inline guint64
ProgressObserver::get_num_events() const throw()
{
    return numEvents_;
}

inline guint64
ProgressObserver::get_current_events() const throw()
{
    return currentEvents_;
}

inline const Glib::ustring &
ProgressObserver::get_event_description() const throw()
{
    return eventDescription_;
}

inline bool
ProgressObserver::get_stop() const throw()
{
    return stop_;
}

inline Glib::Dispatcher &
ProgressObserver::progress() throw()
{
    return progress_;
}

} // namespace Solang

#endif // SOLANG_PROGRESS_OBSERVER_H
