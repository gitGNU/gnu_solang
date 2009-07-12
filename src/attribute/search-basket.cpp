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

#include <vector>

#include <glibmm/i18n.h>
#include <gdkmm.h>

#include "application.h"
#include "browser-renderer.h"
#include "console-renderer.h"
#include "editor-renderer.h"
#include "engine.h"
#include "enlarged-renderer.h"
#include "main-window.h"
#include "search-basket.h"
#include "search-basket-column-record.h"

namespace Solang
{

SearchBasket::SearchBasket() throw() :
    Plugin(),
    sigc::trackable(),
    dockItemName_("search-basket-dock-item"),
    dockItemTitle_(_("Search")),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NORMAL),
    dockItem_(NULL),
    vBox_( false, 6 ),
    autoApplyCheck_(_("Auto")),
    hBox_( false, 2 ),
    applyButton_(),
    applyImage_( Gtk::Stock::APPLY, Gtk::ICON_SIZE_SMALL_TOOLBAR ),
    clearButton_(),
    clearImage_( Gtk::Stock::CLEAR, Gtk::ICON_SIZE_SMALL_TOOLBAR ),
    trashButton_(), //Container
    trashImage_( Gtk::Stock::DELETE, Gtk::ICON_SIZE_SMALL_TOOLBAR ),
    scrolledWindow_(),
    listStore_(Gtk::ListStore::create(SearchBasketColumnRecord())),
    treeView_(listStore_),
    application_( NULL ),
    signalRendererChanged_()
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
    SearchBasketColumnRecord tmp;
    treeView_.set_tooltip_column( tmp.get_column_description_num() );

    scrolledWindow_.add(treeView_);

    dockItem_ = gdl_dock_item_new_with_stock(dockItemName_.c_str(),
                                             dockItemTitle_.c_str(),
                                             GTK_STOCK_FIND,
                                             dockItemBehaviour_);
    gtk_container_add(GTK_CONTAINER(dockItem_),
                      GTK_WIDGET(vBox_.gobj()));

    vBox_.pack_start( hBox_, Gtk::PACK_SHRINK, 0 );
    autoApplyCheck_.set_active( true );
    hBox_.pack_start( autoApplyCheck_, Gtk::PACK_SHRINK, 0 );
    hBox_.pack_start( applyButton_, Gtk::PACK_SHRINK, 0 );
    hBox_.pack_start( clearButton_, Gtk::PACK_SHRINK, 0 );
    hBox_.pack_start( trashButton_, Gtk::PACK_SHRINK, 0 );
    vBox_.pack_start( scrolledWindow_, Gtk::PACK_EXPAND_WIDGET,0 );
    applyButton_.set_size_request( 32, 32 );
    applyButton_.add( applyImage_ );
    applyButton_.set_relief( Gtk::RELIEF_HALF);
    applyButton_.signal_clicked().connect(
                    sigc::mem_fun(
                        *this, &SearchBasket::apply_criterion));
    applyButton_.set_tooltip_text(_("Apply filters"));
    clearButton_.set_size_request( 32, 32 );
    clearButton_.add( clearImage_ );
    clearButton_.set_relief( Gtk::RELIEF_HALF);
    clearButton_.signal_clicked().connect(
                    sigc::mem_fun(
                        *this, &SearchBasket::clear_criterion));
    clearButton_.set_tooltip_text(_("Clear filters"));
    trashButton_.set_size_request( 32, 32 );
    trashButton_.add( trashImage_ );
    trashButton_.set_relief( Gtk::RELIEF_HALF);
    trashButton_.signal_clicked().connect(
                    sigc::mem_fun(
                        *this, &SearchBasket::remove_selected));
    trashButton_.set_tooltip_text(_("Clear selected filter"));


    std::vector<Gtk::TargetEntry> targets;
    targets.push_back(Gtk::TargetEntry("STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    targets.push_back(Gtk::TargetEntry("UTF8_STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    treeView_.enable_model_drag_dest(targets, Gdk::ACTION_COPY);

    treeView_.signal_drag_data_received().connect_notify(
        sigc::mem_fun(
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
    application_ = &application;
    Engine & engine = application.get_engine();
    engine.get_criterion_repo().register_source( this );

    signalRendererChanged_
        = engine.renderer_changed().connect(
              sigc::mem_fun(*this,
                            &SearchBasket::on_renderer_changed));

    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_left_bottom(
                    GDL_DOCK_OBJECT(dockItem_));

    initialized_.emit(*this);
}

void
SearchBasket::final(Application & application) throw()
{
    signalRendererChanged_.disconnect();

    finalized_.emit(*this);
}

void
SearchBasket::visit_renderer(BrowserRenderer & browser_renderer)
                             throw()
{
    ui_show();
}

void
SearchBasket::visit_renderer(ConsoleRenderer & console_renderer)
                             throw()
{
    ui_hide();
}

void
SearchBasket::visit_renderer(EditorRenderer & editor_renderer) throw()
{
    ui_hide();
}

void
SearchBasket::visit_renderer(EnlargedRenderer & enlarged_renderer)
                             throw()
{
    ui_show();
}

void
SearchBasket::on_drag_data_received(
                  const DragContextPtr & drag_context,
                  gint x, gint y,
                  const Gtk::SelectionData & data,
                  guint info, guint time) throw()
{
    Glib::ustring key = data.get_data_as_string();
    bool status = add_item_to_list( key );
    drag_context->drag_finish(status, false, 0);
    return;
}

void
SearchBasket::on_renderer_changed(Engine & engine) throw()
{
    if (false == gdl_dock_object_is_bound(GDL_DOCK_OBJECT(dockItem_)))
    {
        return;
    }

    const RendererPtr & renderer = engine.get_current_renderer();
    renderer->receive_plugin(*this);
}

bool
SearchBasket::add_item_to_list( const Glib::ustring &key )
{
    DragDropCriteriaMap &dragItemMap
                    = application_->get_drag_drop_map();
    DragDropCriteriaMap::iterator it = dragItemMap.find( key );

    if( it == dragItemMap.end() )
        return false;

    SearchBasketColumnRecord model_column_record;

    PhotoSearchCriteriaPtr criteria = (*it).second;
    Glib::ustring iconPath = criteria->get_criteria_icon_path();
    Glib::ustring description
                    = criteria->get_criteria_description();

    //Check for duplicate
    Gtk::TreeModel::Children children = listStore_->children();
    for( Gtk::TreeModel::const_iterator current = children.begin();
                                current != children.end(); current++)
    {
        Gtk::TreeModel::Row row = (*current);
        PhotoSearchCriteriaPtr tag
                = row[ model_column_record.get_column_criteria()];

        if( tag->get_criteria_description() == description )
        {
            return false;
        }
    }

    //Add
    Gtk::TreeModel::iterator model_iter = listStore_->append();
    Gtk::TreeModel::Row row = *model_iter;
    PixbufPtr icon = Gdk::Pixbuf::create_from_file( iconPath );

    row[ model_column_record.get_column_pixbuf() ] = icon;
    row[ model_column_record.get_column_description() ] = description;
    row[ model_column_record.get_column_criteria() ] = criteria;

    //remove this entry from the map
    dragItemMap.erase( it );

    if( true == autoApplyCheck_.get_active() )
    {
        apply_criterion();
    }

    return true;
}

void
SearchBasket::apply_criterion()
{
    Engine & engine = application_->get_engine();
    engine.criterion_changed().emit();
    return;

}

void
SearchBasket::clear_criterion()
{
    listStore_->clear();
    apply_criterion();
    return;

}

void
SearchBasket::remove_selected()
{
    Glib::RefPtr<Gtk::TreeSelection> selected
                                        = treeView_.get_selection();

    if( 0 == selected->count_selected_rows() )
        return;

    Gtk::TreeModel::iterator item = selected->get_selected();
    listStore_->erase( item );

    if( true == autoApplyCheck_.get_active() )
    {
        apply_criterion();
    }

    return;
}

void
SearchBasket::get_criterion(
                    PhotoSearchCriteriaList &criterion) const throw()
{
    SearchBasketColumnRecord model_column_record;
    Gtk::TreeModel::Children children = listStore_->children();
    for( Gtk::TreeModel::const_iterator current = children.begin();
                                current != children.end(); current++)
    {
        Gtk::TreeModel::Row row = (*current);
        PhotoSearchCriteriaPtr tag
                = row[ model_column_record.get_column_criteria()];
        criterion.push_back( tag );
    }
    return;
}

void
SearchBasket::ui_hide() throw()
{
    GdlDockItem * const dock_item = GDL_DOCK_ITEM(dockItem_);

    if (true == GDL_DOCK_OBJECT_ATTACHED(dock_item))
    {
        gdl_dock_item_hide_item(GDL_DOCK_ITEM(dockItem_));
    }
}

void
SearchBasket::ui_show() throw()
{
    GdlDockItem * const dock_item = GDL_DOCK_ITEM(dockItem_);

    if (false == GDL_DOCK_OBJECT_ATTACHED(dock_item))
    {
        gdl_dock_item_show_item(GDL_DOCK_ITEM(dockItem_));
    }
}

} // namespace Solang
