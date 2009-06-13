/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_ZOOMER_H
#define SOLANG_ZOOMER_H

#include <gtkmm.h>

#include "types.h"

namespace Solang
{

class Zoomer :
    public Gtk::HBox
{
    public:
        Zoomer(guint min, guint max, guint mid, guint step) throw();

        virtual
        ~Zoomer() throw();

        const ActionPtr &
        action_zoom_in() throw();

        const ActionPtr &
        action_zoom_out() throw();

        Gtk::HScale &
        get_scale() throw();

    protected:
        bool
        on_action_view_zoom_in(GdkEventButton * event) throw();

        bool
        on_action_view_zoom_out(GdkEventButton * event) throw();

        ActionPtr actionZoomIn_;

        ActionPtr actionZoomOut_;

        Gtk::EventBox zoomOutEventBox_;

        Gtk::Image zoomOutImage_;

        Gtk::HScale hScale_;

        Gtk::EventBox zoomInEventBox_;

        Gtk::Image zoomInImage_;

    private:
};

} // namespace Solang

#endif // SOLANG_ZOOMER_H
