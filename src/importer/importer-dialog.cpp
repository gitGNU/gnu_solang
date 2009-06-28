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

#include <glibmm/i18n.h>

#include "importer-dialog.h"

namespace Solang
{

ImporterDialog::ImporterDialog(Gtk::Widget & browser,
                               const TagList & tags) throw() :
    Gtk::Dialog(_("Import"), true, false),
    mainHBox_(false, 18),
    inputVBox_(false, 18),
    //previewScrolledWindow_(),
    generalVBox_(false, 6),
    tagsVBox_(false, 6),
    optionsVBox_(false, 6),
    //previewIconView_(),
    generalLabel_("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, false),
    generalAlignment_(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER, 1.0, 1.0),
    tagsLabel_("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, false),
    tagsAlignment_(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER, 1.0, 1.0),
    optionsLabel_("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, false),
    optionsAlignment_(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER, 1.0, 1.0),
    generalHBox_(false, 12),
    tagsScrolledWindow_(),
    optionsVBox2_(false, 6),
    sourceLabel_(_("_Source:"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, true),
    tagView_(tags),
    duplicatesCheckButton_(_("_Detect duplicates"), true),
    copyCheckButton_(_("_Copy files to the Pictures folder"), true),
    subFoldersCheckButton_(_("Include su_b-folders"), true),
    importButton_(),
    importImage_(Gtk::Stock::APPLY, Gtk::ICON_SIZE_BUTTON)
{
    set_border_width(12);
    set_default_size(300, 400);

    Gtk::VBox * const dialog_vbox = get_vbox();
    dialog_vbox->set_spacing(18);
    dialog_vbox->pack_start(mainHBox_, Gtk::PACK_EXPAND_WIDGET, 0);

    mainHBox_.pack_start(inputVBox_, Gtk::PACK_EXPAND_WIDGET, 0);

    //previewScrolledWindow_.set_policy(Gtk::POLICY_AUTOMATIC,
    //                                  Gtk::POLICY_AUTOMATIC);
    //mainHBox_.pack_start(previewScrolledWindow_,
    //                     Gtk::PACK_EXPAND_WIDGET, 0);

    inputVBox_.pack_start(generalVBox_, Gtk::PACK_SHRINK, 0);
    inputVBox_.pack_start(tagsVBox_, Gtk::PACK_EXPAND_WIDGET, 0);
    inputVBox_.pack_start(optionsVBox_, Gtk::PACK_SHRINK, 0);

    //previewScrolledWindow_.add(previewIconView_);

    generalLabel_.set_use_markup(true);
    {
        Glib::ustring markup = "<b>";
        markup += _("General");
        markup += "</b>";
        generalLabel_.set_markup(markup);
    }
    generalVBox_.pack_start(generalLabel_, Gtk::PACK_SHRINK, 0);

    generalAlignment_.set_padding(0, 0, 12, 0);
    generalVBox_.pack_start(generalAlignment_, Gtk::PACK_SHRINK, 0);

    tagsLabel_.set_use_markup(true);
    {
        Glib::ustring markup = "<b>";
        markup += _("Tags");
        markup += "</b>";
        tagsLabel_.set_markup(markup);
    }
    tagsVBox_.pack_start(tagsLabel_, Gtk::PACK_SHRINK, 0);

    tagsAlignment_.set_padding(0, 0, 12, 0);
    tagsVBox_.pack_start(tagsAlignment_, Gtk::PACK_EXPAND_WIDGET, 0);

    optionsLabel_.set_use_markup(true);
    {
        Glib::ustring markup = "<b>";
        markup += _("Options");
        markup += "</b>";
        optionsLabel_.set_markup(markup);
    }
    optionsVBox_.pack_start(optionsLabel_, Gtk::PACK_SHRINK, 0);

    optionsAlignment_.set_padding(0, 0, 12, 0);
    optionsVBox_.pack_start(optionsAlignment_, Gtk::PACK_SHRINK, 0);

    generalAlignment_.add(generalHBox_);

    tagsScrolledWindow_.set_policy(Gtk::POLICY_AUTOMATIC,
                                   Gtk::POLICY_AUTOMATIC);
    tagsAlignment_.add(tagsScrolledWindow_);

    optionsAlignment_.add(optionsVBox2_);

    sourceLabel_.set_use_underline(true);
    generalHBox_.pack_start(sourceLabel_, Gtk::PACK_SHRINK, 0);

    // Insert a Gtk::Widget specific to the particular source.
    generalHBox_.pack_start(browser, Gtk::PACK_EXPAND_WIDGET, 0);

    tagsScrolledWindow_.add(tagView_);

    duplicatesCheckButton_.set_use_underline(true);
    optionsVBox2_.pack_start(duplicatesCheckButton_,
                             Gtk::PACK_SHRINK, 0);

    copyCheckButton_.set_use_underline(true);
    optionsVBox2_.pack_start(copyCheckButton_, Gtk::PACK_SHRINK, 0);

    subFoldersCheckButton_.set_use_underline(true);
    optionsVBox2_.pack_start(subFoldersCheckButton_,
                             Gtk::PACK_SHRINK, 0);

    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

    importButton_.set_image(importImage_);
    importButton_.set_use_underline(true);
    importButton_.set_label(_("_Import"));
    add_action_widget(importButton_, Gtk::RESPONSE_APPLY);

    show_all_children();
}

ImporterDialog::~ImporterDialog() throw()
{
}

bool
ImporterDialog::on_delete_event(GdkEventAny * event)
{
    hide();
    return false;	
}

void
ImporterDialog::on_response(int response_id)
{
    hide();
}

} // namespace Solang
