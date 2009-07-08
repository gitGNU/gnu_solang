/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Santanu Sinha <santanu.sinha@gmail.com>
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

#ifndef SOLANG_EDITOR_H
#define SOLANG_EDITOR_H

#include <glibmm.h>
#include <gtkmm.h>

#include "error.h"
#include "types.h"

namespace Solang
{

class Editor
{
    public:
        Editor();

        ~Editor() throw();

        void
        init( Application &app );

        void
        final( Application &app );

        void
        apply( const EditActionPtr &action);

        void
        apply( const EditActionPtr &action,
                const EditablePhotoPtr &photo ) throw(Error);

        void
        apply( const EditActionPtr &action,
               const EditablePhotoList &photo) throw(Error);

        void
        register_ui() throw();

        void
        unregister_ui() throw();

        void
        save();

        inline Glib::Dispatcher &
        edit_action_performed() throw();

        void
        set_current_photo( const EditablePhotoPtr &photo );

    private:

        void
        set_target_photo( const EditablePhotoPtr &photo );

        void
        on_action_flip_horz() throw();

        void
        on_action_flip_vert() throw();

        void
        on_action_rotate_left() throw();

        void
        on_action_rotate_right() throw();

        void
        on_action_scale() throw();

        void
        on_action_save() throw();

        void
        on_action_undo() throw();

        void
        on_action_redo() throw();

        void
        on_action_copy_actions() throw();

        void
        on_action_paste_actions() throw();

        ApplicationPtr application_;
        EditablePhotoList modifiedPhotos_;
        Glib::Mutex mutex_;
        ActionGroupPtr actionGroup_;
        Glib::RefPtr<Gtk::IconFactory> iconFactory_;
        Gtk::UIManager::ui_merge_id uiID_;
        Glib::Dispatcher actionPerformed_;
        EditablePhotoPtr currentPhoto_;
        EditActionList copiedActions_;
};

inline Glib::Dispatcher &
Editor::edit_action_performed() throw()
{
    return actionPerformed_;
}

} //namespace solang

#endif
