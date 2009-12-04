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
#endif

#include "scale-action.h"
#include "scale-tool-item.h"

namespace Solang
{

ScaleAction::ScaleAction(const Glib::ustring & name,
                         const Glib::ustring & tooltip,
                         const Gtk::Adjustment & adjustment) throw() :
    Gtk::Action(name, Gtk::StockID(),
                Glib::ustring(), tooltip),
    adjustment_(adjustment.get_value(),
                adjustment.get_lower(),
                adjustment.get_upper(),
                adjustment.get_step_increment(),
                adjustment.get_page_increment(),
                adjustment.get_page_size()),
    changeValue_(),
    valueChanged_()
{
}

ScaleAction::~ScaleAction() throw()
{
}

Glib::RefPtr<ScaleAction>
ScaleAction::create(const Glib::ustring & name,
                    const Glib::ustring & tooltip,
                    const Gtk::Adjustment & adjustment) throw()
{
    return Glib::RefPtr<ScaleAction>(new ScaleAction(name,
                                                     tooltip,
                                                     adjustment));
}

void
ScaleAction::connect_proxy_vfunc(Gtk::Widget * proxy)
{
    ScaleToolItem * const scale_tool_item
                              = dynamic_cast<ScaleToolItem *>(proxy);

    if (0 != scale_tool_item)
    {
        scale_tool_item->signal_change_value().connect(
            sigc::mem_fun(*this,
                          &ScaleAction::on_delegate_change_value));

        scale_tool_item->signal_value_changed().connect(
            sigc::mem_fun(*this,
                          &ScaleAction::on_delegate_value_changed));
    }

    Action::connect_proxy_vfunc(proxy);
}

Gtk::Widget *
ScaleAction::create_tool_item_vfunc()
{
    ScaleToolItem * const scale_tool_item
                              = new ScaleToolItem(adjustment_);

    scale_tool_item->signal_change_value().connect(
        sigc::mem_fun(*this,
                      &ScaleAction::on_delegate_change_value));

    scale_tool_item->signal_value_changed().connect(
        sigc::mem_fun(*this,
                      &ScaleAction::on_delegate_value_changed));

    return Gtk::manage(scale_tool_item);
}

double
ScaleAction::get_value() const throw()
{
    return adjustment_.get_value();
}

bool
ScaleAction::on_delegate_change_value(Gtk::ScrollType scroll,
                                      double new_value) throw()
{
    changeValue_.emit(scroll, new_value);
    return false;
}

void
ScaleAction::on_delegate_value_changed() throw()
{
    valueChanged_.emit();
}

void
ScaleAction::set_value(double value) throw()
{
    adjustment_.set_value(value);
}

sigc::signal<void, Gtk::ScrollType, double> &
ScaleAction::signal_change_value() throw()
{
    return changeValue_;
}

sigc::signal<void> &
ScaleAction::signal_value_changed() throw()
{
    return valueChanged_;
}

} // namespace Solang
