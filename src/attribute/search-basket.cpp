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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <list>

#include <glibmm/i18n.h>

#include "application.h"
#include "main-window.h"
#include "search-basket.h"
#include "search-basket-column-record.h"

namespace Solang
{

SearchBasket::SearchBasket() throw() :
    Plugin(),
    sigc::trackable(),
    dockItemName_("search-basket-dock-item"),
    dockItemTitle_("Search"),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NORMAL),
    dockItem_(NULL),
    scrolledWindow_(),
    listStore_(Gtk::ListStore::create(SearchBasketColumnRecord())),
    treeView_(listStore_)
{
    scrolledWindow_.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC);

    SearchBasketColumnRecord model_column_record;
    treeView_.append_column("",
        model_column_record.get_column_pixbuf());
    treeView_.append_column("",
        model_column_record.get_column_description());

    treeView_.set_enable_search(true);
    treeView_.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_NONE);
    treeView_.set_headers_clickable(false);
    treeView_.set_headers_visible(false);

    scrolledWindow_.add(treeView_);

    dockItem_ = gdl_dock_item_new_with_stock(dockItemName_.c_str(),
                                             dockItemTitle_.c_str(),
                                             GTK_STOCK_FIND,
                                             dockItemBehaviour_);
    gtk_container_add(GTK_CONTAINER(dockItem_),
                      GTK_WIDGET(scrolledWindow_.gobj()));

    std::list<Gtk::TargetEntry> targets;
    targets.push_back(Gtk::TargetEntry("STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    targets.push_back(Gtk::TargetEntry("UTF8_STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    treeView_.enable_model_drag_dest(targets, Gdk::ACTION_COPY);

    treeView_.signal_drag_data_received().connect(sigc::mem_fun(
        *this, &SearchBasket::on_drag_data_received));
}

SearchBasket::~SearchBasket() throw()
{
    treeView_.unset_model();
    //g_object_unref(dockItem_);
}

void
SearchBasket::init(Application & application) throw()
{
    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_left(GDL_DOCK_OBJECT(dockItem_));

    initialized_.emit(*this);
}

void
SearchBasket::final(Application & application) throw()
{
    finalized_.emit(*this);
}

void
SearchBasket::on_drag_data_received(
                  const DragContextPtr & drag_context,
                  gint x, gint y,
                  const Gtk::SelectionData & data,
                  guint info, guint time) throw()
{
    Gtk::TreeModel::iterator model_iter = listStore_->append();
    Gtk::TreeModel::Row row = *model_iter;

    SearchBasketColumnRecord model_column_record;
    // row[model_column_record.get_column_pixbuf()] = ;
    row[model_column_record.get_column_description()]
        = data.get_text();

    drag_context->drop_finish(true, 0);
}

} // namespace Solang
