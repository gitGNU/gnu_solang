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

#include "config.h"

#include "progress-observer.h"

namespace Solang
{

ProgressObserver::ProgressObserver() throw() :
    numEvents_(0),
    currentEvents_(0),
    descriptionChanged_(),
    progress_(),
    mutex_(),
    eventDescription_(),
    stop_()
{
}

ProgressObserver::ProgressObserver(const ProgressObserver & source)
    throw ():
    numEvents_(source.numEvents_),
    currentEvents_(source.currentEvents_),
    // progress_(source.progress_),
    eventDescription_(source.eventDescription_),
    stop_(source.stop_)
{
}

ProgressObserver &
ProgressObserver::operator=(const ProgressObserver & source) throw()
{
    if (&source != this)
    {
        numEvents_ = source.numEvents_;
        currentEvents_ = source.currentEvents_;
        // progress_ = source.progress_;
        eventDescription_ = source.eventDescription_;
        stop_ = source.stop_;
    }
    
    return *this;
}

ProgressObserver::~ProgressObserver() throw()
{
}

void
ProgressObserver::set_num_events(guint64 num_events) throw()
{
    Glib::Mutex::Lock lock(mutex_);
    numEvents_ = num_events;
}

void
ProgressObserver::set_current_events(guint64 current_events) throw()
{
    Glib::Mutex::Lock lock(mutex_);
    currentEvents_ = current_events;
    progress_.emit();
}

void
ProgressObserver::set_event_description(
    const Glib::ustring & event_description) throw()
{
    Glib::Mutex::Lock lock(mutex_);
    eventDescription_ = event_description;
    descriptionChanged_.emit();
}

void
ProgressObserver::receive_event_notifiation() throw()
{
    Glib::Mutex::Lock lock(mutex_);

    currentEvents_ = (currentEvents_ < numEvents_) ? currentEvents_ + 1
                                                   : numEvents_;
    progress_.emit();
}

void
ProgressObserver::set_stop(bool val) throw()
{
    Glib::Mutex::Lock lock(mutex_);
    stop_ = val;
}

void 
ProgressObserver::reset() throw()
{
    Glib::Mutex::Lock lock(mutex_);

    eventDescription_.clear();
    currentEvents_ = 0;
    numEvents_ = 0;
    stop_ = false;
    progress_.emit();
}

} // namespace Solang
