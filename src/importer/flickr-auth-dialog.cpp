/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009, 2010 Debarshi Ray <rishi@gnu.org>
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

#include <glibmm/i18n.h>
#include <webkit/webkit.h>

#include "flickr-auth-dialog.h"

namespace Solang
{

FlickrAuthDialog::FlickrAuthDialog(const Glib::ustring & uri)
                                   throw() :
    Gtk::Dialog(),
    vBox_(false, 6),
    scrolledWindow_(),
    webView_(webkit_web_view_new()),
    hBox_(false, 12),
    frobLabel_(_("_Nine digit code:"), Gtk::ALIGN_LEFT,
               Gtk::ALIGN_CENTER, true),
    frobEntry_(),
    authButton_(),
    authImage_(Gtk::Stock::OK, Gtk::ICON_SIZE_BUTTON)
{
    set_border_width(12);
    set_default_size(800, 600);
    set_has_separator(false);

    Gtk::VBox * const dialog_vbox = get_vbox();
    dialog_vbox->set_spacing(18);
    dialog_vbox->pack_start(vBox_, Gtk::PACK_EXPAND_WIDGET, 0);

    scrolledWindow_.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC);
    scrolledWindow_.set_shadow_type(Gtk::SHADOW_IN);

    scrolledWindow_.add(*Glib::wrap(webView_, false));
    g_object_ref(webView_);

    vBox_.pack_start(scrolledWindow_, Gtk::PACK_EXPAND_WIDGET, 0);

    hBox_.pack_start(frobLabel_, Gtk::PACK_SHRINK, 0);
    hBox_.pack_start(frobEntry_, Gtk::PACK_SHRINK, 0);
    vBox_.pack_start(hBox_, Gtk::PACK_SHRINK, 0);

    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

    authButton_.set_image(authImage_);
    authButton_.set_use_underline(true);
    authButton_.set_label(_("_Authorize"));
    add_action_widget(authButton_, Gtk::RESPONSE_OK);

    set_response_sensitive(Gtk::RESPONSE_OK, false);

    webkit_web_view_open(WEBKIT_WEB_VIEW(webView_), uri.c_str());

    frobEntry_.signal_activate().connect(
        sigc::mem_fun(*this,
                      &FlickrAuthDialog::on_frob_entry_activate));

    frobEntry_.signal_changed().connect(
        sigc::mem_fun(*this,
                      &FlickrAuthDialog::on_frob_entry_changed));

    show_all_children();
}

FlickrAuthDialog::~FlickrAuthDialog() throw()
{
}

std::string
FlickrAuthDialog::get_frob() const throw()
{
    return frobEntry_.get_text();
}

bool
FlickrAuthDialog::on_delete_event(GdkEventAny * event)
{
    hide();
    return false;
}

void
FlickrAuthDialog::on_frob_entry_activate() throw()
{
    if (11 == frobEntry_.get_text_length())
    {
        authButton_.clicked();
    }
}

void
FlickrAuthDialog::on_frob_entry_changed() throw()
{
    if (11 == frobEntry_.get_text_length())
    {
        set_response_sensitive(Gtk::RESPONSE_OK, true);
    }
    else
    {
        set_response_sensitive(Gtk::RESPONSE_OK, false);
    }
}

void
FlickrAuthDialog::on_response(int response_id)
{
    hide();
}

} // namespace Solang
