/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_BROWSER_RENDERER_H
#define SOLANG_BROWSER_RENDERER_H

#include <string>

#include <gdl/gdl.h>
#include <glibmm.h>
#include <gtkmm.h>
#include <sigc++/sigc++.h>

#include "i-renderer.h"
#include "pagination-bar.h"
#include "thumbnail-view.h"
#include "types.h"
#include "zoomer.h"

namespace Solang
{

class BrowserRenderer :
    public IRenderer,
    public sigc::trackable
{
    public:
        BrowserRenderer() throw();

        virtual
        ~BrowserRenderer() throw();

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
        on_selection_changed() throw();

        virtual PhotoList
        get_current_selection() throw();

        virtual void
        present() throw();

        virtual void
        receive_plugin(IPlugin & plugin) throw();

        virtual IRendererPtr
        receive_selector(IRendererSelector & selector,
                         const IRendererPtr & renderer) throw();

    protected:
        void
        clear_thumbnails() throw();

        void
        generate_thumbnails() throw();

        void
        on_action_view_slideshow() throw();

        void
        on_item_activated(const Gtk::TreeModel::Path & path) throw();

        void
        on_limits_changed() throw();

        void
        on_list_store_change_begin(Application & application) throw();

        void
        on_list_store_change_end(Application & application) throw();

        void
        on_signal_value_changed() throw();

        void
        on_switch_page(GtkNotebookPage * notebook_page, guint page_num)
                       throw();

        bool
        on_visible(const Gtk::TreeModel::const_iterator & iter)
                   throw();

        void
        on_item_edit() throw();

        void
        reload() throw();

        ApplicationPtr application_;

        IconFactoryPtr iconFactory_;

        ActionGroupPtr actionGroup_;

        Gtk::UIManager::ui_merge_id uiID_;

        Gtk::UIManager::ui_merge_id uiIDThumbnail_;

        const std::string dockItemName_;
    
        const Glib::ustring dockItemTitle_;
    
        GdlDockItemBehavior dockItemBehaviour_;

        GtkWidget * dockItem_;

        Gtk::VBox vBox_;

        Gtk::HBox hBox_;

        PaginationBar paginationBar_;

        Gtk::Label dummyLabel_;

        Zoomer zoomer_;
    
        Gtk::ScrolledWindow scrolledWindow_;

        TreeModelFilterPtr treeModelFilter_;

        ThumbnailView thumbnailView_;

        gint pageNum_;

        sigc::connection signalInitEnd_;

        sigc::connection signalItemActivated_;

        sigc::connection signalListStoreChangeBegin_;

        sigc::connection signalListStoreChangeEnd_;

        sigc::connection signalSelectionChanged_;

        sigc::connection signalSwitchPage_;

    private:
};

} // namespace Solang

#endif // SOLANG_BROWSER_RENDERER_H
