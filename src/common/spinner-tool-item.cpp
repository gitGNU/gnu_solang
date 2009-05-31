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
 * Based on code by: Christian Persch
 *
 * This widget was originally written in C as a part of Epiphany:
 * lib/widgets/ephy-spinner-tool-item.c
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "spinner-tool-item.h"

namespace Solang
{

SpinnerToolItem::SpinnerToolItem() throw() :
    Gtk::ToolItem(),
    spinner_()
{
    add(spinner_);
    spinner_.show();
}

SpinnerToolItem::~SpinnerToolItem() throw()
{
}

void
SpinnerToolItem::on_toolbar_reconfigured() throw()
{
    Gtk::IconSize spinner_size;
    const Gtk::ToolbarStyle toolbar_style = get_toolbar_style();

    // FIXME: be smarter by taking the toolbar icon size
    //        (Gtk::Toolbar::get_icon_size) into account.

    switch (toolbar_style)
    {
        case Gtk::TOOLBAR_BOTH:
            spinner_size = Gtk::ICON_SIZE_DIALOG;
            break;

        default:
            spinner_size = Gtk::ICON_SIZE_LARGE_TOOLBAR;
            break;
    }

    spinner_.set_size(spinner_size);
    Gtk::ToolItem::on_toolbar_reconfigured();
}

void
SpinnerToolItem::set_spinning(bool spinning) throw()
{
    switch (spinning)
    {
        case true:
            spinner_.start();
            break;

        case false:
            spinner_.stop();
            break;
    }
}

} // namespace Solang
