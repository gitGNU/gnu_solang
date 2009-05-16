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

#include "cell-renderer-info.h"

namespace Solang
{

CellRendererInfo::CellRendererInfo() throw() :
    Gtk::CellRendererText()
{
}

CellRendererInfo::~CellRendererInfo() throw()
{
}

void
CellRendererInfo::render_vfunc(
    const Glib::RefPtr<Gdk::Drawable> & window,
    Gtk::Widget & widget,
    const Gdk::Rectangle & background_area,
    const Gdk::Rectangle & cell_area,
    const Gdk::Rectangle & expose_area,
    Gtk::CellRendererState flags)
{
/*     if (0 != (flags & (Gtk::CELL_RENDERER_SELECTED */
/*                        | Gtk::CELL_RENDERER_PRELIT))) */
/*     { */
//         flags = Gtk::CELL_RENDERER_PRELIT;
//         const gint height = background_area.get_height();
//         const gint width = background_area.get_width();

//         // Sometimes width is -1 - not sure what to do here.
//         if (width == -1)
//         {
//             return;
//         }

//         Gtk::StateType state;

//         if (0 != (flags & Gtk::CELL_RENDERER_SELECTED))
//         {
//             if (true == widget.has_focus())
//             {
//                 state = Gtk::STATE_SELECTED;
//             }
//             else
//             {
//                 state = Gtk::STATE_ACTIVE;
//             }
//         }
//         else
//         {
//             state = Gtk::STATE_PRELIGHT;
//         }

//         const Cairo::RefPtr<Cairo::Context> context
//                   = window->create_cairo_context();

//         const Glib::RefPtr<const Gtk::Style> style = widget.get_style();
//         const Gdk::Color color = style->get_base(state);

//         Gdk::Cairo::set_source_color(context, color);

//         const gint radius = 4;
//         const gint x = background_area.get_x();
//         const gint y = background_area.get_y();

//         context->arc(x + radius, y + radius, radius,
//                      M_PI, M_PI * 1.5);
//         context->arc(x + width - radius, y + radius, radius,
//                      M_PI * 1.5, 0);
//         context->arc(x + width - radius, y + height - radius, radius,
//                      0, M_PI * 0.5);
//         context->arc(x + radius, y + height - radius, radius,
//                      M_PI * 0.5, M_PI);
//         context->close_path();

//         context->rectangle(x + 4, y + 4, width - 8, height - 8);

//         context->set_fill_rule(Cairo::FILL_RULE_WINDING);
//         context->fill();
//     }
//     else
//     {
//         const gint height = background_area.get_height();
//         const gint width = background_area.get_width();
//         const gint x = background_area.get_x();
//         const gint y = background_area.get_y();

//         const Glib::RefPtr<const Gtk::Style> style = widget.get_style();
//         style->paint_shadow(
//             Glib::RefPtr<Gdk::Window>::cast_dynamic(window),
//             Gtk::STATE_NORMAL, Gtk::SHADOW_IN, background_area,
//             widget, "", x + 3, y + 3, width - 6, height - 6);
//     }

    Gtk::CellRendererText::render_vfunc(window, widget, background_area,
                                        cell_area, expose_area, flags);
}

} // namespace Solang
