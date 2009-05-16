/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
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

#include <cmath>

#include <cairomm/cairomm.h>

#include "cell-renderer-thumbnail.h"

namespace Solang
{

CellRendererThumbnail::CellRendererThumbnail() throw() :
    Gtk::CellRendererPixbuf(),
    extraHeight_(0)
{
    property_xalign().set_value(0.5);
    property_yalign().set_value(0.5);
}

CellRendererThumbnail::~CellRendererThumbnail() throw()
{
}

void
CellRendererThumbnail::render_vfunc(
    const Glib::RefPtr<Gdk::Drawable> & window,
    Gtk::Widget & widget,
    const Gdk::Rectangle & background_area,
    const Gdk::Rectangle & cell_area,
    const Gdk::Rectangle & expose_area,
    Gtk::CellRendererState flags)
{
    const gint height = background_area.get_height() + extraHeight_;
    const gint width = background_area.get_width();

    // Sometimes width is -1 - not sure what to do here.
    if (width == -1)
    {
        return;
    }

    const gint x = background_area.get_x();
    const gint y = background_area.get_y();

    const Cairo::RefPtr<Cairo::Context> context
              = window->create_cairo_context();
    const Glib::RefPtr<const Gtk::Style> style = widget.get_style();

    if (0 != (flags & (Gtk::CELL_RENDERER_SELECTED
                       | Gtk::CELL_RENDERER_PRELIT)))
    {

        Gtk::StateType state;

        if (0 != (flags & Gtk::CELL_RENDERER_SELECTED))
        {
            if (true == widget.has_focus())
            {
                state = Gtk::STATE_SELECTED;
            }
            else
            {
                state = Gtk::STATE_ACTIVE;
            }
        }
        else
        {
            state = Gtk::STATE_PRELIGHT;
        }

        const Gdk::Color color = style->get_base(state);
        Gdk::Cairo::set_source_color(context, color);

        const gint radius = 3;

        context->arc(x + radius, y + radius, radius,
                     M_PI, M_PI * 1.5);
        context->arc(x + width - radius, y + radius, radius,
                     M_PI * 1.5, 0);
        context->arc(x + width - radius, y + height - radius, radius,
                     0, M_PI * 0.5);
        context->arc(x + radius, y + height - radius, radius,
                     M_PI * 0.5, M_PI);
        context->close_path();

        context->set_fill_rule(Cairo::FILL_RULE_WINDING);
        context->fill();
    }
    else
    {
        const Gdk::Color color = style->get_dark(Gtk::STATE_NORMAL);
        Gdk::Cairo::set_source_color(context, color);

        context->rectangle(x, y, width, height);
        context->rectangle(x + 1, y + 1, width - 2, height - 2);

        context->set_fill_rule(Cairo::FILL_RULE_EVEN_ODD);
        context->fill();
    }

    Gtk::CellRendererPixbuf::render_vfunc(window, widget, background_area,
                                          cell_area, expose_area, flags);
}

void
CellRendererThumbnail::set_extra_height(guint height) throw()
{
    extraHeight_ = height;
}

} // namespace Solang
