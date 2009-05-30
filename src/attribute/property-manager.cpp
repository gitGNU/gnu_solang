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

#include <glibmm/i18n.h>

#include "application.h"
#include "main-window.h"
#include "property-manager.h"
#include "renderer.h"

namespace Solang
{

static const std::string uiFile
    = PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/ui/"
          PACKAGE_TARNAME"-property-manager.ui";

PropertyManager::PropertyManager() throw() :
    Plugin(),
    application_(),
    actionGroup_(Gtk::ActionGroup::create()),
    uiID_( 1 ),
    dockItemName_("property-managers-dock-item"),
    dockItemTitle_(_("Properties")),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NORMAL),
    dockItem_(NULL),
    noteBook_(),
    vBox_(false, 6),
    basicInfo_(),
    basicExifView_()
{
    dockItem_ = gdl_dock_item_new_with_stock(dockItemName_.c_str(),
                                             dockItemTitle_.c_str(),
                                             "gtk-properties",
                                             dockItemBehaviour_);
    gtk_container_add(GTK_CONTAINER(dockItem_),
                      GTK_WIDGET(noteBook_.gobj()));

    basicInfo_.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC);
    basicInfo_.add(basicExifView_);

    noteBook_.append_page( vBox_, _("Basic") );
    vBox_.pack_start( basicInfo_, Gtk::PACK_EXPAND_WIDGET,0 );

}

PropertyManager::~PropertyManager() throw()
{
    //g_object_unref(dockItem_);
}

void
PropertyManager::init(Application & application)
    throw()
{
    application_ = &application;
    basicExifView_.set_application( application_ );
    Engine & engine = application.get_engine();
    engine.selection_changed().connect(
            sigc::mem_fun(*this,
                    &PropertyManager::on_selection_changed ) );

    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_left_top(GDL_DOCK_OBJECT(dockItem_));

    initialized_.emit(*this);
}

void
PropertyManager::final(Application & application)
    throw()
{
    finalized_.emit(*this);
}

void
PropertyManager::on_selection_changed() throw()
{
    PhotoList photos = application_->get_engine().get_current_renderer()->get_current_selection();

    if( photos.empty() )
        return;

    basicExifView_.populate( (*photos.begin())->get_exif_data() );

    return;
}

} // namespace Solang
