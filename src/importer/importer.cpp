/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * importer.cpp
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * importer.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * importer.cpp is distributed in the hope that it will be useful, but
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

#include <sigc++/sigc++.h>

#include "application.h"
#include "i-photo-source.h"
#include "importer-dialog.h"
#include "importer.h"
#include "progress-observer.h"
#include "tag.h"

namespace Solang
{

static const std::string uiFile
    = PACKAGE_DATA_DIR"/"PACKAGE_NAME"/ui/"PACKAGE_NAME"-importer.ui";
    
Importer::Importer(const IPhotoSourcePtr & photo_source, bool standard)
    throw() :
    Plugin(),
    application_(NULL),
    photoSource_(photo_source),
    standard_(standard),
    actionGroup_(Gtk::ActionGroup::create()),
    standardUIID_(0),
    uiID_(0)
{
    if (true == standard_)
    {
        actionGroup_->add(
            Gtk::Action::create("ActionPhotoImportStandard",
                                Gtk::Stock::ADD, "Import"),
            sigc::mem_fun(*this, &Importer::on_action_photo_import));
    }

    actionGroup_->add(
        Gtk::Action::create("ActionPhotoMenuImportMenu",
                            Gtk::Stock::ADD, "Import From"));

    const Glib::ustring action_name
        = "ActionPhotoImport" + photoSource_->get_name();

    actionGroup_->add(
        Gtk::Action::create(action_name, photoSource_->get_stock_id(),
                            photoSource_->get_label()),
        sigc::mem_fun(*this, &Importer::on_action_photo_import));
}

Importer::~Importer() throw()
{
}

void
Importer::init(Application & application) throw()
{
    application_ = &application;

    MainWindow & main_window = application.get_main_window();
    const UIManagerPtr & ui_manager = main_window.get_ui_manager();

    if (true == standard_)
    {
        Glib::ustring standard_ui_info
            = "<ui>"
              "  <toolbar name='ToolBar'>"
              "    <placeholder name='PlaceholderImportToolBar'>"
              "    <toolitem action='ActionPhotoImportStandard'/>"
              "    </placeholder>"
              "  </toolbar>"
              "</ui>";

        standardUIID_
            = ui_manager->add_ui_from_string(standard_ui_info);
        if (0 == standardUIID_)
        {
            // FIXME: error condition.
        }
    }

    Glib::ustring ui_info
        = "<ui>"
          "  <menubar name='MenuBar'>"
          "    <menu action='ActionPhotoMenu'>"
          "      <placeholder name='PlaceholderImportMenus'>"
          "      <menu action='ActionPhotoMenuImportMenu'>"
          "        <placeholder name='PlaceholderImportSubMenus'>"
          "        <menuitem action='ActionPhotoImport"
                       + photoSource_->get_name()
                       + "'/>"
          "        </placeholder>"
          "      </menu>"
          "      </placeholder>"
          "    </menu>"
          "  </menubar>"
          "</ui>";

    uiID_ = ui_manager->add_ui_from_string(ui_info);
    if (0 == uiID_)
    {
        // FIXME: error condition.
    }

    ui_manager->insert_action_group(actionGroup_);

    initialized_.emit(*this);
}

void
Importer::final(Application & application) throw()
{
    MainWindow & main_window = application.get_main_window();
    const Glib::RefPtr<Gtk::UIManager> & ui_manager
        = main_window.get_ui_manager();

    ui_manager->remove_action_group(actionGroup_);
    ui_manager->remove_ui(uiID_);
    if (true == standard_)
    {
        ui_manager->remove_ui(standardUIID_);
    }

    finalized_.emit(*this);
}

void
Importer::on_action_photo_import() throw()
{
    Engine & engine = application_->get_engine();
    ImporterDialog importer_dialog(photoSource_->get_browser(),
                                   engine.get_tags());
    importer_dialog.set_transient_for(application_->get_main_window());

    const gint response = importer_dialog.run();

    switch (response)
    {
        case Gtk::RESPONSE_APPLY:
        {
            TagList tags;
            importer_dialog.get_tag_view().get_selected_tags( tags );
            
            Glib::ThreadPool & thread_pool
                = application_->get_thread_pool();
            const IStoragePtr & storage
                = engine.get_current_storage_system("file");

            thread_pool.push(
                sigc::bind(sigc::mem_fun(engine, &Engine::import),
                           photoSource_, storage, tags));
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
