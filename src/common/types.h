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

#ifndef SOLANG_TYPES_H
#define SOLANG_TYPES_H

#include <tr1/memory>

#include <gdkmm.h>
#include <glibmm.h>
#include <gtkmm.h>
#include <libgdamm.h>

namespace Solang
{

class Application;
typedef const Application * ConstApplicationPtr;
typedef Application * ApplicationPtr;

class Database;
typedef const Database * ConstDatabasePtr;
typedef Database * DatabasePtr;

class DBObject;
typedef std::tr1::shared_ptr<const DBObject> ConstDBObjectPtr;
typedef std::tr1::shared_ptr<DBObject> DBObjectPtr;

class DBTable;
typedef std::tr1::shared_ptr<const DBTable> ConstDBTablePtr;
typedef std::tr1::shared_ptr<DBTable> DBTablePtr;

class ExifDataKey;
typedef std::tr1::shared_ptr<ExifDataKey> ExifDataKeyPtr;
typedef std::list<ExifDataKeyPtr> ExifDataKeyList;

class IPhotoDestination;
typedef std::tr1::shared_ptr<const IPhotoDestination>
    ConstIPhotoDestinationPtr;
typedef std::tr1::shared_ptr<IPhotoDestination> IPhotoDestinationPtr;
typedef std::list<IPhotoDestinationPtr> IPhotoDestinationList;

class ExifData;
typedef std::tr1::shared_ptr<ExifData> ExifDataPtr;

class IPhotoSource;
typedef std::tr1::shared_ptr<const IPhotoSource> ConstIPhotoSourcePtr;
typedef std::tr1::shared_ptr<IPhotoSource> IPhotoSourcePtr;
typedef std::list<IPhotoSourcePtr> IPhotoSourceList;

class IPlugin;
typedef std::tr1::shared_ptr<const IPlugin> ConstIPluginPtr;
typedef std::tr1::shared_ptr<IPlugin> IPluginPtr;

class Photo;
typedef std::tr1::shared_ptr<const Photo> ConstPhotoPtr;
typedef std::tr1::shared_ptr<Photo> PhotoPtr;
typedef Photo * UnrefPhotoPtr;
typedef std::list<PhotoPtr> PhotoList;

class PhotoSearchCriteria;
typedef std::tr1::shared_ptr<const PhotoSearchCriteria>
    ConstPhotoSearchCriteriaPtr;
typedef std::tr1::shared_ptr<PhotoSearchCriteria>
    PhotoSearchCriteriaPtr;
typedef std::list<PhotoSearchCriteriaPtr> PhotoSearchCriteriaList;

class ProgressDialog;
typedef Glib::RefPtr<const ProgressDialog> ConstProgressDialogPtr;
typedef Glib::RefPtr<ProgressDialog> ProgressDialogPtr;

class ProgressObserver;
typedef std::tr1::shared_ptr<const ProgressObserver>
    ConstProgressObserverPtr;
typedef std::tr1::shared_ptr<ProgressObserver> ProgressObserverPtr;

class Renderer;
typedef std::tr1::shared_ptr<const Renderer> ConstRendererPtr;
typedef std::tr1::shared_ptr<Renderer> RendererPtr;
typedef std::list<RendererPtr> RendererList;

class IStorage;
typedef std::tr1::shared_ptr<const IStorage> ConstIStoragePtr;
typedef std::tr1::shared_ptr<IStorage> IStoragePtr;

class Tag;
typedef std::tr1::shared_ptr<const Tag> ConstTagPtr;
typedef std::tr1::shared_ptr<Tag> TagPtr;
typedef std::list<TagPtr> TagList;

class Thumbnail;
typedef std::tr1::shared_ptr<const Thumbnail> ConstThumbnailPtr;
typedef std::tr1::shared_ptr<Thumbnail> ThumbnailPtr;

typedef Glib::RefPtr<const Gdk::DragContext> ConstDragContextPtr;
typedef Glib::RefPtr<Gdk::DragContext> DragContextPtr;

typedef Glib::RefPtr<const Gdk::Pixbuf> ConstPixbufPtr;
typedef Glib::RefPtr<Gdk::Pixbuf> PixbufPtr;

typedef Glib::RefPtr<const Gnome::Gda::DataModelQuery>
    ConstDataModelPtr;
typedef Glib::RefPtr<Gnome::Gda::DataModelQuery> DataModelPtr;

typedef Glib::RefPtr<const Gtk::ActionGroup> ConstActionGroupPtr;
typedef Glib::RefPtr<Gtk::ActionGroup> ActionGroupPtr;

typedef Glib::RefPtr<const Gtk::ListStore> ConstListStorePtr;
typedef Glib::RefPtr<Gtk::ListStore> ListStorePtr;

typedef Glib::RefPtr<const Gtk::Menu> ConstMenuPtr;
typedef Glib::RefPtr<Gtk::Menu> MenuPtr;

class SearchCriterionSource;
typedef SearchCriterionSource * SearchCriterionSourcePtr;
typedef std::list<SearchCriterionSourcePtr> SearchCriterionSourceList;

typedef Glib::RefPtr<const Gtk::TreeModel> ConstTreeModelPtr;
typedef Glib::RefPtr<Gtk::TreeModel> TreeModelPtr;

typedef Glib::RefPtr<const Gtk::TreePath> ConstTreePathPtr;
typedef Glib::RefPtr<Gtk::TreePath> TreePathPtr;
typedef std::list<TreePathPtr> TreePathList;

typedef Glib::RefPtr<const Gtk::UIManager> ConstUIManagerPtr;
typedef Glib::RefPtr<Gtk::UIManager> UIManagerPtr;

} // namespace Solang

#endif // SOLANG_TYPES_H
