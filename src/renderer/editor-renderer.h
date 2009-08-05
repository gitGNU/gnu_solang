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

#ifndef SOLANG_EDITOR_RENDERER_H
#define SOLANG_EDITOR_RENDERER_H

#include <string>

#include <gdl/gdl.h>
#include <glibmm.h>
#include <gtkmm.h>
#include <sigc++/sigc++.h>

#include "editor.h"
#include "i-renderer.h"
#include "thumbnail-view.h"
#include "types.h"

namespace Solang
{

class EditorRenderer :
    public IRenderer,
    public sigc::trackable
{
    private:
        class EditablePhotoColumns: public Gtk::TreeModel::ColumnRecord
        {
            public:
                EditablePhotoColumns()
                {
                    add( buffer_ );
                    add( photo_ );
                }

                Gtk::TreeModelColumn<PixbufPtr> buffer_;
                Gtk::TreeModelColumn<EditablePhotoPtr> photo_;
        };

    public:
        EditorRenderer() throw();

        virtual
        ~EditorRenderer() throw();

        virtual void
        init(Application & application) throw();

        virtual void
        render(const PhotoPtr & photo) throw();

        virtual void
        render(const PhotoList & photos) throw();

        virtual void
        final(Application & application) throw();

        void
        on_init_end(Application & application) throw();

        void
        on_scroll_event(GdkScrollDirection direction) throw();

        virtual PhotoList
        get_current_selection() throw();

        virtual std::string
        get_name() const throw();

        virtual void
        receive_plugin(IPlugin & plugin) throw();

    protected:

        virtual void
        renderPhotoList(const PhotoList & photos) throw();

        virtual void
        renderSelectedPhotos(const EditablePhotoList & photos) throw();

        virtual void
        render(const EditablePhotoPtr & photo) throw();

        void
        refresh_image() throw();

        void
        on_photo_selection_changed() throw();

        void
        on_photo_activated( const Gtk::TreeModel::Path& path) throw();

        void
        on_action_go_previous() throw();

        void
        on_action_go_next() throw();

        void
        on_action_go_first() throw();

        void
        on_action_go_last() throw();

        void
        on_action_view_reload() throw();

        void
        on_action_view_best_fit() throw();

        void
        on_action_view_normal_size() throw();

        void
        on_action_view_zoom_in() throw();

        void
        on_action_view_zoom_out() throw();

        void
        on_switch_page(GtkNotebookPage * notebook_page, guint page_num)
                       throw();

        ApplicationPtr application_;

        Glib::RefPtr<Gtk::IconFactory> iconFactory_;

        ActionGroupPtr actionGroup_;

        Gtk::UIManager::ui_merge_id uiID_;

        const std::string dockItemName_;

        const Glib::ustring dockItemTitle_;

        GdlDockItemBehavior dockItemBehaviour_;

        Editor editor_;

        GtkWidget * dockItem_;

        Gtk::VPaned vPane_;

        GtkWidget * imageView_;

        GtkWidget * imageScrollWin_;

        Gtk::HBox imagesArea_;

        Gtk::Button btPrev_;

        Gtk::Arrow left_;

        Gtk::ScrolledWindow iconWindow_;

        Gtk::IconView editablePhotosView_;

        Gtk::CellRendererPixbuf iconRenderer_;

        Gtk::Button btNext_;

        Gtk::Arrow right_;

        EditablePhotoColumns columns_;

        Glib::RefPtr<Gtk::ListStore> photosModel_;

        Gtk::TreeModel::iterator currentItem_;

        gint pageNum_;

        sigc::connection signalInitEnd_;

        sigc::connection signalItemActivated_;

        sigc::connection signalSwitchPage_;

        sigc::connection signalPhotoSelected_;

};

} // namespace Solang

#endif // SOLANG_EDITOR_RENDERER_H
