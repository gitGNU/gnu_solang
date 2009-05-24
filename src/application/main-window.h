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

#ifndef SOLANG_MAIN_WINDOW_H
#define SOLANG_MAIN_WINDOW_H

#include <vector>

#include <gdl/gdl.h>
#include <gtkmm.h>

#include "non-copyable.h"
#include "types.h"

namespace Solang
{

class MainWindow :
    public Gtk::Window,
    public NonCopyable
{
public:
    MainWindow() throw();
    
    virtual
    ~MainWindow() throw();

    void
    init() throw();

    void
    final() throw();

    void
    add_dock_object_left_top(GdlDockObject * dock_object) throw();

    void
    add_dock_object_left_bottom(GdlDockObject * dock_object) throw();

    void
    add_dock_object_center(GdlDockObject * dock_object) throw();

    void
    present_dock_object(GdlDockObject * dock_object) throw();

    Gtk::Notebook *
    get_notebook_center() throw();

    const UIManagerPtr &
    get_ui_manager() throw();

protected:
    void
    on_action_photo_quit() throw();

    void
    on_action_view_full_screen() throw();

    void
    on_action_view_status_bar() throw();

    void
    on_action_view_tool_bar() throw();

    virtual bool
    on_delete_event(GdkEventAny * event);

    ActionGroupPtr actionGroup_;

    UIManagerPtr uiManager_;

    Gtk::UIManager::ui_merge_id uiID_;

    Gtk::VBox vBox_;

    Gtk::HBox hBox_;

    Gtk::Statusbar statusBar_;

    GtkWidget * const dock_;
    
    GtkWidget * const dockBar_;

    GdlDockLayout * const layout_;

    bool showToolBar_;

    bool showStatusBar_;

    std::vector<GdlDockObject *> dockObjectsLeftTop_;

    std::vector<GdlDockObject *> dockObjectsLeftBottom_;

    std::vector<GdlDockObject *> dockObjectsCenter_;

private:
};

} // namespace Solang

#endif // SOLANG_MAIN_WINDOW_H
