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
#include <string>
#include <utility>

#include "application.h"
#include "browser-renderer.h"
#include "camera-source.h"
#include "console-renderer.h"
#include "directory-source.h"
#include "directory-storage.h"
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

    void operator()(std::pair<const std::string, T> & x) throw();

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

    void operator()(std::pair<const std::string, T> & x) throw();

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
    modelColumnRecord_(),
    listStore_(Gtk::ListStore::create(modelColumnRecord_)),
    listStoreIter_(),
    plugins_(),
    renderers_()
{
    ThumbnailStore thumbnail_store("/tmp/solang/thumbnails");
    IStoragePtr directory_storage(new DirectoryStorage(thumbnail_store,
                                                       engine_.get_db(),
                                                       "/tmp/solang"));
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

//    IPhotoSourcePtr flickr_source(new FlickrSource());
//    IPluginPtr flickr_importer(new Importer(flickr_source, false));
//    plugins_.insert(std::make_pair("flickr-importer", flickr_importer));

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

    std::for_each(renderers_.begin(), renderers_.end(),
                  Initializer<RendererPtr>(this));

    engine_.set_current_renderer(browser_renderer);

    mainWindow_.init();
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
    const Thumbnail & thumbnail = photo->get_thumbnail();
    PixbufPtr pixbuf;
    std::string path;

    try
    {
        path = Glib::filename_from_utf8(thumbnail.get_path());
    }
    catch (const Glib::ConvertError & e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
        return;
    }

    try
    {
        pixbuf = Gdk::Pixbuf::create_from_file(path, -1, 120, true);
    }
    catch (const Glib::FileError & e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
        return;
    }
    catch (const Gdk::PixbufError & e)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << ", "
                  << __FUNCTION__ << ": " << e.what()
                  << std::endl;
        return;
    }

    Gtk::TreeModel::iterator model_iter = listStore_->append();
    Gtk::TreeModel::Row row = *model_iter;

    row[modelColumnRecord_.get_column_photo()] = photo;
    row[modelColumnRecord_.get_column_pixbuf()] = pixbuf;
    row[modelColumnRecord_.get_column_tag_name()] 
        = photo->get_exif_data().get_picture_taken_time();
}

void
Application::add_photos_to_model(const PhotoList & photos) throw()
{
    PhotoList::const_iterator list_iter;

    listStore_->clear();

    for (list_iter = photos.begin(); list_iter != photos.end();
         list_iter++)
    {
        add_photo_to_model(*list_iter);
        while (true == Gtk::Main::events_pending())
        {
            Gtk::Main::iteration();
        }
    }
}

void
Application::on_photo_import_begin() throw()
{
    progressDialog_.set_progress_title( 
            engine_.get_default_observer()->get_event_description() );
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
    PhotoList photos = engine_.get_photos();
    add_photos_to_model(photos);
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

const Gtk::TreeModel::iterator &
Application::get_list_store_iter() const throw()
{
    return listStoreIter_;
}

void
Application::set_list_store_iter(const Gtk::TreeModel::Path & path)
                                 throw()
{
    listStoreIter_ = listStore_->get_iter(path);
}

} // namespace Solang
