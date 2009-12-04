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

#ifndef SOLANG_SCALE_TOOL_ITEM_H
#define SOLANG_SCALE_TOOL_ITEM_H

#include <glibmm.h>
#include <gtkmm.h>

namespace Solang
{

class ScaleToolItem :
    public Gtk::ToolItem
{
    public:
        ScaleToolItem(Gtk::Adjustment & adjustment) throw();

        virtual
        ~ScaleToolItem() throw();

        double
        get_value() const throw();

        Glib::SignalProxy2<bool, Gtk::ScrollType, double>
        signal_change_value() throw();

        Glib::SignalProxy0<void>
        signal_value_changed() throw();

    protected:
        virtual void
        on_toolbar_reconfigured() throw();

        Gtk::HScale hScale_;

    private:
};

} // namespace Solang

#endif // SOLANG_SPINNER_TOOL_ITEM_H
