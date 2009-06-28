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

#include <gdkmm.h>

#include "spinner-dialog.h"
#include "types.h"

namespace Solang
{

SpinnerDialog::SpinnerDialog() throw() :
    Gtk::Dialog(),
    hBox_(false, 18),
    spinner_(),
    primaryLabel_("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, false)
{
    configure("", "");
}

SpinnerDialog::SpinnerDialog(const Glib::ustring & title,
                             const Glib::ustring & text) throw() :
    Gtk::Dialog(),
    hBox_(false, 18),
    spinner_(),
    primaryLabel_("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, false)
{
    configure(title, text);
}

SpinnerDialog::~SpinnerDialog() throw()
{
}

void
SpinnerDialog::configure(const Glib::ustring & title,
                         const Glib::ustring & text) throw()
{
    set_title(title);
    set_border_width(12);
    set_default_size(408, 108);
    set_has_separator(false);

    Gtk::VBox * const dialog_vbox = get_vbox();
    dialog_vbox->set_spacing(18);
    dialog_vbox->pack_start(hBox_, Gtk::PACK_EXPAND_WIDGET, 0);

    spinner_.set_size(Gtk::ICON_SIZE_DIALOG);
    hBox_.pack_start(spinner_, Gtk::PACK_SHRINK, 0);

    primaryLabel_.set_markup(text);
    primaryLabel_.set_use_markup(true);
    hBox_.pack_start(primaryLabel_, Gtk::PACK_EXPAND_WIDGET, 0);

    add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_CLOSE);
    set_default_response(Gtk::RESPONSE_CLOSE);

    show_all_children();
}

bool
SpinnerDialog::on_delete_event(GdkEventAny * event)
{
    hide();
    return false;
}

void
SpinnerDialog::on_response(int response_id)
{
    hide();
}

void
SpinnerDialog::set_busy(bool busy) throw()
{
    WindowPtr window = get_window();

    if (0 == window)
    {
        return;
    }

    switch (busy)
    {
        case true:
        {
            Gdk::Cursor cursor(Gdk::WATCH);
            window->set_cursor(cursor);
            spinner_.start();
            break;
        }

        case false:
        {
            window->set_cursor();
            spinner_.stop();
            break;
        }
    }
}

void
SpinnerDialog::set_text(const Glib::ustring & text) throw()
{
    primaryLabel_.set_markup(text);
}

} // namespace Solang
