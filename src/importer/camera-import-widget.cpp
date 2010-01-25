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

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <glibmm/i18n.h>

#include "camera-import-widget.h"

namespace Solang
{
void CameraImportWidget::populate()
{
    //Cleanup context
    gpContext_.cleanup();

    //Cleanup lists
    camerasModel_->clear();
    picturesModel_->clear();

    progress_.set_text("");
    progress_.set_fraction(0.0);

    //Load cameras
    gp_camera_new (&gpContext_.camera_);
    gpContext_.context_ = gp_context_new ();

    {
        GPPortInfoList *list = NULL;
        if( GP_OK > gp_port_info_list_new( &list ) )
            return;
        if( 0 > gp_port_info_list_load( list )
                || 0 > gp_port_info_list_count( list ))
        {
            gp_port_info_list_free (list);
            return;
        }
        gpContext_.portinfoList_ = list;
    }

    {
        int count;
        gp_list_new( &gpContext_.cameras_ );
        gp_abilities_list_new( &gpContext_.abilitiesList_ );
        gp_abilities_list_load( gpContext_.abilitiesList_,
                                        gpContext_.context_ );
        gp_abilities_list_detect( gpContext_.abilitiesList_,
                                gpContext_.portinfoList_,
                                gpContext_.cameras_,
                                gpContext_.context_ );

        if( 0 > ( count = gp_list_count(
                                    gpContext_.cameras_ ) ) )
        {
            gp_list_free( gpContext_.cameras_ );
            gpContext_.cameras_ = NULL;
        }

        for( int i = 0; i < count; i++ )
        {
            const char *name = NULL;
            const char *value = NULL;
            if( 0 > gp_list_get_name(
                            gpContext_.cameras_, i, &name ) )
            {
                gp_list_free( gpContext_.cameras_ );
                gpContext_.cameras_ = NULL;
            }
            if( 0 > gp_list_get_value(
                            gpContext_.cameras_, i, &value ) )
            {
                gp_list_free( gpContext_.cameras_ );
                gpContext_.cameras_ = NULL;
            }

            Gtk::TreeModel::Row row
                            = *(camerasModel_->append());
            row[ connectedCameraColumns_.cameraIndex_ ] = i;
            row[ connectedCameraColumns_.camera_ ] = name;
            row[ connectedCameraColumns_.port_ ] = value;

        }
        if( count > 0 )
            cmbConnectedCameras_.set_active( 0 );

    }
    return;
}

void
CameraImportWidget::create_camera( int index,
                const Glib::ustring &name, const Glib::ustring &port )
{

    std::cout<<"Creating camera for:"
            <<name<<": "
            <<port<<std::endl;

    Camera *cam = NULL;
    gp_camera_new( &cam );

    CameraAbilities    a;

    //Set model info
    {
        int model
            = gp_abilities_list_lookup_model (
                    gpContext_.abilitiesList_, name.c_str());
        gp_abilities_list_get_abilities(
                    gpContext_.abilitiesList_, model, &a );
        gp_camera_set_abilities( cam, a );
    }

    //Set port info
    {
        int portNum;
        GPPortInfo    pi;
        portNum = gp_port_info_list_lookup_path(
                            gpContext_.portinfoList_,
                            port.c_str() );
        gp_port_info_list_get_info(
                    gpContext_.portinfoList_, portNum, &pi );

        gp_camera_set_port_info( cam, pi );
    }

    gpContext_.camera_ = cam;
}

void
CameraImportWidget::list_pictures()
{
    progress_.set_text(_("Importing list of photos.. please wait"));
    list_pictures( "/" );
    progress_.set_text("");
}

void
CameraImportWidget::on_preview_enabled()
{
    if( true == chkEnablePreview_.get_active( ) )
    {
        if( !previewImported_ )
        {
            progress_.set_text(_("Importing previews"));
            pool_.push(
                sigc::mem_fun( *this,
                    &CameraImportWidget::save_thumbnails_for_all_files));
        }
    }
}

void
CameraImportWidget::list_pictures(const Glib::ustring &path)
{
    //progress_.set_text("Reading from " + path );

    {
        CameraList *folderList;
        gp_list_new( &folderList );
        if( 0 > gp_camera_folder_list_folders(
                        gpContext_.camera_, path.c_str(),
                        folderList, gpContext_.context_ ))
        {
            gp_list_free( folderList );
            return;
        }

        int numFolders = gp_list_count( folderList );
        for( int i = 0; i < numFolders; i++ )
        {
            const char *folder;
            gp_list_get_name( folderList, i, &folder  );
            std::ostringstream sout;
            sout<<path;
            if( path != "/" )
                sout<<"/";
            sout<<folder;

            //Get pictures in sub-folders
            list_pictures( sout.str() );
        }
    }

    {
        CameraList *files;
        gp_list_new( &files );

        if( 0 > gp_camera_folder_list_files(
                    gpContext_.camera_, path.c_str(),
                    files, gpContext_.context_ ))
        {
            gp_list_free( files );
        }

        int fileCount = gp_list_count( files );
        for( int file = 0; file < fileCount; file ++ )
        {
            const char *fileName = NULL;
            gp_list_get_name( files, file, &fileName );
            Glib::ustring folder = path + "/";

            Gtk::TreeModel::Row row
                            = *(picturesModel_->append());
            row[ cameraPhotoColumns_.selected_ ] = false;
            row[ cameraPhotoColumns_.pictureName_ ]
                                            = fileName;
            row[ cameraPhotoColumns_.picturePath_ ]
                                            = folder;
        }
    }
}


//Signal handlers

void
CameraImportWidget::on_camera_selection_changed() throw()
{
    Gtk::TreeModel::const_iterator cam
                        = cmbConnectedCameras_.get_active();
    if( cam )
    {
        Gtk::TreeModel::Row row = (*cam);
        create_camera(
                row[connectedCameraColumns_.cameraIndex_],
                row[connectedCameraColumns_.camera_ ],
                row[connectedCameraColumns_.port_ ]);

        if( gpContext_.camera_ )
        {
            list_pictures();
        }
    }
}

void
CameraImportWidget::save_thumbnails_for_all_files()
{
    Gtk::TreeModel::Children children
                = cameraPhotos_.get_model()->children();
    numPhotos_ = children.size();

    for( Gtk::TreeModel::iterator it = children.begin();
                it != children.end(); it ++ )
    {
        Gtk::TreeModel::Row row = (*it);
        save_thumbnail(
            row[ cameraPhotoColumns_.picturePath_ ],
            row[ cameraPhotoColumns_.pictureName_ ]);
        numImportedPhotos_++;
        progressed_.emit();
    }
    previewImported_ = true;

    return;
}

void
CameraImportWidget::save_thumbnail( const Glib::ustring &folder,
                                        const Glib::ustring &file )
{
    CameraFile *thumb;
    Glib::ustring tPath = "/tmp/thumb/" + folder;
    Glib::RefPtr<Gio::File> tDir = Gio::File::create_for_path( tPath );
    if( !tDir->query_exists() )
    {
        tDir->make_directory_with_parents();
    }

    Glib::ustring thumbPath = "/tmp/thumb/" + folder;
    Glib::RefPtr<Gio::File> th
                            = Gio::File::create_for_path( thumbPath );
    if( !th->query_exists() )
    {
        th->make_directory_with_parents();
    }
    thumbPath += "/"+file;
    int fd = open( thumbPath.c_str(), O_CREAT | O_WRONLY, 0644 );
    if( gp_file_new_from_fd( &thumb, fd  ) )
    {
        return;
    }
    if( GP_OK > gp_camera_file_get(
                    gpContext_.camera_, folder.c_str(),
                    file.c_str(), GP_FILE_TYPE_PREVIEW,
                    thumb, gpContext_.context_ ) )
    {
        gp_file_unref( thumb );
    }
    gp_file_free( thumb );

}

void
CameraImportWidget::show_thumbnail( const Gtk::TreeModel::Path& path,
                                        Gtk::TreeViewColumn* column)
{
    Gtk::TreeModel::iterator current
                    = picturesModel_->get_iter( path );
    Gtk::TreeModel::Row row = (*current);
    std::ostringstream sout;
    sout<<"/tmp/thumb"<<row[cameraPhotoColumns_.picturePath_]
        <<"/"<<row[cameraPhotoColumns_.pictureName_];
    imgPreview_.clear();
    imgPreview_.set( sout.str() );
}

void
CameraImportWidget::on_row_activated (
        const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Gtk::TreeModel::iterator current
                    = picturesModel_->get_iter( path );
    Gtk::TreeModel::Row row = (*current);
    row[ cameraPhotoColumns_.selected_ ]
                = !row[ cameraPhotoColumns_.selected_ ];
}

void
CameraImportWidget::on_cursor_changed()
{
    if( !chkEnablePreview_.get_active() )
    {
        return;
    }
    Gtk::TreeModel::Path path;
    Gtk::TreeViewColumn *column;
    cameraPhotos_.get_cursor( path, column );
    show_thumbnail( path, column );
}

void
CameraImportWidget::on_select_all_clicked()
{
    Gtk::TreeModel::Children children
                        = cameraPhotos_.get_model()->children();
    for( Gtk::TreeModel::iterator it = children.begin();
                it != children.end(); it ++ )
    {
        Gtk::TreeModel::Row row = (*it);
        row[ cameraPhotoColumns_.selected_ ] = true;
    }
}

void
CameraImportWidget::on_select_none_clicked()
{
    Gtk::TreeModel::Children children
                        = cameraPhotos_.get_model()->children();
    for( Gtk::TreeModel::iterator it = children.begin();
                it != children.end(); it ++ )
    {
        Gtk::TreeModel::Row row = (*it);
        row[ cameraPhotoColumns_.selected_ ] = false;
    }
}

void
CameraImportWidget::on_preview_imported()
{
    progress_.set_fraction( numImportedPhotos_ / numPhotos_ );
}

CameraImportWidget::CameraImportWidget(GPhotoContext &context)
    :Gtk::VBox( false, 6 ),
    gpContext_( context ),
    connectedCameraColumns_(),
    cameraPhotoColumns_(),
    //top_( false, 6 ),
    cameraBox_( false, 6 ),
    lblCamera_(_("Connected Cameras")),
    cmbConnectedCameras_(),
    camerasModel_(
        Gtk::ListStore::create( connectedCameraColumns_ ) ),
    getPhotos_( Gtk::Stock::EXECUTE ),
    selectionBox_( false, 6 ),
    listArea_(false, 6),
    buttonArea_(false, 6),
    photos_(),
    cameraPhotos_(),
    picturesModel_(
        Gtk::ListStore::create( cameraPhotoColumns_ ) ),
    selectAll_(_("Select All")),
    selectNone_(_("Select None")),
    previewArea_(_("Preview")),
    chkEnablePreview_(_("Enable Preview")),
    imgPreview_(),
    progress_(),
    progressed_(),
    pool_( 10, true ),
    numPhotos_( 0.0 ),
    numImportedPhotos_( 0.0 ),
    previewImported_( false )
{
    //Layout
    add( cameraBox_ );
    cameraBox_.pack_start( lblCamera_, Gtk::PACK_SHRINK );
    cameraBox_.pack_start(cmbConnectedCameras_, Gtk::PACK_SHRINK);
    cmbConnectedCameras_.set_model( camerasModel_ );
    cmbConnectedCameras_.pack_start(
                        connectedCameraColumns_.camera_);
    cmbConnectedCameras_.set_size_request( 400, -1 );
    cameraBox_.pack_start( getPhotos_, Gtk::PACK_SHRINK );
    add( selectionBox_ );
    selectionBox_.pack_start( listArea_ );
    listArea_.pack_start( photos_ );
    listArea_.pack_start( buttonArea_ );
    buttonArea_.pack_end( selectNone_, Gtk::PACK_SHRINK );
    buttonArea_.pack_end( selectAll_, Gtk::PACK_SHRINK );
    buttonArea_.pack_start(chkEnablePreview_);
    photos_.add( cameraPhotos_ );
    selectionBox_.pack_start( previewArea_ );
    previewArea_.add(imgPreview_);
    add( progress_ );
    cameraPhotos_.set_size_request(400,250);
    cameraPhotos_.set_grid_lines( Gtk::TREE_VIEW_GRID_LINES_BOTH );
    cameraPhotos_.append_column(
                "", cameraPhotoColumns_.selected_ );
    cameraPhotos_.append_column(
                "Name", cameraPhotoColumns_.pictureName_ );
    cameraPhotos_.append_column(
                "Path", cameraPhotoColumns_.picturePath_ );
    cameraPhotos_.set_headers_clickable( false );
    imgPreview_.set_size_request( 200, 200 );

    //Setup
    getPhotos_.set_label(_("Load Pictures"));
    photos_.set_policy(
            Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC );
    cameraPhotos_.set_model( picturesModel_ );
    const TreeSelectionPtr refTreeSelection
                                = cameraPhotos_.get_selection();
    refTreeSelection->set_mode( Gtk::SELECTION_MULTIPLE);
    selectAll_.set_size_request( 90, -1 );
    selectNone_.set_size_request( 90, -1 );
    chkEnablePreview_.set_active( false );

    //Bind signal handlers
    getPhotos_.signal_clicked().connect(
        sigc::mem_fun( *this,
            &CameraImportWidget::on_camera_selection_changed ));
    cameraPhotos_.signal_row_activated().connect(
        sigc::mem_fun( *this,
            &CameraImportWidget::on_row_activated));
    cameraPhotos_.signal_cursor_changed().connect(
        sigc::mem_fun( *this,
            &CameraImportWidget::on_cursor_changed));
    selectAll_.signal_clicked().connect(
        sigc::mem_fun( *this,
            &CameraImportWidget::on_select_all_clicked ));
    selectNone_.signal_clicked().connect(
        sigc::mem_fun( *this,
            &CameraImportWidget::on_select_none_clicked ));
    progressed_.connect(
        sigc::mem_fun( *this,
            &CameraImportWidget::on_preview_imported ));
    chkEnablePreview_.signal_toggled().connect(
        sigc::mem_fun( *this,
            &CameraImportWidget::on_preview_enabled));
}

CameraImportWidget::~CameraImportWidget()
{
    //cleanup gp-context will be cleaned up
}

} //namespace Solang
