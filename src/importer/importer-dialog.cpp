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

#include "importer-dialog.h"
#include "photo-source-enums.h"

namespace Solang
{

ImporterDialog::ImporterDialog(Gtk::Widget & browser,
                               const TagList & tags,
                               gint source_options) throw() :
    Gtk::Dialog(_("Import"), true, false),
    generalVBox_(false, 6),
    tagsVBox_(false, 6),
    optionsVBox_(false, 6),
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
    copyCheckButton_((source_options
                      & PHOTO_SOURCE_COPY_PHOTOS_OPTIONAL)
                      ? Gtk::manage(new Gtk::CheckButton(
                            _("_Copy files to the Pictures folder"),
                            true))
                      : 0),
    subFoldersCheckButton_((
        source_options & PHOTO_SOURCE_INCLUDE_SUBFOLDERS_OPTIONAL)
        ? Gtk::manage(new Gtk::CheckButton(
                          _("Include su_b-folders"),
                          true))
        : 0),
    importButton_(),
    importImage_(Gtk::Stock::APPLY, Gtk::ICON_SIZE_BUTTON),
    sourceOptions_(source_options)
{
    set_border_width(12);
    set_default_size(300, 400);

    Gtk::VBox * const dialog_vbox = get_vbox();
    dialog_vbox->set_spacing(18);

    dialog_vbox->pack_start(generalVBox_, Gtk::PACK_SHRINK, 0);
    dialog_vbox->pack_start(tagsVBox_, Gtk::PACK_EXPAND_WIDGET, 0);
    dialog_vbox->pack_start(optionsVBox_, Gtk::PACK_SHRINK, 0);

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
    tagsScrolledWindow_.set_shadow_type(Gtk::SHADOW_IN);
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

    if (0 != copyCheckButton_)
    {
        copyCheckButton_->set_use_underline(true);
        optionsVBox2_.pack_start(*copyCheckButton_,
                                 Gtk::PACK_SHRINK, 0);
    }

    if (0 != subFoldersCheckButton_)
    {
        subFoldersCheckButton_->set_use_underline(true);
        optionsVBox2_.pack_start(*subFoldersCheckButton_,
                                 Gtk::PACK_SHRINK, 0);
    }

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
ImporterDialog::get_copy_photos() const throw()
{
    return (sourceOptions_ & PHOTO_SOURCE_COPY_PHOTOS_OPTIONAL)
           ? copyCheckButton_->get_active()
           : sourceOptions_ & PHOTO_SOURCE_COPY_PHOTOS_TRUE;
}

bool
ImporterDialog::get_include_subfolders() const throw()
{
    return (sourceOptions_ & PHOTO_SOURCE_INCLUDE_SUBFOLDERS_OPTIONAL)
           ? subFoldersCheckButton_->get_active()
           : sourceOptions_ & PHOTO_SOURCE_INCLUDE_SUBFOLDERS_TRUE;
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
