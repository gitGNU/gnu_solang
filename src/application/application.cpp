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

#include <algorithm>
#include <functional>
#include <string>
#include <utility>

#include <giomm.h>

#include "application.h"
#include "browser-model-column-record.h"
#include "browser-renderer.h"
#include "camera-source.h"
#include "console-renderer.h"
#include "content-type-repo.h"
#include "date-manager.h"
#include "directory-source.h"
#include "directory-storage.h"
#include "editor-renderer.h"
#include "enlarged-renderer.h"
#include "flickr-source.h"
#include "i-photo-source.h"
#include "i-plugin.h"
#include "importer.h"
#include "photo.h"
#include "progress-observer.h"
#include "property-manager.h"
#include "search-basket.h"
#include "tag-manager.h"
#include "thumbnail.h"
#include "thumbnail-store.h"

namespace Solang
{

template<typename T>
class Initializer :
    public std::unary_function<std::pair<const std::string, T> &, void>
{
    public:
        Initializer(Application * const & application) throw();

        Initializer(const Initializer<T> & source) throw();
    
        ~Initializer() throw();

        Initializer<T> &
        operator=(const Initializer<T> & source) throw();

        void
        operator()(std::pair<const std::string, T> & x) throw();

    protected:
        Application * application_;
};

template<typename T>
Initializer<T>::Initializer(Application * const & application) throw() :
    std::unary_function<std::pair<const std::string, T> &, void>(),
    application_(application)
{
}

template<typename T>
Initializer<T>::Initializer(const Initializer<T> & source) throw() :
    std::unary_function<std::pair<const std::string, T> &, void>(source),
    application_(source.application_)
{
}

template<typename T>
Initializer<T>::~Initializer() throw()
{
}

template<typename T>
Initializer<T> &
Initializer<T>::operator=(const Initializer<T> & source) throw()
{
    if (this != &source)
    {
        std::unary_function<std::pair<const std::string, T> &, void>
            ::operator=(source);
        application_ = source.application_;
    }

    return *this;
}

template<typename T>
void
Initializer<T>::operator()(std::pair<const std::string, T> & x) throw()
{
    x.second->init(*application_);
}

template<typename T>
class Finalizer :
    public std::unary_function<std::pair<const std::string, T> &, void>
{
    public:
        Finalizer(Application * const & application) throw();

        Finalizer(const Finalizer<T> & source) throw();
    
        ~Finalizer() throw();

        Finalizer<T> &
        operator=(const Finalizer<T> & source) throw();

        void
        operator()(std::pair<const std::string, T> & x) throw();

    protected:
        Application * application_;
};

template<typename T>
Finalizer<T>::Finalizer(Application * const & application) throw() :
    std::unary_function<std::pair<const std::string, T> &, void>(),
    application_(application)
{
}

template<typename T>
Finalizer<T>::Finalizer(const Finalizer<T> & source) throw() :
    std::unary_function<std::pair<const std::string, T> &, void>(source),
    application_(source.application_)
{
}

template<typename T>
Finalizer<T>::~Finalizer() throw()
{
}

template<typename T>
Finalizer<T> &
Finalizer<T>::operator=(const Finalizer<T> & source) throw()
{
    if (this != &source)
    {
        std::unary_function<std::pair<const std::string, T> &, void>
            ::operator=(source);
        application_ = source.application_;
    }

    return *this;
}

template<typename T>
void
Finalizer<T>::operator()(std::pair<const std::string, T> & x) throw()
{
    x.second->final(*application_);
}

Application::Application(int & argc, char ** & argv) throw() :
    sigc::trackable(),
    threadPool_(4, false),
    observer_( new ProgressObserver() ),
    engine_(argc, argv, observer_),
    mainWindow_(),
    progressDialog_(engine_.get_default_observer()),
    listStore_(Gtk::ListStore::create(BrowserModelColumnRecord())),
    listStoreIter_(),
    plugins_(),
    renderers_(),
    initEnd_(),
    listStoreChangeBegin_(),
    listStoreChangeEnd_()
{
    const std::string cache_dir_path = Glib::get_user_cache_dir()
                                       + "/" + Glib::get_prgname();

    if (false == Glib::file_test(cache_dir_path,
                                 Glib::FILE_TEST_EXISTS))
    {
        try
        {
            Gio::File::create_for_path(
                cache_dir_path)->make_directory_with_parents();
        }
        catch (const Gio::Error & e)
        {
            g_warning("%s", e.what().c_str());
        }
    }

    const std::string config_dir_path = Glib::get_user_config_dir()
                                        + "/" + Glib::get_prgname();

    if (false == Glib::file_test(config_dir_path,
                                 Glib::FILE_TEST_EXISTS))
    {
        try
        {
            Gio::File::create_for_path(
                config_dir_path)->make_directory_with_parents();
        }
        catch (const Gio::Error & e)
        {
            g_warning("%s", e.what().c_str());
        }
    }

    const std::string data_dir_path = Glib::get_user_data_dir()
                                        + "/" + Glib::get_prgname();

    if (false == Glib::file_test(data_dir_path,
                                 Glib::FILE_TEST_EXISTS))
    {
        try
        {
            Gio::File::create_for_path(
                data_dir_path)->make_directory_with_parents();
        }
        catch (const Gio::Error & e)
        {
            g_warning("%s", e.what().c_str());
        }
    }

    std::string thumbnail_store_path = Glib::get_user_data_dir() + "/";
    thumbnail_store_path += Glib::get_prgname();
    thumbnail_store_path += "/thumbnails/";

    std::string photo_store_path = Glib::get_user_special_dir(
                                       G_USER_DIRECTORY_PICTURES);

    if( photo_store_path.empty() )
    {
        photo_store_path = Glib::get_home_dir();
        photo_store_path += "/Pictures";
    }
    photo_store_path += "/";
    photo_store_path += Glib::get_application_name();

    ThumbnailStore thumbnail_store(thumbnail_store_path);
    IStoragePtr directory_storage(new DirectoryStorage(
                                          thumbnail_store,
                                          engine_.get_db(),
                                          photo_store_path));
    engine_.add_current_storage_system(
        directory_storage->get_storage_uri_prefix(), directory_storage);

    engine_.photo_import_begin().connect(sigc::mem_fun(*this,
        &Application::on_photo_import_begin));
    engine_.photo_import_end().connect(sigc::mem_fun(*this,
        &Application::on_photo_import_end));
    engine_.photo_render_begin().connect(sigc::mem_fun(*this,
        &Application::on_photo_render_begin));
}

Application::~Application() throw()
{
    threadPool_.shutdown(false);
}

void
Application::init() throw()
{
    engine_.init("");

    // Plugins.

    IPluginPtr date_manager(new DateManager());
    plugins_.insert(std::make_pair("date-manager",
                                   date_manager));

    IPluginPtr property_manager(new PropertyManager());
    plugins_.insert(std::make_pair("property-manager",
                                   property_manager));

    IPluginPtr search_basket(new SearchBasket());
    plugins_.insert(std::make_pair("search-basket", search_basket));

    IPluginPtr tag_manager(new TagManager( ));
    plugins_.insert(std::make_pair("tag-manager", tag_manager));

    IPhotoSourcePtr directory_source(new DirectorySource());
    IPluginPtr directory_importer(new Importer(directory_source, true));
    plugins_.insert(std::make_pair("directory-importer",
                                   directory_importer));

    IPhotoSourcePtr camera_source(new CameraSource());
    IPluginPtr camera_importer(new Importer(camera_source, false));
    plugins_.insert(std::make_pair("camera-importer", camera_importer));

    IPhotoSourcePtr flickr_source(new FlickrSource());
    IPluginPtr flickr_importer(new Importer(flickr_source, false));
    plugins_.insert(std::make_pair("flickr-importer", flickr_importer));

    std::for_each(plugins_.begin(), plugins_.end(),
                  Initializer<IPluginPtr>(this));

    // Renderers.

    RendererPtr browser_renderer(new BrowserRenderer());
    renderers_.insert(std::make_pair("browser-renderer",
                                     browser_renderer));

//    RendererPtr console_renderer(new ConsoleRenderer());
//    renderers_.insert(std::make_pair("console-renderer",
//                                     console_renderer));

    RendererPtr enlarged_renderer(new EnlargedRenderer());
    renderers_.insert(std::make_pair("enlarged-renderer",
                                     enlarged_renderer));

    RendererPtr editor_renderer(new EditorRenderer());
    renderers_.insert(std::make_pair("editor-renderer",
                                     editor_renderer));

    std::for_each(renderers_.begin(), renderers_.end(),
                  Initializer<RendererPtr>(this));

    engine_.set_current_renderer(browser_renderer);

    ContentTypeRepo::instance()->init();

    mainWindow_.init();

    initEnd_.emit(*this);
}

void
Application::run() throw()
{
    mainWindow_.show_all();
    Gtk::Main::run(mainWindow_);
}

void
Application::final() throw()
{
    engine_.final();
    mainWindow_.final();

    std::for_each(renderers_.begin(), renderers_.end(),
                  Finalizer<RendererPtr>(this));

    std::for_each(plugins_.begin(), plugins_.end(),
                  Finalizer<IPluginPtr>(this));
    plugins_.clear();
}

void
Application::add_photo_to_model(const PhotoPtr & photo) throw()
{
    Gtk::TreeModel::iterator model_iter = listStore_->append();
    const Gtk::TreeModel::Path model_path
                                   = listStore_->get_path(model_iter);
    Gtk::TreeModel::Row row = *model_iter;

    BrowserModelColumnRecord model_column_record;

    row[model_column_record.get_column_serial()]
        = static_cast<guint>(model_path.front());
    row[model_column_record.get_column_photo()] = photo;
    row[model_column_record.get_column_pixbuf()] = PixbufPtr(0);
}

void
Application::add_photos_to_model(const PhotoList & photos) throw()
{
    listStoreChangeBegin_.emit(*this);

    PhotoList::const_iterator list_iter;

    listStore_->clear();

    // Force the iterator to evaluate to false, which is otherwise
    // not happening.
    listStoreIter_ = listStore_->children().end();

    for (list_iter = photos.begin(); list_iter != photos.end();
         list_iter++)
    {
        add_photo_to_model(*list_iter);

        if (false == listStoreIter_)
        {
            listStoreIter_ = listStore_->children().begin();
        }

        while (true == Gtk::Main::events_pending())
        {
            Gtk::Main::iteration();
        }
    }

    listStoreChangeEnd_.emit(*this);
}

sigc::signal<void, Application &> &
Application::init_end() throw()
{
    return initEnd_;
}

sigc::signal<void, Application &> &
Application::list_store_change_begin() throw()
{
    return listStoreChangeBegin_;
}

sigc::signal<void, Application &> &
Application::list_store_change_end() throw()
{
    return listStoreChangeEnd_;
}

void
Application::on_photo_import_begin() throw()
{
    progressDialog_.set_transient_for(mainWindow_);
    progressDialog_.show_all();
}

void
Application::on_photo_import_end() throw()
{
    progressDialog_.hide();
    progressDialog_.reset();
    engine_.get_default_observer()->reset();
}

void
Application::on_photo_render_begin() throw()
{
    mainWindow_.set_busy(true);

    PhotoList photos = engine_.get_photos();
    add_photos_to_model(photos);

    mainWindow_.set_busy(false);
}

Glib::ThreadPool &
Application::get_thread_pool() throw()
{
    return threadPool_;
}

Engine &
Application::get_engine() throw()
{
    return engine_;
}

MainWindow &
Application::get_main_window() throw()
{
    return mainWindow_;
}

const ListStorePtr &
Application::get_list_store() throw()
{
    return listStore_;
}

Gtk::TreeModel::iterator &
Application::get_list_store_iter() throw()
{
    return listStoreIter_;
}

void
Application::set_list_store_iter(
                 const Gtk::TreeModel::iterator & iter) throw()
{
    listStoreIter_ = iter;
}

void
Application::set_list_store_iter(const Gtk::TreeModel::Path & path)
                                 throw()
{
    listStoreIter_ = listStore_->get_iter(path);
}

DragDropCriteriaMap &
Application::get_drag_drop_map() throw()
{
    return dragItemMap_;
}

void
Application::set_drag_item(const Glib::ustring & key,
                const PhotoSearchCriteriaPtr &criteria) throw()
{
    //Existing item will be replaced
    //This handles double dragging of same criteria
    dragItemMap_[ key ] = criteria;
    return;
}

RendererPtr
Application::get_renderer(const std::string & name) throw()
{
    const std::map<std::string, RendererPtr>::const_iterator iter
        = renderers_.find(name);

    if (renderers_.end() == iter)
    {
        return RendererPtr();
    }

    return iter->second;
}

} // namespace Solang
