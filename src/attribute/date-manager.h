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

#ifndef SOLANG_DATE_MANAGER_H
#define SOLANG_DATE_MANAGER_H

#include <string>

#include <glibmm.h>
#include <gtkmm.h>

#include "plugin.h"
#include "date-view.h"

namespace Solang
{

class Engine;

class DateManager :
    public Plugin
{
    public:
        DateManager() throw();

        virtual
        ~DateManager() throw();

        virtual void
        init(Application & application) throw();

        virtual void
        final(Application & application) throw();

        virtual void
        visit_renderer(BrowserRenderer & browser_renderer) throw();

        virtual void
        visit_renderer(ConsoleRenderer & console_renderer) throw();

        virtual void
        visit_renderer(EditorRenderer & editor_renderer) throw();

        virtual void
        visit_renderer(EnlargedRenderer & enlarged_renderer) throw();

        void
        on_photos_changed() throw();

    protected:
        void
        on_renderer_changed(Engine & engine) throw();

        void
        ui_hide() throw();

        void
        ui_show() throw();

        ApplicationPtr application_;

        Glib::RefPtr<Gtk::ActionGroup> actionGroup_;

        Gtk::UIManager::ui_merge_id uiID_;

        const std::string dockItemName_;

        const Glib::ustring dockItemTitle_;

        GdlDockItemBehavior dockItemBehaviour_;

        GtkWidget * dockItem_;

        Gtk::VBox vBox_;

        Gtk::ScrolledWindow scrolledWindow_;

        DateView dateView_;

        sigc::connection signalRendererChanged_;

    private:
};

} // namespace Solang

#endif // SOLANG_DATE_MANAGER_H
