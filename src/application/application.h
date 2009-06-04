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

#ifndef SOLANG_APPLICATION_H
#define SOLANG_APPLICATION_H

#include <map>

#include <glibmm.h>
#include <gtkmm.h>
#include <sigc++/sigc++.h>

#include "engine.h"
#include "main-window.h"
#include "progress-dialog.h"
#include "types.h"

namespace Solang
{

class IPlugin;

class Application :
    public sigc::trackable
{
    public:
        Application(int & argc, char ** & argv) throw();

        ~Application() throw();

        void
        init() throw();

        void
        run() throw();

        void
        final() throw();

        sigc::signal<void, Application &> &
        init_end() throw();

        sigc::signal<void, Application &> &
        list_store_change_begin() throw();

        sigc::signal<void, Application &> &
        list_store_change_end() throw();

        Glib::ThreadPool &
        get_thread_pool() throw();

        Engine &
        get_engine() throw();

        MainWindow &
        get_main_window() throw();

        const ListStorePtr &
        get_list_store() throw();

        Gtk::TreeModel::iterator &
        get_list_store_iter() throw();

        void
        set_list_store_iter(const Gtk::TreeModel::iterator & iter)
                            throw();

        void
        set_list_store_iter(const Gtk::TreeModel::Path & path) throw();

        DragDropCriteriaMap &
        get_drag_drop_map() throw();

        void
        set_drag_item(const Glib::ustring &, //key
                      const PhotoSearchCriteriaPtr &) throw();

        RendererPtr
        get_renderer(const std::string & name) throw();

    protected:

    private:
        void
        add_photo_to_model(const PhotoPtr & photo) throw();

        void
        add_photos_to_model(const PhotoList & photos) throw();

        void
        on_photo_import_begin() throw();

        void
        on_photo_import_end() throw();

        void
        on_photo_render_begin() throw();

        Glib::ThreadPool threadPool_;

        ProgressObserverPtr observer_;

        Engine engine_;

        MainWindow mainWindow_;

        ProgressDialog progressDialog_;

        ListStorePtr listStore_;

        Gtk::TreeModel::iterator listStoreIter_;

        std::map<std::string, IPluginPtr> plugins_;

        std::map<std::string, RendererPtr> renderers_;

        DragDropCriteriaMap dragItemMap_;

        sigc::signal<void, Application &> initEnd_;

        sigc::signal<void, Application &> listStoreChangeBegin_;

        sigc::signal<void, Application &> listStoreChangeEnd_;
};

} // namespace Solang

#endif // SOLANG_APPLICATION_H
