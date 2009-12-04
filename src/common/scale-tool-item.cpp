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

#include "scale-tool-item.h"

namespace Solang
{

ScaleToolItem::ScaleToolItem(Gtk::Adjustment & adjustment) throw() :
    Gtk::ToolItem(),
    hScale_(adjustment)
{
    add(hScale_);
    hScale_.set_size_request(132, -1);
    show_all_children();
}

ScaleToolItem::~ScaleToolItem() throw()
{
}

double
ScaleToolItem::get_value() const throw()
{
    return hScale_.get_value();
}

void
ScaleToolItem::on_toolbar_reconfigured() throw()
{
    const Gtk::ToolbarStyle toolbar_style = get_toolbar_style();

    switch (toolbar_style)
    {
        case Gtk::TOOLBAR_BOTH:
            hScale_.set_draw_value(true);
            break;

        default:
            hScale_.set_draw_value(false);
            break;
    }

    Gtk::ToolItem::on_toolbar_reconfigured();
}

Glib::SignalProxy2<bool, Gtk::ScrollType, double>
ScaleToolItem::signal_change_value() throw()
{
    return hScale_.signal_change_value();
}

Glib::SignalProxy0<void>
ScaleToolItem::signal_value_changed() throw()
{
    return hScale_.signal_value_changed();
}

} // namespace Solang
