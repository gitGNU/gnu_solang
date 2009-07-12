/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
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

#include <gdkmm.h>
#include <glibmm/i18n.h>
#include <gtkimageview/gtkimagescrollwin.h>
#include <gtkimageview/gtkimageview.h>
#include <gtk/gtk.h>

#include "application.h"
#include "browser-model-column-record.h"
#include "editable-photo.h"
#include "editor-renderer.h"
#include "engine.h"
#include "i-plugin.h"
#include "main-window.h"
#include "photo.h"
#include "photo-search-criteria.h"
#include "thumbbuf-maker.h"
#include "thumbnail.h"
#include "types.h"

namespace Solang
{

static const std::string uiFile
    = PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/ui/"
          PACKAGE_TARNAME"-editor-renderer.ui";

static void
image_view_on_scroll_event(GtkImageView * view,
                           GdkScrollDirection direction,
                           gpointer user_data) throw()
{
    if (0 == user_data)
    {
        g_warning("Not an instance of EditorRenderer");
        return;
    }

    EditorRenderer * editor_renderer
        = static_cast<EditorRenderer *>(user_data);

    editor_renderer->on_scroll_event(direction);
}

EditorRenderer::EditorRenderer() throw() :
    Renderer(),
    sigc::trackable(),
    application_(NULL),
    actionGroup_(Gtk::ActionGroup::create()),
    uiID_(0),
    dockItemName_("editor-dock-item"),
    dockItemTitle_(_("Editor")),
    dockItemBehaviour_(GDL_DOCK_ITEM_BEH_NO_GRIP),
    editor_(),
    dockItem_(NULL),
    vPane_(),
    imageView_(0),
    imageScrollWin_(0),
    imagesArea_( false, 6 ),
    btPrev_(),
    left_( Gtk::ARROW_LEFT, Gtk::SHADOW_ETCHED_IN ),
    iconWindow_(),
    editablePhotosView_(),
    iconRenderer_(),
    btNext_(),
    right_( Gtk::ARROW_RIGHT, Gtk::SHADOW_ETCHED_IN ),
    columns_(),
    photosModel_(
        Gtk::ListStore::create( columns_ )),
    pageNum_(-1),
    signalInitEnd_(),
    signalItemActivated_(),
    signalSwitchPage_(),
    signalPhotoSelected_()
{
    actionGroup_->add(
        Gtk::Action::create(
            "ActionMenuGo", _("_Go")));

    {
        ActionPtr action = Gtk::Action::create(
            "ActionViewZoomIn", Gtk::Stock::ZOOM_IN,
            _("_Zoom In"),
            _("Enlarge the photo"));

        action->property_short_label().set_value(_("In"));
        action->property_is_important().set_value(true);
        actionGroup_->add(
            action, Gtk::AccelKey("<control>plus"),
            sigc::mem_fun(*this,
                          &EditorRenderer::on_action_view_zoom_in));
    }

    {
        ActionPtr action = Gtk::Action::create(
            "ActionViewZoomOut", Gtk::Stock::ZOOM_OUT,
            _("Zoom _Out"),
            _("Shrink the photo"));

        action->property_short_label().set_value(_("Out"));
        action->property_is_important().set_value(true);
        actionGroup_->add(
            action, Gtk::AccelKey("<control>minus"),
            sigc::mem_fun(*this,
                &EditorRenderer::on_action_view_zoom_out));
    }

    {
        ActionPtr action = Gtk::Action::create(
            "ActionViewNormalSize", Gtk::Stock::ZOOM_100,
            _("_Normal Size"),
            _("Show the photo at its normal size"));

        action->property_short_label().set_value(_("Normal"));
        action->property_is_important().set_value(true);
        actionGroup_->add(
            action, Gtk::AccelKey("<control>0"),
            sigc::mem_fun(*this,
                &EditorRenderer::on_action_view_normal_size));
    }

    {
        ActionPtr action = Gtk::Action::create(
            "ActionViewBestFit", Gtk::Stock::ZOOM_FIT,
            _("Best _Fit"),
            _("Fit the photo to the window"));

        action->property_short_label().set_value(_("Fit"));
        action->property_is_important().set_value(true);
        actionGroup_->add(
            action,
            sigc::mem_fun(*this,
                &EditorRenderer::on_action_view_best_fit));
    }

    {
        ActionPtr action = Gtk::Action::create(
            "ActionViewReload", Gtk::Stock::REFRESH,
            _("_Reload Photo"),
            _("Reload current photo"));

        action->property_short_label().set_value(_("Reload"));
        action->property_is_important().set_value(true);
        actionGroup_->add(
            action, Gtk::AccelKey("<control>R"),
            sigc::mem_fun(*this,
                          &EditorRenderer::on_action_view_reload));
    }

    {
        ActionPtr action = Gtk::Action::create(
            "ActionGoPrevious", Gtk::Stock::GO_BACK,
            _("_Previous Photo"),
            _("Go to the previous photo in the collection"));

        action->property_short_label().set_value(_("Previous"));
        action->property_is_important().set_value(true);
        actionGroup_->add(
            action, Gtk::AccelKey("<alt>Left"),
            sigc::mem_fun(*this,
                          &EditorRenderer::on_action_go_previous));
    }

    {
        ActionPtr action = Gtk::Action::create(
            "ActionGoNext", Gtk::Stock::GO_FORWARD,
            _("_Next Photo"),
            _("Go to the next photo in the collection"));

        action->property_short_label().set_value(_("Next"));
        action->property_is_important().set_value(true);
        actionGroup_->add(
            action, Gtk::AccelKey("<alt>Right"),
            sigc::mem_fun(*this,
                          &EditorRenderer::on_action_go_next));
    }

    actionGroup_->add(
        Gtk::Action::create(
            "ActionGoFirst", Gtk::Stock::GOTO_FIRST,
            _("_First Photo"),
            _("Go to the first photo in the collection")),
        Gtk::AccelKey("<alt>Home"),
        sigc::mem_fun(*this, &EditorRenderer::on_action_go_first));

    actionGroup_->add(
        Gtk::Action::create(
            "ActionGoLast", Gtk::Stock::GOTO_LAST,
            _("_Last Photo"),
            _("Go to the last photo in the collection")),
        Gtk::AccelKey("<alt>End"),
        sigc::mem_fun(*this, &EditorRenderer::on_action_go_last));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelGoPrevious", Gtk::StockID(),
            _("_Previous Photo"),
            _("Go to the previous photo in the collection")),
        Gtk::AccelKey("Left"),
        sigc::mem_fun(*this,
                      &EditorRenderer::on_action_go_previous));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelGoNext", Gtk::StockID(),
            _("_Next Photo"),
            _("Go to the next photo in the collection")),
        Gtk::AccelKey("Right"),
        sigc::mem_fun(*this, &EditorRenderer::on_action_go_next));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelGoFirst", Gtk::StockID(),
            _("_First Photo"),
            _("Go to the first photo in the collection")),
        Gtk::AccelKey("Home"),
        sigc::mem_fun(*this, &EditorRenderer::on_action_go_first));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelGoLast", Gtk::StockID(),
            _("_Last Photo"),
            _("Go to the last photo in the collection")),
        Gtk::AccelKey("End"),
        sigc::mem_fun(*this, &EditorRenderer::on_action_go_last));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelViewZoomIn", Gtk::StockID(),
            _("_Zoom In"),
            _("Enlarge the photo")),
        Gtk::AccelKey("plus"),
        sigc::mem_fun(*this,
                      &EditorRenderer::on_action_view_zoom_in));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelViewZoomIn1", Gtk::StockID(),
            _("_Zoom In"),
            _("Enlarge the photo")),
        Gtk::AccelKey("KP_Add"),
        sigc::mem_fun(*this,
                      &EditorRenderer::on_action_view_zoom_in));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelViewZoomIn2", Gtk::StockID(),
            _("_Zoom In"),
            _("Enlarge the photo")),
        Gtk::AccelKey("equal"),
        sigc::mem_fun(*this,
                      &EditorRenderer::on_action_view_zoom_in));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelViewZoomOut", Gtk::StockID(),
            _("Zoom _Out"),
            _("Shrink the photo")),
        Gtk::AccelKey("minus"),
        sigc::mem_fun(*this,
                      &EditorRenderer::on_action_view_zoom_out));

    actionGroup_->add(
        Gtk::Action::create(
            "AccelViewZoomOut1", Gtk::StockID(),
            _("Zoom _Out"),
            _("Shrink the photo")),
        Gtk::AccelKey("KP_Subtract"),
        sigc::mem_fun(*this,
                      &EditorRenderer::on_action_view_zoom_out));


    {
        imageView_ = gtk_image_view_new();
        if (0 == imageView_)
        {
            return;
        }

        GtkBindingSet * binding_set = gtk_binding_set_by_class(
            GTK_IMAGE_VIEW_GET_CLASS(imageView_));

        // Get rid of some of the default keybindings.

        gtk_binding_entry_remove(binding_set, GDK_KP_Add,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_equal,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_plus,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_KP_Subtract,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_minus,
                                 static_cast<GdkModifierType>(0));

        gtk_binding_entry_remove(binding_set, GDK_Right,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_Left,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_Down,
                                 static_cast<GdkModifierType>(0));
        gtk_binding_entry_remove(binding_set, GDK_Up,
                                 static_cast<GdkModifierType>(0));

        Gtk::Widget * image_view = Glib::wrap(GTK_WIDGET(imageView_),
                                              false);

        g_signal_connect(GTK_IMAGE_VIEW(imageView_),
                         "mouse-wheel-scroll",
                         G_CALLBACK(image_view_on_scroll_event),
                         this);
        btPrev_.signal_clicked().connect( sigc::mem_fun(*this,
                          &EditorRenderer::on_action_go_previous));
        btNext_.signal_clicked().connect( sigc::mem_fun(*this,
                          &EditorRenderer::on_action_go_next));
    }

    {
        imageScrollWin_ = gtk_image_scroll_win_new(
                              GTK_IMAGE_VIEW(imageView_));
        if (0 == imageScrollWin_)
        {
            return;
        }
        gtk_paned_pack1( GTK_PANED( vPane_.gobj() ), imageScrollWin_,
                        TRUE, FALSE );
        gtk_widget_show_all(GTK_WIDGET(imageScrollWin_));
    }
    btPrev_.add( left_ );
    btPrev_.set_relief( Gtk::RELIEF_NONE );
    imagesArea_.pack_start( btPrev_, Gtk::PACK_SHRINK, 2 );
    vPane_.pack2( imagesArea_, false, true );
    editablePhotosView_.set_size_request( -1, 100 );
//    editablePhotosView_.set_item_height( 100 );
//    editablePhotosView_.set_pixbuf_column( columns_.buffer_ );
    editablePhotosView_.set_model( photosModel_ );

    //Setup pixbuf renderer
    GtkCellLayout *iconLayout
                     = GTK_CELL_LAYOUT(editablePhotosView_.gobj());
    GtkCellRenderer *renderer
                    = GTK_CELL_RENDERER( iconRenderer_.gobj() );
    gtk_cell_layout_pack_start( iconLayout, renderer, FALSE );
    gtk_cell_layout_set_attributes( iconLayout, renderer,
                    "pixbuf", 0, NULL  );

    iconRenderer_.property_height().set_value( 100 );

    editablePhotosView_.set_orientation( Gtk::ORIENTATION_HORIZONTAL);
    editablePhotosView_.set_selection_mode( Gtk::SELECTION_SINGLE );
    editablePhotosView_.set_spacing( 0 );
    editablePhotosView_.set_row_spacing( 8 );
    editablePhotosView_.set_column_spacing( 8 );
    editablePhotosView_.set_has_tooltip( false );

    iconWindow_.add( editablePhotosView_ );
    iconWindow_.set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_NEVER);
    imagesArea_.pack_start( iconWindow_, Gtk::PACK_EXPAND_WIDGET, 0 );
    btNext_.add( right_ );
    btNext_.set_relief( Gtk::RELIEF_NONE );
    imagesArea_.pack_start( btNext_, Gtk::PACK_SHRINK, 2 );

    dockItem_ = gdl_dock_item_new_with_stock(dockItemName_.c_str(),
                    dockItemTitle_.c_str(),
                    Gtk::Stock::EDIT.id,
                    dockItemBehaviour_);
    gtk_container_add(GTK_CONTAINER(dockItem_),
                    GTK_WIDGET(vPane_.gobj()));
    vPane_.show_all_children();
}

EditorRenderer::~EditorRenderer() throw()
{
    //g_object_unref(dockItem_);
}

void
EditorRenderer::init(Application & application) throw()
{
    application_ = &application;

    Engine & engine = application.get_engine();

    editor_.init( application );

    editor_.edit_action_performed().connect(
            sigc::mem_fun(*this,
                        &EditorRenderer::refresh_image));

    editablePhotosView_.signal_selection_changed().connect(
            sigc::mem_fun(*this,
                    &EditorRenderer::on_photo_selection_changed));

    signalPhotoSelected_ = editablePhotosView_.signal_item_activated().connect(
            sigc::mem_fun(*this,
                        &EditorRenderer::on_photo_activated));

    signalItemActivated_
        = engine.item_edit().connect(
              sigc::mem_fun(*this,
                            &EditorRenderer::renderPhotoList));

    MainWindow & main_window = application.get_main_window();
    main_window.add_dock_object_center(GDL_DOCK_OBJECT(dockItem_));

    signalInitEnd_
        = application.init_end().connect(
              sigc::mem_fun(*this,
                            &EditorRenderer::on_init_end));

    // initialized_.emit(*this);
}

void
EditorRenderer::render(const PhotoPtr & photo) throw()
{
}

void
EditorRenderer::render(const PhotoList & photos) throw()
{
    renderPhotoList( photos );
}

void
EditorRenderer::final(Application & application) throw()
{
    signalItemActivated_.disconnect();
    signalSwitchPage_.disconnect();
    signalPhotoSelected_.disconnect();

    MainWindow & main_window = application.get_main_window();
    const Glib::RefPtr<Gtk::UIManager> & ui_manager
        = main_window.get_ui_manager();

    editor_.final( application );

    if (0 != uiID_)
    {
        ui_manager->remove_action_group(actionGroup_);
        ui_manager->remove_ui(uiID_);
        uiID_ = 0;
    }

    // finalized_.emit(*this);
}

PhotoList
EditorRenderer::get_current_selection() throw()
{
    const Gtk::TreeModel::iterator & iter
        = application_->get_list_store_iter();
    Gtk::TreeModel::Row row = *iter;

    const EditablePhotoPtr photo = row[ columns_.photo_ ];

    PhotoList photos;
    photos.push_back(photo->get_photo());

    return photos;
}

std::string
EditorRenderer::get_name() const throw()
{
    return "editor-renderer";
}

void
EditorRenderer::receive_plugin(IPlugin & plugin) throw()
{
    plugin.visit_renderer(*this);
}

void
EditorRenderer::on_action_go_previous() throw()
{
    if (false == currentItem_ )
    {
        if (0 != imageScrollWin_)
        {
            //gtk_container_remove(GTK_CONTAINER(dockItem_),
            //                     imageScrollWin_);
        }

        imageView_ = 0;
        imageScrollWin_ = 0;
        return;
    }

    if (true == currentItem_.equal(photosModel_->children().begin()))
    {
        currentItem_ = photosModel_->children().end();
    }
    currentItem_--;

    Gtk::TreeModel::Path path = photosModel_->get_path( currentItem_ );
    editablePhotosView_.set_cursor( path, iconRenderer_, false );
    editablePhotosView_.item_activated( path );
    editablePhotosView_.grab_focus();
}

void
EditorRenderer::on_action_go_next() throw()
{
    if (false == currentItem_ )
    {
        if (0 != imageScrollWin_)
        {
            //gtk_container_remove(GTK_CONTAINER(dockItem_),
            //                     imageScrollWin_);
        }

        imageView_ = 0;
        imageScrollWin_ = 0;
        return;
    }

    currentItem_++;
    if( currentItem_ == photosModel_->children().end() )
        currentItem_ = photosModel_->children().begin();

    Gtk::TreeModel::Path path = photosModel_->get_path( currentItem_ );
    editablePhotosView_.set_cursor( path, iconRenderer_, false );
    editablePhotosView_.item_activated( path );
    editablePhotosView_.grab_focus();
}

void
EditorRenderer::on_action_go_first() throw()
{
    if (false == currentItem_ )
    {
        if (0 != imageScrollWin_)
        {
            //gtk_container_remove(GTK_CONTAINER(dockItem_),
            //                     imageScrollWin_);
        }

        imageView_ = 0;
        imageScrollWin_ = 0;
        return;
    }

    currentItem_ = photosModel_->children().begin();

    Gtk::TreeModel::Path path = photosModel_->get_path( currentItem_ );
    editablePhotosView_.set_cursor( path, iconRenderer_, false );
    editablePhotosView_.item_activated( path );
    editablePhotosView_.grab_focus();
}

void
EditorRenderer::on_action_go_last() throw()
{
    if (false == currentItem_ )
    {
        if (0 != imageScrollWin_)
        {
            //gtk_container_remove(GTK_CONTAINER(dockItem_),
            //                     imageScrollWin_);
        }

        imageView_ = 0;
        imageScrollWin_ = 0;
        return;
    }

    currentItem_ = photosModel_->children().end();
    currentItem_--;

    Gtk::TreeModel::Path path = photosModel_->get_path( currentItem_ );
    editablePhotosView_.set_cursor( path, iconRenderer_, false );
    editablePhotosView_.item_activated( path );
    editablePhotosView_.grab_focus();
}

void
EditorRenderer::on_action_view_reload() throw()
{
    if (false == currentItem_ )
    {
        if (0 != imageScrollWin_)
        {
            //gtk_container_remove(GTK_CONTAINER(dockItem_),
            //                     imageScrollWin_);
        }

        imageView_ = 0;
        imageScrollWin_ = 0;
        return;
    }

    Gtk::TreeModel::Path path = photosModel_->get_path( currentItem_ );
    editablePhotosView_.set_cursor( path, iconRenderer_, false );
    editablePhotosView_.item_activated( path );
    editablePhotosView_.grab_focus();
}

void
EditorRenderer::on_action_view_best_fit() throw()
{
    if (0 == imageView_)
    {
        return;
    }

    if (false == GTK_IS_IMAGE_VIEW(imageView_))
    {
        g_warning("Not a GtkImageView");
        return;
    }

    gtk_image_view_set_fitting(GTK_IMAGE_VIEW(imageView_), TRUE);
}

void
EditorRenderer::on_action_view_normal_size() throw()
{
    if (0 == imageView_)
    {
        return;
    }

    if (false == GTK_IS_IMAGE_VIEW(imageView_))
    {
        g_warning("Not a GtkImageView");
        return;
    }

    gtk_image_view_set_zoom(GTK_IMAGE_VIEW(imageView_), 1.0);
}

void
EditorRenderer::on_action_view_zoom_in() throw()
{
    if (0 == imageView_)
    {
        return;
    }

    if (false == GTK_IS_IMAGE_VIEW(imageView_))
    {
        g_warning("Not a GtkImageView");
        return;
    }

    gtk_image_view_zoom_in(GTK_IMAGE_VIEW(imageView_));
}

void
EditorRenderer::on_action_view_zoom_out() throw()
{
    if (0 == imageView_)
    {
        return;
    }

    if (false == GTK_IS_IMAGE_VIEW(imageView_))
    {
        g_warning("Not a GtkImageView");
        return;
    }

    gtk_image_view_zoom_out(GTK_IMAGE_VIEW(imageView_));
}

void
EditorRenderer::on_init_end(Application & application) throw()
{
    MainWindow & main_window = application.get_main_window();
    Gtk::Notebook * notebook = main_window.get_notebook_center();

    if (0 == notebook)
    {
        g_warning("0 == notebook");
        return;
    }

    pageNum_ = notebook->page_num(*Glib::wrap(dockItem_, false));

    signalSwitchPage_
        = notebook->signal_switch_page().connect(
              sigc::mem_fun(*this,
                            &EditorRenderer::on_switch_page));

    signalInitEnd_.disconnect();
}

void
EditorRenderer::on_scroll_event(GdkScrollDirection direction) throw()
{
    switch (direction)
    {
        case GDK_SCROLL_UP:
            on_action_view_zoom_in();
            break;

        case GDK_SCROLL_DOWN:
            on_action_view_zoom_out();
            break;

        default:
            break;
    }
}

void
EditorRenderer::on_switch_page(GtkNotebookPage * notebook_page,
                                 guint page_num) throw()
{
    MainWindow & main_window = application_->get_main_window();
    const Glib::RefPtr<Gtk::UIManager> & ui_manager
        = main_window.get_ui_manager();

    // NB: Sometimes this gets invoked more than once consecutively
    //     -- no idea why (FIXME). Better safe than sorry.

    if (pageNum_ == static_cast<gint>(page_num))
    {
        Engine & engine = application_->get_engine();
        RendererPtr renderer = application_->get_renderer(
                                   "editor-renderer");
        engine.set_current_renderer(renderer);

        if (0 == uiID_)
        {
            uiID_ = ui_manager->add_ui_from_file(uiFile);
            if (0 == uiID_)
            {
                // FIXME: error condition.
            }

            ui_manager->insert_action_group(actionGroup_);
            editor_.register_ui();
        }
    }
    else
    {
        if (0 != uiID_)
        {
            photosModel_->clear();
            gtk_image_view_set_pixbuf( GTK_IMAGE_VIEW(imageView_),
                                    0 , TRUE );
            editor_.save();
			editor_.set_current_photo( EditablePhotoPtr() );
            ui_manager->remove_action_group(actionGroup_);
            ui_manager->remove_ui(uiID_);
            editor_.unregister_ui();
            uiID_ = 0;
        }
    }
}

void
EditorRenderer::renderPhotoList(const PhotoList & photos) throw()
{
    EditablePhotoList editablePhotos;
    for( PhotoList::const_iterator it = photos.begin();
                                        it != photos.end(); it++ )
    {
        editablePhotos.push_back(
                    EditablePhotoPtr( new EditablePhoto( *it ) ) );
    }
    renderSelectedPhotos( editablePhotos );
    MainWindow & main_window = application_->get_main_window();
    main_window.present_dock_object(GDL_DOCK_OBJECT(dockItem_));

}
void
EditorRenderer::renderSelectedPhotos(const EditablePhotoList & photos) throw()
{
    signalPhotoSelected_.block();
    for( EditablePhotoList::const_iterator photo = photos.begin();
                    photo != photos.end(); photo ++ )
    {
        ThumbbufMaker thumbbuf_maker( 128, 96 );
        Gtk::TreeModel::Row row = *(photosModel_->append());
        row[ columns_.buffer_ ] = (*photo)->get_photo()->get_thumbnail_buffer();
                    //->scale_simple( 128, 96, Gdk::INTERP_BILINEAR );
        row[ columns_.photo_ ] = (*photo);
    }
    signalPhotoSelected_.unblock();
    if( !photos.empty() )
    {
        currentItem_ = photosModel_->children().begin();
        on_action_go_first();
    }
}

void
EditorRenderer::render(const EditablePhotoPtr & photo) throw()
try
{
    PixbufPtr pixbuf = photo->get_buffer();

    if( !pixbuf )
    {
        Engine & engine = application_->get_engine();
        const IStoragePtr & storage
                        = engine.get_current_storage_system("file");
        photo->get_photo()->set_disk_file_path(storage);
        std::string path
            = Glib::filename_from_utf8(
                        photo->get_photo()->get_disk_file_path());
        pixbuf = Gdk::Pixbuf::create_from_file(path);
        photo->set_buffer( pixbuf );
    }


    gtk_image_view_set_pixbuf(GTK_IMAGE_VIEW(imageView_),
                              pixbuf->gobj(), TRUE);
}
catch (const Glib::ConvertError & e)
{
    g_warning("%s", e.what().c_str());
    return;
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

void
EditorRenderer::refresh_image() throw()
{
    Gtk::TreeModel::Row row = *currentItem_;
    EditablePhotoPtr photo = row[ columns_.photo_ ];
    ThumbbufMaker thumbbuf_maker( 128, 96 );
    row[ columns_.buffer_ ] = photo->get_photo()->get_thumbnail_buffer();
    gtk_image_view_set_pixbuf(GTK_IMAGE_VIEW(imageView_),
                              photo->get_buffer()->gobj(), TRUE);
}

void
EditorRenderer::on_photo_selection_changed() throw()
{
    std::vector<Gtk::TreeModel::Path> items
                            = editablePhotosView_.get_selected_items();
    if( items.empty() )
        return;

    const Gtk::TreeModel::Path & path = *items.begin();
    editablePhotosView_.item_activated( path );
}

void
EditorRenderer::on_photo_activated(
                        const Gtk::TreeModel::Path& path) throw()
{
    currentItem_ = photosModel_->get_iter( path );
    Gtk::TreeModel::Row row = *currentItem_;
    EditablePhotoPtr photo = row[ columns_.photo_ ];
    editor_.set_current_photo( photo );
    render( photo );
}

} // namespace Solang