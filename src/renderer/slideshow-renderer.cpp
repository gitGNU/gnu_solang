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

#include <string>

#include <gdkmm.h>
#include <glibmm/i18n.h>

#include "application.h"
#include "browser-model-column-record.h"
#include "i-plugin.h"
#include "i-renderer-selector.h"
#include "slideshow-renderer.h"
#include "slideshow-window.h"

namespace Solang
{

static const guint interval = 4; // s

static const std::string uiFile
    = PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/ui/"
          PACKAGE_TARNAME"-slideshow-renderer.ui";

SlideshowRenderer::SlideshowRenderer() throw() :
    IRenderer(),
    sigc::trackable(),
    application_(0),
    actionGroup_(Gtk::ActionGroup::create()),
    uiID_(0),
    modelIter_(),
    previousRenderer_(),
    slideshowWindow_(),
    signalTimeout_()
{
    actionGroup_->add(
        Gtk::Action::create(
            "ActionGoFirstPhoto", Gtk::Stock::GOTO_FIRST,
            _("First"),
            _("Go to the first photo in the collection")),
        Gtk::AccelKey("<alt>Home"),
        sigc::mem_fun(*this, &SlideshowRenderer::on_action_go_first));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionGoPreviousPhoto", Gtk::Stock::GO_BACK,
            _("Previous"),
            _("Go to the previous photo in the collection")),
        Gtk::AccelKey("<alt>Left"),
        sigc::mem_fun(*this,
                      &SlideshowRenderer::on_action_go_previous));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionGoNextPhoto", Gtk::Stock::GO_FORWARD,
            _("Next"),
            _("Go to the next photo in the collection")),
        Gtk::AccelKey("<alt>Right"),
        sigc::mem_fun(*this, &SlideshowRenderer::on_action_go_next));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionGoLastPhoto", Gtk::Stock::GOTO_LAST,
            _("Last"),
            _("Go to the last photo in the collection")),
        Gtk::AccelKey("<alt>End"),
        sigc::mem_fun(*this, &SlideshowRenderer::on_action_go_last));

    {
        ActionPtr action = Gtk::Action::create(
            "ActionStopSlideshow", Gtk::Stock::LEAVE_FULLSCREEN);

        action->property_is_important().set_value(true);
        actionGroup_->add(
            action, Gtk::AccelKey("F5"),
            sigc::mem_fun(*this,
                &SlideshowRenderer::on_action_stop_slideshow));
    }

    actionGroup_->add(
        Gtk::Action::create(
            "AccelGoFirstPhoto", Gtk::StockID(),
            _("First"),
            _("Go to the first photo in the collection")),
        Gtk::AccelKey("Home"),
        sigc::mem_fun(*this, &SlideshowRenderer::on_action_go_first));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelGoPreviousPhoto", Gtk::StockID(),
            _("Previous"),
            _("Go to the previous photo in the collection")),
        Gtk::AccelKey("Left"),
        sigc::mem_fun(*this,
                      &SlideshowRenderer::on_action_go_previous));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelGoNextPhoto", Gtk::StockID(),
            _("Next"),
            _("Go to the next photo in the collection")),
        Gtk::AccelKey("Right"),
        sigc::mem_fun(*this, &SlideshowRenderer::on_action_go_next));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelGoLastPhoto", Gtk::StockID(),
            _("Last"),
            _("Go to the last photo in the collection")),
        Gtk::AccelKey("End"),
        sigc::mem_fun(*this, &SlideshowRenderer::on_action_go_last));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelStopSlideshow", Gtk::Stock::LEAVE_FULLSCREEN),
        Gtk::AccelKey("Escape"),
        sigc::mem_fun(*this,
                      &SlideshowRenderer::on_action_stop_slideshow));
}

SlideshowRenderer::~SlideshowRenderer() throw()
{
}

void
SlideshowRenderer::init(Application & application) throw()
{
    application_ = &application;
}

void
SlideshowRenderer::render(const PhotoPtr & photo) throw()
{
    Engine & engine = application_->get_engine();
    const IStoragePtr & storage = engine.get_current_storage_system(
                                      "file");
    photo->set_disk_file_path(storage);

    PixbufPtr pixbuf;
    std::string path;

    try
    {
        path = Glib::filename_from_utf8(photo->get_disk_file_path());
    }
    catch (const Glib::ConvertError & e)
    {
        g_warning("%s", e.what().c_str());
        return;
    }

    try
    {
        pixbuf = Gdk::Pixbuf::create_from_file(path);
    }
    catch (const Glib::FileError & e)
    {
        g_warning("%s", e.what().c_str());
        return;
    }
    catch (const Gdk::PixbufError & e)
    {
        g_warning("%s", e.what().c_str());
        return;
    }

    if (0 == slideshowWindow_)
    {
        create_slideshow_window();
    }
    slideshowWindow_->render(pixbuf);
}

void
SlideshowRenderer::render(const PhotoList & photos) throw()
{
}

void
SlideshowRenderer::final(Application & application) throw()
{
}

PhotoList
SlideshowRenderer::get_current_selection() throw()
{
    const Gtk::TreeModel::iterator & iter
        = application_->get_list_store_iter();
    Gtk::TreeModel::Row row = *iter;

    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    PhotoList photos;
    photos.push_back(photo);

    return photos;
}

void
SlideshowRenderer::present() throw()
{
    RendererRegistry & renderer_registry
                           = application_->get_renderer_registry();
    previousRenderer_ = renderer_registry.get_current();

    const IRendererPtr slideshow_renderer
        = renderer_registry.select<SlideshowRenderer>();

    if (0 == slideshow_renderer)
    {
        return;
    }

    renderer_registry.set_current(slideshow_renderer);

    modelIter_ = application_->get_list_store_iter();
    if (false == modelIter_)
    {
        return;
    }

    if (0 == slideshowWindow_)
    {
        create_slideshow_window();
    }
    slideshowWindow_->fullscreen();
    slideshowWindow_->show_all();

    signalTimeout_
        = Glib::signal_timeout().connect_seconds(
              sigc::mem_fun(*this, &SlideshowRenderer::on_timeout),
              interval, Glib::PRIORITY_DEFAULT);
}

void
SlideshowRenderer::receive_plugin(IPlugin & plugin) throw()
{
    plugin.visit_renderer(*this);
}

IRendererPtr
SlideshowRenderer::receive_selector(IRendererSelector & selector,
                                    const IRendererPtr & renderer)
                                    throw()
{
    return selector.select(*this, renderer);
}

void
SlideshowRenderer::create_slideshow_window() throw()
{
    slideshowWindow_ = SlideshowWindowPtr(new SlideshowWindow());

    const UIManagerPtr & ui_manager
                             = slideshowWindow_->get_ui_manager();

    uiID_ = ui_manager->add_ui_from_file(uiFile);
    if (0 == uiID_)
    {
        // FIXME: error condition.
    }

    ui_manager->insert_action_group(actionGroup_);
}

void
SlideshowRenderer::on_action_go_first() throw()
{
    if (false == modelIter_)
    {
        return;
    }

    const ListStorePtr & list_store = application_->get_list_store();
    modelIter_ = list_store->children().begin();

    Gtk::TreeModel::Row row = *modelIter_;
    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    render(photo);
}

void
SlideshowRenderer::on_action_go_last() throw()
{
    if (false == modelIter_)
    {
        return;
    }

    const ListStorePtr & list_store = application_->get_list_store();
    modelIter_ = list_store->children().end();
    modelIter_--;

    Gtk::TreeModel::Row row = *modelIter_;
    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    render(photo);
}

void
SlideshowRenderer::on_action_go_next() throw()
{
    if (false == modelIter_)
    {
        return;
    }

    modelIter_++;
    if (false == modelIter_)
    {
        const ListStorePtr & list_store
                                 = application_->get_list_store();
        modelIter_ = list_store->children().begin();
    }

    Gtk::TreeModel::Row row = *modelIter_;
    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    render(photo);
}

void
SlideshowRenderer::on_action_go_previous() throw()
{
    if (false == modelIter_)
    {
        return;
    }

    const ListStorePtr & list_store = application_->get_list_store();

    if (list_store->children().begin() == modelIter_)
    {
        modelIter_ = list_store->children().end();
    }
    modelIter_--;

    Gtk::TreeModel::Row row = *modelIter_;
    BrowserModelColumnRecord model_column_record;
    const PhotoPtr photo = row[model_column_record.get_column_photo()];

    render(photo);
}

void
SlideshowRenderer::on_action_stop_slideshow() throw()
{
    signalTimeout_.disconnect();
    slideshowWindow_->hide();
    slideshowWindow_->unfullscreen();

    RendererRegistry & renderer_registry
                           = application_->get_renderer_registry();
    renderer_registry.set_current(previousRenderer_);
}

bool
SlideshowRenderer::on_timeout() throw()
{
    on_action_go_next();
    return true;
}

} // namespace Solang
