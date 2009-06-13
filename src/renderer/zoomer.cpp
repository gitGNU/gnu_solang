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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glibmm/i18n.h>

#include "zoomer.h"

namespace Solang
{

static const std::string zoomInFile
    = PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/view-near-22.png";

static const std::string zoomOutFile
    = PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/view-far-22.png";

Zoomer::Zoomer(guint min, guint max, guint mid, guint step) throw() :
    Gtk::HBox(false, 6),
    actionZoomIn_(Gtk::Action::create(
                      "ActionViewZoomIn", Gtk::Stock::ZOOM_IN,
                      _("_Zoom In"),
                      _("Enlarge the thumbnails"))),
    actionZoomOut_(Gtk::Action::create(
                       "ActionViewZoomOut", Gtk::Stock::ZOOM_OUT,
                       _("Zoom _Out"),
                       _("Shrink the thumbnails"))),
    zoomOutEventBox_(),
    zoomOutImage_(zoomOutFile),
    hScale_(min, max, step),
    zoomInEventBox_(),
    zoomInImage_(zoomInFile)
{
    actionZoomIn_->connect_proxy(zoomInEventBox_);
    actionZoomIn_->signal_activate().connect(
        sigc::hide_return(sigc::bind(
            sigc::mem_fun(*this,
                          &Zoomer::on_action_view_zoom_in),
                          reinterpret_cast<GdkEventButton *>(0))));

    actionZoomOut_->connect_proxy(zoomOutEventBox_);
    actionZoomOut_->signal_activate().connect(
        sigc::hide_return(sigc::bind(
            sigc::mem_fun(*this,
                          &Zoomer::on_action_view_zoom_out),
                          reinterpret_cast<GdkEventButton *>(0))));

    zoomOutImage_.show();
    zoomOutEventBox_.add(zoomOutImage_);
    zoomOutEventBox_.set_above_child(false);
    zoomOutEventBox_.set_visible_window(true);
    pack_start(zoomOutEventBox_, Gtk::PACK_SHRINK, 0);

    hScale_.set_value(mid);
    hScale_.set_digits(0);
    hScale_.set_draw_value(false);
    hScale_.set_size_request(132, -1);
    pack_start(hScale_, Gtk::PACK_EXPAND_WIDGET, 0);

    zoomInImage_.show();
    zoomInEventBox_.add(zoomInImage_);
    zoomInEventBox_.set_above_child(false);
    zoomInEventBox_.set_visible_window(true);
    pack_start(zoomInEventBox_, Gtk::PACK_SHRINK, 0);

    zoomInEventBox_.signal_button_press_event().connect(
        sigc::mem_fun(*this,
                      &Zoomer::on_action_view_zoom_in));

    zoomOutEventBox_.signal_button_press_event().connect(
        sigc::mem_fun(*this,
                      &Zoomer::on_action_view_zoom_out));

    show_all_children();
}

Zoomer::~Zoomer() throw()
{
}

const ActionPtr &
Zoomer::action_zoom_in() throw()
{
    return actionZoomIn_;
}

const ActionPtr &
Zoomer::action_zoom_out() throw()
{
    return actionZoomOut_;
}

Gtk::HScale &
Zoomer::get_scale() throw()
{
    return hScale_;
}

bool
Zoomer::on_action_view_zoom_in(GdkEventButton * event) throw()
{
    if (0 != event
        && (1 != event->button || GDK_BUTTON_PRESS != event->type))
    {
        return true;
    }

    const Gtk::Adjustment * const adjustment
                                      = hScale_.get_adjustment();
    const double step = adjustment->get_step_increment();
    const double value = hScale_.get_value();

    hScale_.set_value(value + step);

    return true;
}

bool
Zoomer::on_action_view_zoom_out(GdkEventButton * event) throw()
{
    if (0 != event
        && (1 != event->button || GDK_BUTTON_PRESS != event->type))
    {
        return true;
    }

    const Gtk::Adjustment * const adjustment
                                      = hScale_.get_adjustment();
    const double step = adjustment->get_step_increment();
    const double value = hScale_.get_value();

    hScale_.set_value(value - step);

    return true;
}

} // namespace Solang
