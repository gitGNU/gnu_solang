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

#include <glibmm/i18n.h>
#include <sigc++/sigc++.h>

#include "application.h"
#include "main-window.h"
#include "tag-manager.h"
#include "tag-new-dialog.h"

namespace Solang
{

static const std::string uiFile
    = PACKAGE_DATA_DIR"/"PACKAGE_NAME"/ui/"PACKAGE_NAME"-tag-manager.ui";

TagManager::TagManager() throw() :
    Plugin(),
    application_(),
    iconFactory_(Gtk::IconFactory::create()),
    actionGroup_(Gtk::ActionGroup::create()),
    uiID_(0),
    dockItemName_("tag-managers-dock-item"),
    dockItemTitle_("Tags"),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NORMAL),
    dockItem_(NULL),
    vBox_( false, 6 ),
    scrolledWindow_(),
    hBox_(),
    applyButton_(Gtk::Stock::APPLY),
    clearButton_(Gtk::Stock::CLEAR),
    tagView_()
{
    Gtk::IconSource icon_source;
    Gtk::IconSet icon_set_tag;

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/tag-16.png");
    icon_source.set_size(Gtk::IconSize(16));
    icon_set_tag.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/tag-22.png");
    icon_source.set_size(Gtk::IconSize(22));
    icon_set_tag.add_source(icon_source);

    Gtk::IconSet icon_set_tag_new;

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/tag-new-16.png");
    icon_source.set_size(Gtk::IconSize(16));
    icon_set_tag_new.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/tag-new-22.png");
    icon_source.set_size(Gtk::IconSize(22));
    icon_set_tag_new.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/tag-new-24.png");
    icon_source.set_size(Gtk::IconSize(24));
    icon_set_tag_new.add_source(icon_source);

    iconFactory_->add(Gtk::StockID(PACKAGE_NAME"-tag"), icon_set_tag);
    iconFactory_->add(Gtk::StockID(PACKAGE_NAME"-tag-new"),
                      icon_set_tag_new);
    iconFactory_->add_default();

    actionGroup_->add(
        Gtk::Action::create(
            "ActionMenuTags", _("_Tags")));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionTagsNew", Gtk::StockID(PACKAGE_NAME"-tag-new"),
            _("Create New _Tag...")),
        Gtk::AccelKey(""),
        sigc::mem_fun(*this, &TagManager::on_action_tag_new));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionTagsEdit", Gtk::Stock::EDIT,
            _("_Edit Selected Tag...")));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionTagsDelete", Gtk::Stock::DELETE,
            _("_Delete Selected Tag")));

    actionGroup_->add(
        Gtk::Action::create("ActionTagsAttach", Gtk::Stock::ADD,
                            "_Attach Tag to Selection"),
        Gtk::AccelKey("<control>T"));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionTagsRemove", Gtk::Stock::REMOVE,
            _("_Remove Tag From Selection")));

    scrolledWindow_.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC);
    scrolledWindow_.add(tagView_);
#if 0
    tagView_.signal_().connect(
                sigc::mem_fun( 
                    *this, &TagManager::tag_row_activated ) );
#endif
    dockItem_ = gdl_dock_item_new_with_stock(dockItemName_.c_str(),
                                             dockItemTitle_.c_str(),
                                             PACKAGE_NAME"-tag",
                                             dockItemBehaviour_);
    
    gtk_container_add(GTK_CONTAINER(dockItem_),
                      GTK_WIDGET(vBox_.gobj()));
    vBox_.pack_start( hBox_, Gtk::PACK_SHRINK, 0 );
    hBox_.pack_start( applyButton_, Gtk::PACK_SHRINK, 0 );
    hBox_.pack_start( clearButton_, Gtk::PACK_SHRINK, 0 );
    vBox_.pack_start( scrolledWindow_, Gtk::PACK_EXPAND_WIDGET,0 );
    applyButton_.set_size_request( -1, 32 );
    applyButton_.set_relief( Gtk::RELIEF_HALF);
    applyButton_.signal_clicked().connect(
                    sigc::mem_fun(
                        *this, &TagManager::apply_selected_tags));
    clearButton_.set_size_request( -1, 32 );
    clearButton_.set_relief( Gtk::RELIEF_HALF);
    clearButton_.signal_clicked().connect(
                    sigc::mem_fun(
                        *this, &TagManager::clear_tag_selection));

}

TagManager::~TagManager() throw()
{
    //g_object_unref(dockItem_);
    iconFactory_->remove_default();
}

void
TagManager::init(Application & application)
    throw()
{
    application_ = &application;

    Engine & engine = application.get_engine();
    TagList tags = engine.get_tags();
    tagView_.populate(tags);
    engine.get_criterion_repo().register_source( &tagView_ );

    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_left(GDL_DOCK_OBJECT(dockItem_));

    const Glib::RefPtr<Gtk::UIManager> & ui_manager
        = main_window.get_ui_manager();

    uiID_ = ui_manager->add_ui_from_file(uiFile);
    if (0 == uiID_)
    {
        // FIXME: error condition.
    }

    ui_manager->insert_action_group(actionGroup_);

    initialized_.emit(*this);
}

void
TagManager::final(Application & application)
    throw()
{
    MainWindow & main_window = application.get_main_window();
    const Glib::RefPtr<Gtk::UIManager> & ui_manager
        = main_window.get_ui_manager();

    ui_manager->remove_action_group(actionGroup_);
    ui_manager->remove_ui(uiID_);

    finalized_.emit(*this);
}

void
TagManager::apply_selected_tags() throw()
{
    Engine & engine = application_->get_engine();

    engine.criterion_changed().emit();

    return;
}

void 
TagManager::clear_tag_selection() throw()
{
    Engine & engine = application_->get_engine();

    Glib::ThreadPool & thread_pool = application_->get_thread_pool(); 

    tagView_.clear_tag_selection();

    PhotoSearchCriteriaList tags;

    thread_pool.push(sigc::bind(
        sigc::mem_fun1<const PhotoSearchCriteriaList &>(
            engine, &Engine::show), tags));

    return;
}

void
TagManager::on_action_tag_new() throw()
{
    TagNewDialog tag_new_dialog;
    tag_new_dialog.set_transient_for(application_->get_main_window());

    const gint response = tag_new_dialog.run();

    switch (response)
    {
        case Gtk::RESPONSE_OK:
        {
            break;
        }

        case Gtk::RESPONSE_CANCEL:
        case Gtk::RESPONSE_DELETE_EVENT:
        {
            break;
        }

        default:
        {
            break;
        }
    }
}

} // namespace Solang
