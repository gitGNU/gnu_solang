/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * property-manager.cpp
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * exif-manager.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * exif-manager.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "application.h"
#include "main-window.h"
#include "property-manager.h"

namespace Solang
{

PropertyManager::PropertyManager() throw() :
    Plugin(),
    dockItemName_("property-managers-dock-item"),
    dockItemTitle_("Properties"),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NORMAL),
    dockItem_(NULL),
    noteBook_()
{
	dockItem_ = gdl_dock_item_new_with_stock(dockItemName_.c_str(),
                                             dockItemTitle_.c_str(),
                                             "gtk-properties",
                                             dockItemBehaviour_);
	gtk_container_add(GTK_CONTAINER(dockItem_),
                      GTK_WIDGET(noteBook_.gobj()));
}

PropertyManager::~PropertyManager() throw()
{
	//g_object_unref(dockItem_);
}

void
PropertyManager::init(Application & application)
    throw()
{
    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_left(GDL_DOCK_OBJECT(dockItem_));
    initialized_.emit(*this);
}

void
PropertyManager::final(Application & application)
    throw()
{
    finalized_.emit(*this);
}

} // namespace Solang
