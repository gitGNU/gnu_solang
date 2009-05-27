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
#include "renderer.h"
#include "tag-manager.h"
#include "tag-new-dialog.h"

namespace Solang
{

static const std::string uiFile
    = PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/ui/"
          PACKAGE_TARNAME"-tag-manager.ui";

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
    tagView_( )
{
    Gtk::IconSource icon_source;
    Gtk::IconSet icon_set_tag;

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/tag-16.png");
    icon_source.set_size(Gtk::IconSize(16));
    icon_set_tag.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/tag-22.png");
    icon_source.set_size(Gtk::IconSize(22));
    icon_set_tag.add_source(icon_source);

    Gtk::IconSet icon_set_tag_new;

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/tag-new-16.png");
    icon_source.set_size(Gtk::IconSize(16));
    icon_set_tag_new.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/tag-new-22.png");
    icon_source.set_size(Gtk::IconSize(22));
    icon_set_tag_new.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/tag-new-24.png");
    icon_source.set_size(Gtk::IconSize(24));
    icon_set_tag_new.add_source(icon_source);

    iconFactory_->add(Gtk::StockID(PACKAGE_TARNAME"-tag"),
                      icon_set_tag);
    iconFactory_->add(Gtk::StockID(PACKAGE_TARNAME"-tag-new"),
                      icon_set_tag_new);
    iconFactory_->add_default();

    actionGroup_->add(
        Gtk::Action::create(
            "ActionMenuTags", _("_Tags")));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionTagsNew", Gtk::StockID(PACKAGE_TARNAME"-tag-new"),
            _("Create New _Tag...")),
        Gtk::AccelKey(""),
        sigc::mem_fun(*this, &TagManager::on_action_tag_new));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionTagsEdit", Gtk::Stock::EDIT,
            _("_Edit Selected Tag...")),
        Gtk::AccelKey(""),
        sigc::mem_fun(*this, &TagManager::on_action_tag_edit));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionTagsDelete", Gtk::Stock::DELETE,
            _("_Delete Selected Tag")));

    actionGroup_->add(
        Gtk::Action::create("ActionTagsAttach", Gtk::Stock::ADD,
                            _("_Attach Tag to Selection")),
        Gtk::AccelKey("<control>T"),
        sigc::mem_fun(*this, &TagManager::on_action_apply_tag));

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
                                             PACKAGE_TARNAME"-tag",
                                             dockItemBehaviour_);
    
    gtk_container_add(GTK_CONTAINER(dockItem_),
                      GTK_WIDGET(vBox_.gobj()));
    vBox_.pack_start( scrolledWindow_ );

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
    tagView_.set_application( &application );

    populate_view();

    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_left_top(GDL_DOCK_OBJECT(dockItem_));

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
TagManager::on_action_tag_new() throw()
{
    TagNewDialog tag_new_dialog;
    tag_new_dialog.set_transient_for(application_->get_main_window());

    const gint response = tag_new_dialog.run();

    switch (response)
    {
        case Gtk::RESPONSE_OK:
        {
            TagPtr tag( new Tag() );
            tag->set_name( tag_new_dialog.get_name() );
            tag->set_description( tag_new_dialog.get_description() );
            tag->set_icon_path( tag_new_dialog.get_icon_path() );
            DatabasePtr db = application_->get_engine().get_db();
            tag->save( *db );
            populate_view();
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

void
TagManager::on_action_tag_edit() throw()
{
    Glib::RefPtr<Gtk::TreeSelection> selected
                                            = tagView_.get_selection();

    if( 0 == selected->count_selected_rows() )
        return;

    Gtk::TreeModel::iterator item = selected->get_selected();
    const TagViewModelColumnRecord &rec
                                = tagView_.get_column_records();

    if( item != selected->get_model()->children().end() )
    {
        Gtk::TreeModel::Row row= (*item);
        TagPtr tag = row[ rec.get_column_tag() ];

        TagNewDialog tag_new_dialog( tag );
        tag_new_dialog.set_transient_for(
                                application_->get_main_window());

        const gint response = tag_new_dialog.run();

        switch (response)
        {
            case Gtk::RESPONSE_OK:
            {
                tag->set_name( tag_new_dialog.get_name() );
                tag->set_description( tag_new_dialog.get_description() );
                tag->set_icon_path( tag_new_dialog.get_icon_path() );
                DatabasePtr db = application_->get_engine().get_db();
                tag->save( *db );
                populate_view();
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
}

void
TagManager::on_action_apply_tag() throw()
{
    Glib::RefPtr<Gtk::TreeSelection> selected
                                            = tagView_.get_selection();

    if( 0 == selected->count_selected_rows() )
        return;

    Gtk::TreeModel::iterator item = selected->get_selected();
    const TagViewModelColumnRecord &rec
                                = tagView_.get_column_records();

    if( item != selected->get_model()->children().end() )
    {
        Gtk::TreeModel::Row row= (*item);
        TagPtr tag = row[ rec.get_column_tag() ];

        Engine &engine = application_->get_engine();
        RendererPtr renderer = engine.get_current_renderer();

        PhotoList photos = renderer->get_current_selection();

        engine.apply_tag_to_photos( photos, tag );
    }

    return;
}

void
TagManager::populate_view() throw()
{
    Engine & engine = application_->get_engine();
    TagList tags = engine.get_tags();
    tagView_.populate(tags);
}
} // namespace Solang
