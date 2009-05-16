/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_TAG_MANAGER_H
#define SOLANG_TAG_MANAGER_H

#include <string>

#include <gdl/gdl.h>
#include <glibmm.h>
#include <gtkmm.h>

#include "plugin.h"
#include "tag-view.h"

namespace Solang
{

class Application;

class TagManager :
    public Plugin
{
public:
    TagManager() throw();

    virtual
    ~TagManager() throw();

    virtual void
    init(Application & application) throw();

    virtual void
    final(Application & application) throw();

    void
    apply_selected_tags() throw();

    void
    clear_tag_selection() throw();

protected:
    void
    on_action_tag_new() throw();

    ApplicationPtr application_;

    Glib::RefPtr<Gtk::IconFactory> iconFactory_;

    Glib::RefPtr<Gtk::ActionGroup> actionGroup_;

    Gtk::UIManager::ui_merge_id uiID_;

    const std::string dockItemName_;
    
    const Glib::ustring dockItemTitle_;
    
    GdlDockItemBehavior dockItemBehaviour_;

    GtkWidget * dockItem_;

    Gtk::VBox vBox_;
	
    Gtk::ScrolledWindow scrolledWindow_;

    Gtk::HBox hBox_;

    Gtk::Button applyButton_;

    Gtk::Button clearButton_;

    TagView tagView_;

private:
};

} // namespace Solang

#endif // SOLANG_TAG_MANAGER_H
