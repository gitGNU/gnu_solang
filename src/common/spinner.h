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

/*
 * Based on code by: the gedit Team
 *
 * This widget was originally written in C as a part of gedit:
 * gedit/gedit-spinner.h
 */

#ifndef SOLANG_SPINNER_H
#define SOLANG_SPINNER_H

#include <tr1/memory>
#include <vector>

#include <gdkmm.h>
#include <glibmm.h>
#include <gtkmm.h>
#include <sigc++/sigc++.h>

#include "types.h"

namespace Solang
{

class Cache;
class Images;

class Spinner :
    public Gtk::Widget
{
    public:
        Spinner() throw();

        virtual
        ~Spinner() throw();

        void
        start() throw();

        void
        stop() throw();

        void
        set_size(const Gtk::IconSize & size) throw();

    protected:
        bool
        load_images() throw();

        void
        unload_images() throw();

        virtual bool
        on_expose_event(GdkEventExpose * event);

        void
        on_icon_theme_changed() throw();

        virtual void
        on_map();

        virtual void
        on_screen_changed(const ScreenPtr & old_screen);

        virtual void
        on_size_request(Gtk::Requisition * requisition);

        bool
        on_timeout() throw();

        virtual void
        on_unmap();

        IconThemePtr iconTheme_;

        CachePtr cache_;

        Gtk::IconSize size_;

        ImagesPtr images_;

        guint currentImage_;

	guint timeout_;

	guint timerTask_;

        bool spinning_;

        bool needLoad_;

        sigc::connection signalChanged_;

        sigc::connection signalTimeout_;

    private:
};

} // namespace Solang

#endif // SOLANG_SPINNER_H
