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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <vector>

#include <glibmm/i18n.h>
#include <gdkmm.h>

#include "application.h"
#include "browser-renderer.h"
#include "console-renderer.h"
#include "engine.h"
#include "enlarged-renderer.h"
#include "main-window.h"
#include "search-basket.h"
#include "search-basket-column-record.h"

namespace Solang
{

const std::string SearchBasket::uiFile_
    = PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/ui/"
          PACKAGE_TARNAME"-search-basket.ui";

SearchBasket::SearchBasket() throw() :
    Plugin(),
    sigc::trackable(),
    actionGroup_(Gtk::ActionGroup::create(
                     Glib::ustring::compose("%1:%2",
                                            __FILE__,
                                            __LINE__))),
    dockItemName_("search-basket-dock-item"),
    dockItemTitle_(_("Search")),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NORMAL),
    dockItem_(NULL),
    vBox_( false, 6 ),
    scrolledWindow_(),
    listStore_(Gtk::ListStore::create(SearchBasketColumnRecord())),
    treeView_(listStore_),
    application_( NULL ),
    uiManager_(Gtk::UIManager::create()),
    uiIDTreeView_(uiManager_->add_ui_from_file(uiFile_)),
    menu_(0),
    signalRendererChanged_()
{
    actionGroup_->add(
        Gtk::Action::create(
            "ActionSelectAll", Gtk::Stock::SELECT_ALL,
            _("Select _All"),
            _("Select all the filters in this list")),
        Gtk::AccelKey("<control>A"),
        sigc::mem_fun(*this,
                      &SearchBasket::on_action_select_all));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionRemoveSelected", Gtk::Stock::REMOVE,
            _("_Remove"),
            _("Remove the selected filters from this list")),
        Gtk::AccelKey("Delete"),
        sigc::mem_fun(*this,
                      &SearchBasket::on_action_remove_selected));

    uiManager_->insert_action_group(actionGroup_);

    Gtk::Widget * menu = uiManager_->get_widget("/SearchBasketPopup");
    menu_ = dynamic_cast<Gtk::Menu *>(menu);
    menu_->accelerate(treeView_);

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

    const Glib::RefPtr<Gtk::TreeSelection> selection
        = treeView_.get_selection();
    selection->set_mode(Gtk::SELECTION_MULTIPLE);

    scrolledWindow_.add(treeView_);

    dockItem_ = gdl_dock_item_new_with_stock(dockItemName_.c_str(),
                                             dockItemTitle_.c_str(),
                                             GTK_STOCK_FIND,
                                             dockItemBehaviour_);
    gtk_container_add(GTK_CONTAINER(dockItem_),
                      GTK_WIDGET(vBox_.gobj()));

    vBox_.pack_start( scrolledWindow_, Gtk::PACK_EXPAND_WIDGET,0 );

    std::vector<Gtk::TargetEntry> targets;
    targets.push_back(Gtk::TargetEntry("STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    targets.push_back(Gtk::TargetEntry("UTF8_STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    treeView_.enable_model_drag_dest(targets, Gdk::ACTION_COPY);

    treeView_.signal_button_press_event().connect(
        sigc::mem_fun(*this, &SearchBasket::on_button_press_event),
        false);

    treeView_.signal_drag_data_received().connect_notify(
        sigc::mem_fun(
            *this, &SearchBasket::on_drag_data_received));
}

SearchBasket::~SearchBasket() throw()
{
    uiManager_->remove_action_group(actionGroup_);
    uiManager_->remove_ui(uiIDTreeView_);
    treeView_.unset_model();
    //g_object_unref(dockItem_);
}

void
SearchBasket::init(Application & application) throw()
{
    application_ = &application;
    Engine & engine = application.get_engine();
    engine.get_criterion_repo().register_source( this );

    RendererRegistry & renderer_registry
                           = application.get_renderer_registry();

    signalRendererChanged_
        = renderer_registry.changed().connect(
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
SearchBasket::visit_renderer(EnlargedRenderer & enlarged_renderer)
                             throw()
{
    ui_show();
}

void
SearchBasket::visit_renderer(SlideshowRenderer & slideshow_renderer)
                             throw()
{
}

void
SearchBasket::on_action_select_all() throw()
{
    const Glib::RefPtr<Gtk::TreeSelection> selection
        = treeView_.get_selection();
    selection->select_all();
}

void
SearchBasket::on_action_remove_selected() throw()
{
    const Glib::RefPtr<Gtk::TreeSelection> selection
        = treeView_.get_selection();

    const TreePathList selected_rows = selection->get_selected_rows();

    // NB: Erase the paths by traversing the list backwards. Otherwise
    //     on erasing each path the remaining ones will be
    //     invalidated.

    for (TreePathList::const_reverse_iterator iter
             = selected_rows.rbegin();
         selected_rows.rend() != iter;
         iter++)
    {
        const Gtk::TreeModel::iterator model_iter
            = listStore_->get_iter(*iter);
        listStore_->erase(model_iter);
    }

    apply_criterion();
    return;
}

bool
SearchBasket::on_button_press_event(GdkEventButton * event) throw()
{
    if (3 == event->button
        && GDK_BUTTON_PRESS == event->type
        && false == listStore_->children().empty())
    {
        const ActionPtr action = actionGroup_->get_action(
                                     "ActionRemoveSelected");
        Gtk::TreeModel::Path path;
        const bool result = treeView_.get_path_at_pos(
                                static_cast<gint>(event->x),
                                static_cast<gint>(event->y),
                                path);

        if (false == result || true == path.empty())
        {
            action->set_visible(false);
        }
        else
        {
            action->set_visible(true);

            const Glib::RefPtr<Gtk::TreeSelection> selection
                = treeView_.get_selection();

            if (false == selection->is_selected(path))
            {
                selection->unselect_all();
                selection->select(path);
                // treeView_.set_cursor(path, 0, false);
            }
        }

        menu_->popup(event->button, event->time);
        return true;
    }

    return false;
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
SearchBasket::on_renderer_changed(
                  RendererRegistry & renderer_registry) throw()
{
    if (false == gdl_dock_object_is_bound(GDL_DOCK_OBJECT(dockItem_)))
    {
        return;
    }

    const IRendererPtr & renderer = renderer_registry.get_current();
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

    apply_criterion();
    return true;
}

void
SearchBasket::apply_criterion()
{
    Engine & engine = application_->get_engine();
    engine.criteria_changed();
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
