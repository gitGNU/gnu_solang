/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * progress-dialog.cpp
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * progress-dialog.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * progress-dialog.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <sstream>

#include "progress-dialog.h"
#include "progress-observer.h"

namespace Solang
{

ProgressDialog::ProgressDialog(const ProgressObserverPtr & observer)
    throw() :
    Gtk::Dialog(),
    observer_(observer),
    primaryLabel_("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, false),
    mainVBox_(false, 3),
    progressBar_()
{
    set_border_width(12);
    set_default_size(408, 108);
    set_has_separator(false);

    Gtk::VBox * const dialog_vbox = get_vbox();
    dialog_vbox->set_spacing(12);

    primaryLabel_.set_use_markup(true);
    dialog_vbox->pack_start(primaryLabel_, Gtk::PACK_SHRINK, 0);

    dialog_vbox->pack_start(mainVBox_, Gtk::PACK_EXPAND_WIDGET, 0);

    progressBar_.set_orientation(Gtk::PROGRESS_LEFT_TO_RIGHT);
    mainVBox_.pack_start(progressBar_, Gtk::PACK_SHRINK, 0);

    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

    show_all_children();

    observer_->progress().connect(sigc::mem_fun(*this,
        &ProgressDialog::on_progress));
}

ProgressDialog::~ProgressDialog() throw()
{
}

void
ProgressDialog::set_progress_title(const Glib::ustring & title)
{
    set_title(observer_->get_event_description());
    primaryLabel_.set_markup("<b><big>" + title + "</big></b>");
}

void
ProgressDialog::pulse() throw()
{
    progressBar_.pulse();
}

void
ProgressDialog::set_pulse_step(double fraction) throw()
{
    progressBar_.set_pulse_step(fraction);
}

bool
ProgressDialog::on_delete_event(GdkEventAny * event)
{
    hide();
    return false;    
}

void
ProgressDialog::on_progress() throw()
{
    std::ostringstream sout;

    sout << observer_->get_current_events() << " of "
         << observer_->get_num_events() << " photos imported";

    progressBar_.set_text(sout.str());
    progressBar_.set_fraction(
        static_cast<double>(observer_->get_current_events())
        / static_cast<double>(observer_->get_num_events()));
}

void
ProgressDialog::on_response(int response_id)
{
    observer_->set_stop(true);
    hide();
}

void
ProgressDialog::reset() throw()
{
    progressBar_.set_fraction(0.0);
    set_progress_title("");
}

} // namespace Solang
