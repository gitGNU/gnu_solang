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

#ifndef SOLANG_CAMERA_IMPORT_WIDGET_H
#define SOLANG_CAMERA_IMPORT_WIDGET_H

#include <gtkmm.h>
#include <glibmm.h>

#include "gp-context.h"

namespace Solang
{

class CameraImportWidget :
    public Gtk::VBox
{
    private:

        //Tree model columns:
      class CameraSelectColumns : public Gtk::TreeModel::ColumnRecord
      {
          public:

            CameraSelectColumns()
            {
                add( cameraIndex_);
                add( camera_ );
                add( port_ );
            }

            Gtk::TreeModelColumn<int> cameraIndex_;
            Gtk::TreeModelColumn<Glib::ustring> camera_;
            Gtk::TreeModelColumn<Glib::ustring> port_;
      };

   public:
      class PictureSelectColumns : public Gtk::TreeModel::ColumnRecord
      {
          public:
            PictureSelectColumns()
            {
                add( selected_ );
                add( pictureName_ );
                add( picturePath_ );
            }

            Gtk::TreeModelColumn<bool> selected_;
            Gtk::TreeModelColumn<Glib::ustring> pictureName_;
            Gtk::TreeModelColumn<Glib::ustring> picturePath_;
      };

    private:
        GPhotoContext &gpContext_;
        CameraSelectColumns connectedCameraColumns_;
        PictureSelectColumns cameraPhotoColumns_;

        //Gtk::VBox top_;

        Gtk::HBox cameraBox_;
        Gtk::Label lblCamera_;
        Gtk::ComboBox cmbConnectedCameras_;
        Glib::RefPtr<Gtk::ListStore> camerasModel_;
        Gtk::Button getPhotos_;

        //Preview and selection area
        Gtk::HBox selectionBox_;
        Gtk::VBox listArea_;
        Gtk::HBox buttonArea_;
        Gtk::ScrolledWindow photos_;
        Gtk::TreeView cameraPhotos_;
        Glib::RefPtr<Gtk::ListStore> picturesModel_;
        Gtk::Button selectAll_;
        Gtk::Button selectNone_;
        Gtk::Frame previewArea_;
        Gtk::CheckButton chkEnablePreview_;
        Gtk::Image imgPreview_;

        //A progress-bar for miscllaneous stuff
        Gtk::ProgressBar progress_;
        Glib::Dispatcher progressed_;

        Glib::ThreadPool pool_;
        double numPhotos_;
        double numImportedPhotos_;
        bool previewImported_;

    public:
        CameraImportWidget(GPhotoContext &context);

        virtual
        ~CameraImportWidget();

        void
        populate();

        inline const Gtk::TreeView &
        get_camera_photos() const throw();

        inline const PictureSelectColumns &
        get_camera_photo_columns() throw();

    protected:

        void
        create_camera(int index, const Glib::ustring & name,
                      const Glib::ustring & port);

        void
        list_pictures();

        void
        on_preview_enabled();

        void
        list_pictures(const Glib::ustring & path);

        //Signal handlers
        void
        on_camera_selection_changed() throw();

        void
        save_thumbnails_for_all_files();

        void
        save_thumbnail(const Glib::ustring & folder,
                       const Glib::ustring & file );

        void
        show_thumbnail(const Gtk::TreeModel::Path & path,
                       Gtk::TreeViewColumn* column);

        virtual void
        on_row_activated(const Gtk::TreeModel::Path & path,
                         Gtk::TreeViewColumn* column);

        virtual void
        on_cursor_changed();

        virtual void
        on_select_all_clicked();

        virtual void
        on_select_none_clicked();

        void
        on_preview_imported();
};

inline const Gtk::TreeView &
CameraImportWidget::get_camera_photos() const throw()
{
    return cameraPhotos_;
}

inline const CameraImportWidget::PictureSelectColumns &
CameraImportWidget::get_camera_photo_columns() throw()
{
    return cameraPhotoColumns_;
}

} // namespace Solang

#endif //SOLANG_CAMERA_IMPORT_WIDGET_H

