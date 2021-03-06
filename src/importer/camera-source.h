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

#ifndef SOLANG_CAMERA_SOURCE_H
#define SOLANG_CAMERA_SOURCE_H

#include <glibmm.h>
#include <gtkmm.h>

#include "photo-source.h"
#include "camera-import-widget.h"

namespace Solang
{

class CameraSource :
    public PhotoSource
{
    public:
        CameraSource() throw();

        virtual
        ~CameraSource() throw();

        virtual void
        init(Application & application) throw();

        virtual void
        final(Application & application) throw();

        virtual PhotoPtr
        import(const PhotoPtr & photo, const IStoragePtr & storage,
               const TagList &tags, Database & db,
               const ProgressObserverPtr & observer) throw();

        virtual PhotoList
        import(const PhotoList & photos, const IStoragePtr & storage,
               const TagList &tags, Database & db,
               const ProgressObserverPtr & observer) throw();

        virtual PhotoList
        import(const IStoragePtr & storage, const TagList &tags,
               Database & db, const ProgressObserverPtr & observer
                                                            ) throw();

        virtual sigc::signal<void, bool> &
        init_end() throw();

        virtual void
        read_selection() throw();

        virtual Gtk::Widget &
        get_browser() throw();

        virtual Glib::ustring
        get_label() const throw();

        virtual Glib::ustring
        get_name() const throw();

        virtual gint
        get_options() const throw();

        virtual Gtk::StockID
        get_stock_id() const throw();

    protected:
        GPhotoContext gpContext_;

        Glib::RefPtr<Gtk::IconFactory> iconFactory_;

        CameraImportWidget select_;

        sigc::signal<void, bool> initEnd_;

    private:
        void
        create_photo_list( PhotoList &files );

        void 
        download_file_from_camera( const PhotoPtr &photo );
};

} // namespace Solang

#endif // SOLANG_CAMERA_SOURCE_H
