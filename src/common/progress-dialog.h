/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * progress-dialog.h
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * progress-dialog.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * progress-dialog.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_PROGRESS_DIALOG_H
#define SOLANG_PROGRESS_DIALOG_H

#include <glibmm.h>
#include <gtkmm.h>

#include "types.h"

namespace Solang
{

class ProgressDialog :
    public Gtk::Dialog 
{
    public:
        ProgressDialog(const ProgressObserverPtr &observer) throw();

        virtual
        ~ProgressDialog() throw();

        void
        pulse() throw();

        void
        set_pulse_step(double fraction) throw();

        void 
        set_progress_title(const Glib::ustring & title);

		void 
		reset() throw();

    protected:
        virtual bool
        on_delete_event(GdkEventAny * event);

        virtual void
        on_response(int response_id);

        void
        on_progress() throw();

        ProgressObserverPtr observer_;

        Gtk::Label primaryLabel_;

        Gtk::VBox mainVBox_;

        Gtk::ProgressBar progressBar_;


    private:
};

} // namespace Solang

#endif // SOLANG_PROGRESS_DIALOG_H
