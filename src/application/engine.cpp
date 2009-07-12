/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
 * Copyright (C) 2009 Santanu Sinha <santanu.sinha@gmail.com>
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

#include "date-photo-info.h"
#include "engine.h"
#include "i-photo-destination.h"
#include "i-photo-source.h"
#include "photo-tag.h"
#include "progress-observer.h"
#include "renderer.h"
#include "tag.h"

namespace Solang
{

Engine::Engine(int & argc, char ** & argv,
               const ProgressObserverPtr & observer) throw() :
    observer_(observer),
    photoImportBegin_(),
    photoImportEnd_(),
    photoRenderBegin_(),
    photoRenderEnd_(),
    tagAddBegin_(),
    tagAddEnd_(),
    criterionChanged_(),
    itemActivated_(),
    itemEdit_(),
    rendererChanged_(),
    selectionChanged_(),
    mutex_(),
    photos_(),
    currentStorageSystems_(),
    currentRenderer_(),
    database_(""),
    criterionRepo_(),
    deleteActions_( database_ )
{
    //TBD::CREATE
    {
        Glib::ustring dbPath = Glib::get_user_data_dir() + "/";
        dbPath += Glib::get_prgname();
        dbPath += "/";
        database_.set_path( dbPath );
    }
    criterionChanged_.connect(
                sigc::mem_fun( *this, &Engine::on_criterion_changed));
}

Engine::~Engine() throw()
{
}

void
Engine::init(Glib::ustring str)
{
    database_.open();
}

void
Engine::final()
{
    deleteActions_.execute_actions( observer_ );
}

void
Engine::import(const PhotoPtr & photo,
               const IPhotoSourcePtr & source,
               const IStoragePtr & selected_storage,
               const TagList & tags) throw()
{
    import(photo, source, selected_storage, tags, observer_);
    return;
}

void
Engine::import(const PhotoPtr & photo,
               const IPhotoSourcePtr & source,
               const IStoragePtr & selected_storage,
               const TagList & tags,
               const ProgressObserverPtr & observer) throw()
{
    photoImportBegin_.emit();
#if 0
    for (TagList::const_iterator it = tags.begin();
         it != tags.end(); it++)
    {
        (*it)->save(database_);
    }
#endif
    PhotoPtr imp_photo = source->import(photo, selected_storage,
                                    tags, database_, observer);
    observer->reset();
    photoImportEnd_.emit();

    PhotoList imp_photos;
    imp_photos.push_back(imp_photo);
    {
        Glib::Mutex::Lock lock(mutex_);
        photos_ = imp_photos;
    }
    photoRenderBegin_.emit();

    return;
}

void
Engine::import(const PhotoList & photos,
               const IPhotoSourcePtr & source,
               const IStoragePtr & selected_storage,
               const TagList & tags) throw()
{
    import(photos, source, selected_storage, tags, observer_);
    return;
}

void
Engine::import(const PhotoList & photos,
               const IPhotoSourcePtr & source,
               const IStoragePtr & selected_storage,
               const TagList & tags,
               const ProgressObserverPtr & observer) throw()
{
    photoImportBegin_.emit();
#if 0
    for (TagList::const_iterator it = tags.begin();
         it != tags.end(); it++)
    {
        (*it)->save(database_);
    }
#endif
    PhotoList imp_photos = source->import(photos, selected_storage,
                                          tags, database_, observer);
    observer->reset();
    photoImportEnd_.emit();

    {
        Glib::Mutex::Lock lock(mutex_);
        photos_ = imp_photos;
    }
    photoRenderBegin_.emit();

    return;
}

void
Engine::import(const IPhotoSourcePtr & source,
               const IStoragePtr & selected_storage,
               const TagList & tags) throw()
{
    import(source, selected_storage, tags, observer_);
    return;
}

void
Engine::import(const IPhotoSourcePtr & source,
               const IStoragePtr & selected_storage,
               const TagList & tags,
               const ProgressObserverPtr & observer) throw()
{
    observer_->set_event_description(_("Beginning to Import Photos"));

    photoImportBegin_.emit();
#if 0
    for (TagList::const_iterator it = tags.begin();
         it != tags.end(); it++)
    {
        (*it)->save(database_);
    }
#endif
    PhotoList imp_photos = source->import(selected_storage, tags,
                                          database_, observer);
    observer->reset();
    photoImportEnd_.emit();

    {
        Glib::Mutex::Lock lock(mutex_);
        photos_ = imp_photos;
    }
    photoRenderBegin_.emit();

    return;
}

PhotoList
Engine::search(const PhotoSearchCriteriaList & criterion,
               const ProgressObserverPtr & observer) throw()
{
    ProgressObserverPtr obs = (observer) ? observer : observer_;
    return database_.search(criterion, obs);
}

void
Engine::on_criterion_changed()
{
    criterionRepo_.update();
    const PhotoSearchCriteriaList & criterion
                        = criterionRepo_.get_criterion();
    show( criterion, observer_ );
}

void
Engine::show(const PhotoSearchCriteriaList & criterion) throw()
{
    return show( criterion, observer_);
}

void
Engine::show(const PhotoSearchCriteriaList & criterion,
             const ProgressObserverPtr & observer) throw()
{
    PhotoList photos = create_renderable_list_from_photos(
                           search(criterion, observer_), observer_);

    {
        Glib::Mutex::Lock lock(mutex_);
        photos_ = photos;
    }
    photoRenderBegin_.emit();

    return;
}

void
Engine::export_photos(const PhotoList & photos,
                      const IPhotoDestinationPtr & destination,
                      const ProgressObserverPtr & observer)
{
    ProgressObserverPtr obs = (observer) ? observer : observer_;

    destination->export_photos(create_renderable_list_from_photos(
                               photos, obs), obs);
    return;
}


void
Engine::erase(const PhotoList & photos,
              const ProgressObserverPtr & observer)
{
    ProgressObserverPtr obs = (observer) ? observer : observer_;

    if (obs)
    {
        obs->set_num_events(photos.size());
        obs->set_event_description("Removing Selected Photos");
    }

    DBTablePtr photosTable = database_.getTable( "photos" );
    DBTablePtr photoTagsTable = database_.getTable( "photo_tags" );

    for( PhotoList::const_iterator photo = photos.begin();
         photo != photos.end(); photo++ )
    {
        const Glib::ustring &uri = (*photo)->get_uri();
        Glib::ustring storagePrefix
                            = uri.substr(0, uri.find( ":" ) );
        StorageMap::iterator storage
                        = currentStorageSystems_.find( storagePrefix );
        if( storage != currentStorageSystems_.end() )
        {
            ((*storage).second)->remove( (*photo) );
            photosTable->remove( *(*photo) );
            photoTagsTable->remove( *(*photo) );
            if( obs )
                obs->receive_event_notifiation();
        }    
        else
        {
            //TBD::Error 
        }
    }
        
    return;
}

void
Engine::save(const PhotoPtr & photo)
{
    const Glib::ustring & uri = photo->get_uri();
    Glib::ustring storagePrefix
                        = uri.substr(0, uri.find( ":" ) );
    StorageMap::iterator storage
                    = currentStorageSystems_.find( storagePrefix );
    if( storage != currentStorageSystems_.end() )
    {
        (*storage).second->save( photo, false );
    }
    photo->set_has_unsaved_data( false );
}

PhotoList
Engine::create_renderable_list_from_photos(
    const PhotoList & photos,
    const ProgressObserverPtr & observer)
{
    ProgressObserverPtr obs = (observer) ? observer : observer_;

    if( obs )
    {
        obs->set_num_events( photos.size() );
        obs->set_event_description( "Updating path info" );
    }

    for( PhotoList::const_iterator photo = photos.begin();
         photo != photos.end(); photo++ )
    {
        const Glib::ustring &uri = (*photo)->get_uri();
        Glib::ustring storagePrefix
                            = uri.substr(0, uri.find( ":" ) );
        StorageMap::iterator storage
                        = currentStorageSystems_.find( storagePrefix );
        if( storage != currentStorageSystems_.end() )
        {
            (*photo)->set_disk_file_path(
                        ((*storage).second)->retrieve( *(*photo) ) );
            if( obs )
                obs->receive_event_notifiation();
        }
    }

    return photos;
}

TagList
Engine::get_tags() const throw()
{
    DBTablePtr tagsTable = database_.getTable("tags");
    DataModelPtr model = tagsTable->get_model();
    TagList tags;

    for (gint32 i = 0; i < model->get_n_rows(); i++)
    {
        TagPtr tag(new Tag());
        tag->create(model, i);
        tag->set_table( tagsTable );
        tags.push_back(tag);
    }

    return tags;
}

void
Engine::apply_tag_to_photos( PhotoList &photos, const TagPtr &tag )
{
    for( PhotoList::iterator photo = photos.begin();
         photo != photos.end(); photo++)
    {
        PhotoTag pt(
                (*photo)->get_photo_id(), tag->get_tag_id());
        pt.save( *get_db() );
    }
    return;
}

DatePhotoInfoList
Engine::get_dates_with_picture_count()
{
    return database_.get_dates_with_picture_count( observer_ );
}

DatePhotoInfoList
Engine::get_dates_with_picture_count( gint year )
{
    return database_.get_dates_with_picture_count(year, observer_ );
}

DatePhotoInfoList
Engine::get_dates_with_picture_count( gint year, gint month )
{
    return database_.get_dates_with_picture_count(year, month,
                                                  observer_ );
}

Glib::Dispatcher &
Engine::photo_import_begin() throw()
{
    return photoImportBegin_;
}

Glib::Dispatcher &
Engine::photo_import_end() throw()
{
    return photoImportEnd_;
}

Glib::Dispatcher &
Engine::photo_render_begin() throw()
{
    return photoRenderBegin_;
}

Glib::Dispatcher &
Engine::photo_render_end() throw()
{
    return photoRenderEnd_;
}

Glib::Dispatcher &
Engine::tag_add_begin() throw()
{
    return tagAddBegin_;
}

Glib::Dispatcher &
Engine::tag_add_end() throw()
{
    return tagAddEnd_;
}

Glib::Dispatcher &
Engine::criterion_changed() throw()
{
    return criterionChanged_;
}

sigc::signal<void, const Gtk::TreeIter &> &
Engine::item_activated() throw()
{
    return itemActivated_;
}

sigc::signal<void, const PhotoList &> &
Engine::item_edit() throw()
{
    return itemEdit_;
}

sigc::signal<void, Engine &> &
Engine::renderer_changed() throw()
{
    return rendererChanged_;
}

sigc::signal<void> &
Engine::selection_changed() throw()
{
    return selectionChanged_;
}

void
Engine::set_current_storage_systems(const Engine::StorageMap & storages)
{
    currentStorageSystems_ = storages;
}

void
Engine::add_current_storage_system(const Glib::ustring & prefix,
                                   const IStoragePtr & storage)
{
    currentStorageSystems_.insert(std::make_pair(prefix, storage));
}

RendererPtr
Engine::get_current_renderer() throw()
{
    return currentRenderer_;
}

void
Engine::set_current_renderer(const RendererPtr & renderer)
{
    currentRenderer_ = renderer;
    rendererChanged_.emit(*this);
}

IStoragePtr
Engine::get_current_storage_system(const Glib::ustring & prefix) const
                                   throw()
{
    const StorageMap::const_iterator it
        = currentStorageSystems_.find(prefix);

    if (currentStorageSystems_.end() == it)
    {
        return IStoragePtr();
    }

    return it->second;
}

PhotoList
Engine::get_photos() throw()
{
    Glib::Mutex::Lock lock(mutex_);
    return photos_;
}

} //namespace Solang
