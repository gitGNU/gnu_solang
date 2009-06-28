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

#ifndef SOLANG_FLICKR_AUTH_DIALOG_H
#define SOLANG_FLICKR_AUTH_DIALOG_H

#include <glibmm.h>
#include <gtkmm.h>

namespace Solang
{

class FlickrAuthDialog :
    public Gtk::Dialog
{
    public:
        FlickrAuthDialog(const Glib::ustring & uri) throw();

        virtual
        ~FlickrAuthDialog() throw();

        std::string
        get_frob() const throw();

    protected:
        virtual bool
        on_delete_event(GdkEventAny * event);

        virtual void
        on_response(int response_id);

        void
        on_frob_entry_activate() throw();

        void
        on_frob_entry_changed() throw();

        Gtk::VBox vBox_;

        Gtk::ScrolledWindow scrolledWindow_;

        GtkWidget * webView_;

        Gtk::HBox hBox_;

        Gtk::Label frobLabel_;

        Gtk::Entry frobEntry_;

        Gtk::Button authButton_;

        Gtk::Image authImage_;

    private:
};

} // namespace Solang

#endif // SOLANG_FLICKR_AUTH_DIALOG_H
