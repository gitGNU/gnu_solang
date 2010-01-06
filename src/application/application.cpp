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

#include <algorithm>
#include <functional>
#include <string>
#include <utility>

#include <giomm.h>

#include "application.h"
#include "browser-model-column-record.h"
#include "browser-renderer.h"
//#include "camera-source.h"
//#include "console-renderer.h"
#include "content-type-repo.h"
//#include "date-manager.h"
#include "directory-destination.h"
//#include "directory-source.h"
#include "enlarged-renderer.h"
#include "exporter.h"
//#include "flickr-source.h"
//#include "i-photo-source.h"
#include "i-plugin.h"
//#include "importer.h"
#include "photo.h"
#include "progress-observer.h"
#include "property-manager.h"
#include "search-basket.h"
#include "slideshow-renderer.h"
#include "tag-manager.h"
#include "thumbnailer.h"

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
    iconFactory_(Gtk::IconFactory::create()),
    observer_( new ProgressObserver() ),
    engine_(argc, argv, observer_),
    mainWindow_(),
    progressDialog_(engine_.get_default_observer()),
    listStore_(Gtk::ListStore::create(BrowserModelColumnRecord())),
    listStoreIter_(),
    plugins_(),
    rendererRegistry_(),
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

    add_icons();

    engine_.signal_criteria_changed().connect(
        sigc::mem_fun(*this,
                      &Application::on_criteria_changed));
    engine_.photo_export_begin().connect(sigc::mem_fun(*this,
        &Application::show_progress_dialog));
    engine_.photo_export_end().connect(sigc::mem_fun(*this,
        &Application::hide_progress_dialog));
    engine_.photo_import_begin().connect(sigc::mem_fun(*this,
        &Application::show_progress_dialog));
    engine_.photo_import_end().connect(sigc::mem_fun(*this,
        &Application::hide_progress_dialog));

    Thumbnailer & thumbnailer = Thumbnailer::instance();
    thumbnailer.signal_ready().connect(
        sigc::mem_fun(*this,
                      &Application::on_thumbnailer_ready));
}

Application::~Application() throw()
{
    threadPool_.shutdown(false);
}

void
Application::init() throw()
{
    engine_.init("");
    engine_.search_async(
        PhotoSearchCriteriaList(),
        sigc::mem_fun(*this,
                      &Application::on_async_search));

    // Plugins.

//    IPluginPtr date_manager(new DateManager());
//    plugins_.insert(std::make_pair("date-manager",
//                                   date_manager));

    IPluginPtr property_manager(new PropertyManager());
    plugins_.insert(std::make_pair("property-manager",
                                   property_manager));

    IPluginPtr search_basket(new SearchBasket());
    plugins_.insert(std::make_pair("search-basket", search_basket));

    IPluginPtr tag_manager(new TagManager( ));
    plugins_.insert(std::make_pair("tag-manager", tag_manager));

    IPhotoDestinationPtr directory_destination(
                             new DirectoryDestination());
    IPluginPtr directory_exporter(new Exporter(directory_destination,
                                               true));
    plugins_.insert(std::make_pair("directory-exporter",
                                   directory_exporter));

//    IPhotoSourcePtr directory_source(new DirectorySource());
//    IPluginPtr directory_importer(new Importer(directory_source, true));
//    plugins_.insert(std::make_pair("directory-importer",
//                                   directory_importer));

//    IPhotoSourcePtr camera_source(new CameraSource());
//    IPluginPtr camera_importer(new Importer(camera_source, false));
//    plugins_.insert(std::make_pair("camera-importer", camera_importer));

//    IPhotoSourcePtr flickr_source(new FlickrSource());
//    IPluginPtr flickr_importer(new Importer(flickr_source, false));
//    plugins_.insert(std::make_pair("flickr-importer", flickr_importer));

    std::for_each(plugins_.begin(), plugins_.end(),
                  Initializer<IPluginPtr>(this));

    // Renderers.

    IRendererPtr browser_renderer(new BrowserRenderer());
    rendererRegistry_.add(browser_renderer);

//    IRendererPtr console_renderer(new ConsoleRenderer());
//    rendererRegistry_.add(console_renderer);

    IRendererPtr enlarged_renderer(new EnlargedRenderer());
    rendererRegistry_.add(enlarged_renderer);

    IRendererPtr slideshow_renderer(new SlideshowRenderer());
    rendererRegistry_.add(slideshow_renderer);

    rendererRegistry_.init(*this);

    const IRendererPtr renderer
        = rendererRegistry_.select<BrowserRenderer>();

    if (0 != renderer)
    {
        rendererRegistry_.set_current(renderer);
    }

    ContentTypeRepo::instance()->init();

    mainWindow_.init(*this);

    mainWindow_.connect_progress( engine_.get_default_observer() );

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
    mainWindow_.final(*this);

    rendererRegistry_.final(*this);

    std::for_each(plugins_.begin(), plugins_.end(),
                  Finalizer<IPluginPtr>(this));
    plugins_.clear();
}

void
Application::add_icons() throw()
{
    Gtk::IconSource icon_source;
    Gtk::IconSet icon_set_slideshow_play;

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME
            "/pixmaps/slideshow-play-16.png");
    icon_source.set_size(Gtk::IconSize(16));
    icon_set_slideshow_play.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME
            "/pixmaps/slideshow-play-22.png");
    icon_source.set_size(Gtk::IconSize(22));
    icon_set_slideshow_play.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME
            "/pixmaps/slideshow-play-24.png");
    icon_source.set_size(Gtk::IconSize(24));
    icon_set_slideshow_play.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME
            "/pixmaps/slideshow-play-32.png");
    icon_source.set_size(Gtk::IconSize(32));
    icon_set_slideshow_play.add_source(icon_source);

    icon_source.set_filename(
        PACKAGE_DATA_DIR"/"PACKAGE_TARNAME
            "/pixmaps/slideshow-play-48.png");
    icon_source.set_size(Gtk::IconSize(48));
    icon_set_slideshow_play.add_source(icon_source);

    iconFactory_->add(Gtk::StockID(PACKAGE_TARNAME"-slideshow-play"),
                      icon_set_slideshow_play);
    iconFactory_->add_default();
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

void
Application::hide_progress_dialog() throw()
{
    progressDialog_.hide();
    engine_.get_default_observer()->reset();
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
Application::on_async_search(PhotoList & photos) throw()
{
    mainWindow_.set_busy(true);
    add_photos_to_model(photos);
    mainWindow_.set_busy(false);
}

void
Application::on_criteria_changed(PhotoSearchCriteriaList & criteria)
                                 throw()
{
    engine_.search_async(
        criteria,
        sigc::mem_fun(*this,
                      &Application::on_async_search));
}

void
Application::on_thumbnailer_ready(PhotoList & photos) const throw()
{
    const Gtk::TreeModel::Children children = listStore_->children();

    // Hoping that keeping the bigger loop within the smaller loop
    // will result in better optimization. In this case,
    // children.size() is likely to be greater than photos.size().

    for (PhotoList::const_iterator photo_iter = photos.begin();
         photos.end() != photo_iter;
         photo_iter++)
    {
        for (Gtk::TreeModel::const_iterator model_iter
                                                = children.begin();
             children.end() != model_iter;
             model_iter++)
        {
            Gtk::TreeModel::Row row = *model_iter;
            BrowserModelColumnRecord model_column_record;
            const PhotoPtr photo
                = row[model_column_record.get_column_photo()];

            if (G_UNLIKELY(photo->get_uri()
                               == (*photo_iter)->get_uri()))
            {
                listStore_->row_changed(listStore_->get_path(
                                            model_iter),
                                        model_iter);
                break;
            }
        }

        while (true == Gtk::Main::events_pending())
        {
            Gtk::Main::iteration();
        }
    }
}

void
Application::show_progress_dialog() throw()
{
    progressDialog_.set_transient_for(mainWindow_);
    progressDialog_.show_all();
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

RendererRegistry &
Application::get_renderer_registry() throw()
{
    return rendererRegistry_;
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

} // namespace Solang
