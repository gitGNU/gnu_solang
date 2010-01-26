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

#ifndef SOLANG_SEARCH_MANAGER_H
#define SOLANG_SEARCH_MANAGER_H

#include <string>

#include <gdl/gdl.h>
#include <glibmm.h>
#include <gtkmm.h>
#include <sigc++/sigc++.h>

#include "plugin.h"
#include "search-basket.h"
#include "search-criterion-source.h"

namespace Solang
{

class Application;
class RendererRegistry;

class SearchManager :
    public Plugin,
    public sigc::trackable,
    public SearchCriterionSource
{
    public:
        SearchManager() throw();

        virtual
        ~SearchManager() throw();

        virtual void
        init(Application & application) throw();

        virtual void
        final(Application & application) throw();

        virtual void
        visit_renderer(BrowserRenderer & browser_renderer) throw();

        virtual void
        visit_renderer(EnlargedRenderer & enlarged_renderer) throw();

        virtual void
        visit_renderer(SlideshowRenderer & slideshow_renderer) throw();

        bool
        add_item_to_list( const Glib::ustring &key );

        virtual void
        get_criterion(PhotoSearchCriteriaList &) const throw();

    protected:
        void
        remove_selected();

        void
        apply_criterion();

        void
        on_action_select_all() throw();

        void
        on_action_remove_selected() throw();

        void
        on_drag_data_received(const DragContextPtr & drag_context,
                              gint x, gint y,
                              const Gtk::SelectionData & data,
                              guint info, guint time) throw();

        void
        on_renderer_changed(RendererRegistry & renderer_registry)
                            throw();

        void
        on_selection_changed() throw();

        void
        ui_hide() throw();

        void
        ui_show() throw();

        ActionGroupPtr actionGroup_;

        Gtk::UIManager::ui_merge_id uiIDBasket_;

        const std::string dockItemName_;

        const Glib::ustring dockItemTitle_;

        GdlDockItemBehavior dockItemBehaviour_;

        GtkWidget * dockItem_;

        Gtk::VBox vBox_;

        Gtk::ScrolledWindow scrolledWindow_;

        ListStorePtr listStore_;

        SearchBasket searchBasket_;

        ApplicationPtr application_;

        sigc::connection signalRendererChanged_;

    private:
        static const std::string uiFile_;
};

} // namespace Solang

#endif // SOLANG_SEARCH_MANAGER_H
