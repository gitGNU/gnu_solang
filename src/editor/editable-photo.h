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

#ifndef SOLANG_EDITABLE_PHOTO_H
#define SOLANG_EDITABLE_PHOTO_H

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <gdkmm.h>

#include "edit-action-history.h"
#include "engine.h"
#include "types.h"

namespace Solang
{

class EditablePhoto
{
    public:
        class Searcher
        {
            public:
                Searcher( const PhotoPtr &photo )
                    :photo_( photo )
                {
                }

                bool operator ()( const EditablePhotoPtr &lhs )
                {
                    return lhs->get_photo()->get_photo_id()
                                == photo_->get_photo_id();
                }

            private:
                PhotoPtr photo_;
        };

    public:
        EditablePhoto( const PhotoPtr &photo ) throw();
        ~EditablePhoto();

        inline const PhotoPtr &
        get_photo() const throw();

        void
        set_photo( const PhotoPtr & ) throw();

        inline const PixbufPtr &
        get_buffer() const throw();

        void
        set_buffer(const PixbufPtr &);

        inline Exiv2::ExifData &  //Modifiable
        get_exif_data() throw();

        inline Exiv2::XmpData &  //Modifiable
        get_xmp_data() throw();

        void
        save( Engine &engine ) throw(Error);

        inline bool
        get_to_save() const throw();

        void
        set_to_save( bool ) throw();

        void
        apply_action( const EditActionPtr &action ) throw(Error);

        void
        undo_last_action( ) throw(Error);

        void
        redo_last_action( ) throw(Error);

    private:

        void
        setup_photo_for_edit() throw();

        PhotoPtr photo_;
        PixbufPtr buffer_;
        Exiv2::Image::AutoPtr image_;
        bool toSave_;
        EditActionHistory appliedActions_;
};

inline const PhotoPtr &
EditablePhoto::get_photo() const throw()
{
    return photo_;
}

inline const PixbufPtr &
EditablePhoto::get_buffer() const throw()
{
    return buffer_;
}

inline Exiv2::ExifData &  //Modifiable
EditablePhoto::get_exif_data() throw()
{
    return image_->exifData();
}

inline Exiv2::XmpData &  //Modifiable
EditablePhoto::get_xmp_data() throw()
{
    return image_->xmpData();
}

inline bool
EditablePhoto::get_to_save() const throw()
{
    return toSave_;
}

} //namespace Solang
#endif
