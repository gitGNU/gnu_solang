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

#ifndef SOLANG_IMPORTER_DIALOG_H
#define SOLANG_IMPORTER_DIALOG_H

#include <gtkmm.h>

#include "tag-view.h"
#include "types.h"

namespace Solang
{

class ImporterDialog :
    public Gtk::Dialog 
{
    public:
        ImporterDialog(Gtk::Widget & browser, const TagList & tags)
                       throw();

        virtual
        ~ImporterDialog() throw();

        inline const TagView &
        get_tag_view() const throw()
        {
            return tagView_;
        }

        inline bool
        get_to_copy()
        {
            return copyCheckButton_.get_active();
        }

    protected:
        virtual bool
        on_delete_event(GdkEventAny * event);

        virtual void
        on_response(int response_id);

        Gtk::HBox mainHBox_;

        Gtk::VBox inputVBox_;

        //Gtk::ScrolledWindow previewScrolledWindow_;

        Gtk::VBox generalVBox_;

        Gtk::VBox tagsVBox_;

        Gtk::VBox optionsVBox_;

        //Gtk::IconView previewIconView_;

        Gtk::Label generalLabel_;

        Gtk::Alignment generalAlignment_;

        Gtk::Label tagsLabel_;

        Gtk::Alignment tagsAlignment_;

        Gtk::Label optionsLabel_;

        Gtk::Alignment optionsAlignment_;

        Gtk::HBox generalHBox_;

        Gtk::ScrolledWindow tagsScrolledWindow_;

        Gtk::VBox optionsVBox2_;

        Gtk::Label sourceLabel_;

        TagView tagView_;

        Gtk::CheckButton duplicatesCheckButton_;

        Gtk::CheckButton copyCheckButton_;

        Gtk::CheckButton subFoldersCheckButton_;

        Gtk::Button importButton_;

        Gtk::Image importImage_;

    private:
};

} // namespace Solang

#endif // SOLANG_IMPORTER_DIALOG_H
