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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <algorithm>
#include <functional>
#include <iostream>

#include <glibmm/i18n.h>
#include <sigc++/sigc++.h>

#include "main-window.h"

namespace Solang
{

static const std::string layoutFile
    = PACKAGE_DATA_DIR"/"PACKAGE_NAME"/"PACKAGE_NAME"-layout.xml";

static const std::string uiFile
    = PACKAGE_DATA_DIR"/"PACKAGE_NAME"/ui/"PACKAGE_NAME".ui";

class Docker :
    public std::unary_function<GdlDockObject * const &, void>
{
public:
    Docker(GdlDockObject * const & reference,
           const GdlDockPlacement placement) throw();

    Docker(const Docker & source) throw();
	
    ~Docker() throw();

    Docker &
    operator=(const Docker & source) throw();

    void operator()(GdlDockObject * const & requester) throw();

protected:
    GdlDockObject * reference_;

    GdlDockPlacement placement_;
};

Docker::Docker(GdlDockObject * const & reference,
               const GdlDockPlacement placement) throw() :
    std::unary_function<GdlDockObject * const &, void>(),
    reference_(reference),
    placement_(placement)
{
}

Docker::Docker(const Docker & source) throw() :
    std::unary_function<GdlDockObject * const &, void>(source),
    reference_(source.reference_),
    placement_(source.placement_)
{
}

Docker::~Docker() throw()
{
}

Docker &
Docker::operator=(const Docker & source) throw()
{
    if (this != &source)
    {
        std::unary_function<GdlDockObject * const &, void>
                ::operator=(source);
        reference_ = source.reference_;
        placement_ = source.placement_;
    }

	return *this;
}

void
Docker::operator()(GdlDockObject * const & requester) throw()
{
    gdl_dock_object_dock(reference_, requester, placement_, NULL);
}

MainWindow::MainWindow() throw() :
    Gtk::Window(Gtk::WINDOW_TOPLEVEL),
    actionGroup_(Gtk::ActionGroup::create()),
    uiManager_(Gtk::UIManager::create()),
    uiID_(uiManager_->add_ui_from_file(uiFile)),
    vBox_(false, 0),
    hBox_(false, 6),
    statusBar_(),
    dock_(gdl_dock_new()),
    dockBar_(gdl_dock_bar_new(GDL_DOCK(dock_))),
    layout_(gdl_dock_layout_new(GDL_DOCK(dock_))),
    showToolBar_(true),
    showStatusBar_(true),
    dockObjectsLeftTop_(),
    dockObjectsLeftBottom_(),
    dockObjectsCenter_()
{
    set_title("Solang");
	set_border_width(0);
    set_default_size(800, 600);
    add(vBox_);

    actionGroup_->add(
        Gtk::Action::create(
            "ActionPhotoMenu", _("_Photo")));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionPhotoQuit", Gtk::Stock::QUIT),
        sigc::mem_fun(*this, &MainWindow::on_action_photo_quit));

    actionGroup_->add(Gtk::Action::create("ActionViewMenu", "_View"));

    actionGroup_->add(
        Gtk::ToggleAction::create(
            "ActionViewToolBar", _("_Toolbar"),
            _("Show or hide the toolbar in the current window"),
            showToolBar_),
        sigc::mem_fun(*this, &MainWindow::on_action_view_tool_bar));

    actionGroup_->add(
        Gtk::ToggleAction::create(
            "ActionViewStatusBar", _("_Statusbar"),
            _("Show or hide the statusbar in the current window"),
            showStatusBar_),
        sigc::mem_fun(*this, &MainWindow::on_action_view_status_bar));

    actionGroup_->add(
        Gtk::ToggleAction::create(
            "ActionViewFullScreen", Gtk::Stock::FULLSCREEN,
            _("_Full Screen"),
            _("Show the current photo in full screen mode"),
            false),
        Gtk::AccelKey("F11"),
        sigc::mem_fun(*this, &MainWindow::on_action_view_full_screen));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionHelpMenu", _("_Help")));

    uiManager_->insert_action_group(actionGroup_);
    add_accel_group(uiManager_->get_accel_group());

    Gtk::Widget * const menu_bar = uiManager_->get_widget("/MenuBar");
    if (NULL != menu_bar)
    {
        vBox_.pack_start(*menu_bar, Gtk::PACK_SHRINK, 0);
    }

    Gtk::Widget * const tool_bar = uiManager_->get_widget("/ToolBar");
    if (NULL != tool_bar)
    {
        vBox_.pack_start(*tool_bar, Gtk::PACK_SHRINK, 0);
    }

    GtkBox * const hbox = GTK_BOX(hBox_.gobj());
    gtk_box_pack_start(hbox, dockBar_, FALSE, FALSE, 0);
    gtk_box_pack_end(hbox, dock_, TRUE, TRUE, 0);
    vBox_.pack_start(hBox_, Gtk::PACK_EXPAND_WIDGET, 0);

    statusBar_.set_has_resize_grip(true);
    vBox_.pack_start(statusBar_, Gtk::PACK_SHRINK, 0);

    show_all_children();
}

MainWindow::~MainWindow() throw()
{
    uiManager_->remove_action_group(actionGroup_);
    uiManager_->remove_ui(uiID_);
    g_object_unref(layout_);
}

void
MainWindow::init() throw()
{
    if (false == dockObjectsCenter_.empty())
    {
        gdl_dock_add_item(GDL_DOCK(dock_),
                          GDL_DOCK_ITEM(dockObjectsCenter_.front()),
                          GDL_DOCK_RIGHT);
        if (1 < dockObjectsCenter_.size())
        {
            std::for_each(++dockObjectsCenter_.begin(),
                          dockObjectsCenter_.end(),
                          Docker(dockObjectsCenter_.front(),
                                 GDL_DOCK_CENTER));
            present_dock_object(dockObjectsCenter_.front());
        }
    }

    if (false == dockObjectsLeftTop_.empty())
    {
        gdl_dock_add_item(GDL_DOCK(dock_),
                          GDL_DOCK_ITEM(dockObjectsLeftTop_.front()),
                          GDL_DOCK_LEFT);
        if (1 < dockObjectsLeftTop_.size())
        {
            std::for_each(++dockObjectsLeftTop_.begin(),
                          dockObjectsLeftTop_.end(),
                          Docker(dockObjectsLeftTop_.front(),
                                 GDL_DOCK_CENTER));
        }
    }

    if (false == dockObjectsLeftBottom_.empty())
    {
        gdl_dock_add_item(GDL_DOCK(dock_),
            GDL_DOCK_ITEM(dockObjectsLeftBottom_.front()),
            GDL_DOCK_BOTTOM);
        if (1 < dockObjectsLeftBottom_.size())
        {
            std::for_each(++dockObjectsLeftBottom_.begin(),
                          dockObjectsLeftBottom_.end(),
                          Docker(dockObjectsLeftBottom_.front(),
                                 GDL_DOCK_CENTER));
        }
    }

    const bool result = gdl_dock_layout_load_from_file(
                            GDL_DOCK_LAYOUT(layout_),
                            layoutFile.c_str());

    if (true == result)
    {
        gdl_dock_layout_load_layout(GDL_DOCK_LAYOUT(layout_), 0);
    }
    else
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": "
                  << layoutFile << " not found"
                  << std::endl;
    }
}

void
MainWindow::final() throw()
{
}

void
MainWindow::add_dock_object_left_top(GdlDockObject * dock_object)
                                     throw()
{
    dockObjectsLeftTop_.push_back(dock_object);
}

void
MainWindow::add_dock_object_left_bottom(GdlDockObject * dock_object)
                                        throw()
{
    dockObjectsLeftBottom_.push_back(dock_object);
}

void
MainWindow::add_dock_object_center(GdlDockObject * dock_object) throw()
{
    dockObjectsCenter_.push_back(dock_object);
}

void
MainWindow::present_dock_object(GdlDockObject * dock_object) throw()
{
    if (0 == dock_object)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << "0 == dock_object"
                  << std::endl;
        return;
    }

    Gtk::Container * dock_container
        = Glib::wrap(GTK_CONTAINER(dock_object), false);
    Gtk::Notebook * parent
        = dynamic_cast<Gtk::Notebook *>(dock_container->get_parent());

    if (0 != parent)
    {
        const gint page_num = parent->page_num(*dock_container);
        parent->set_current_page(page_num);
    }
}

Gtk::Notebook *
MainWindow::get_notebook_center() throw()
{
    Gtk::Container * dock_container
        = Glib::wrap(GTK_CONTAINER(dockObjectsCenter_.front()), false);
    Gtk::Notebook * parent
        = dynamic_cast<Gtk::Notebook *>(dock_container->get_parent());

    return parent;
}

const Glib::RefPtr<Gtk::UIManager> &
MainWindow::get_ui_manager() throw()
{
    return uiManager_;
}

void
MainWindow::on_action_photo_quit() throw()
{
    hide();
}

void
MainWindow::on_action_view_full_screen() throw()
{
}

void
MainWindow::on_action_view_status_bar() throw()
{
    showStatusBar_ = (true == showStatusBar_) ? false : true;
    (true == showStatusBar_) ? statusBar_.show_all()
                             : statusBar_.hide();
}

void
MainWindow::on_action_view_tool_bar() throw()
{
    showToolBar_ = (true == showToolBar_) ? false : true;

    Gtk::Widget * const tool_bar = uiManager_->get_widget("/ToolBar");
    if (NULL != tool_bar)
    {
        (true == showToolBar_) ? tool_bar->show_all()
                               : tool_bar->hide();
    }
}

bool
MainWindow::on_delete_event(GdkEventAny * event)
{
    bool return_value = Gtk::Window::on_delete_event(event);

    hide();
    return return_value;	
}

} // namespace Solang
