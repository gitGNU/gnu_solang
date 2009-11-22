/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * thumbnail.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * thumbnail.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * thumbnail.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_THUMBNAIL_H
#define SOLANG_THUMBNAIL_H

#include <vector>
#include <tr1/memory>

#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>
#include <glibmm.h>

#include "exif-data.h"
#include "types.h"

namespace Solang
{

class Thumbnail
{
    public:
        class Resolution
        {
            private:
                gint32 x_;

                gint32 y_;

            public:
                Resolution(gint32 x = 0, gint32 y = 0) :
                    x_(x),
                    y_(y)
                {
                }

                Resolution(const Resolution & rhs) :
                    x_(rhs.x_),
                    y_(rhs.y_)
                {
                }

                Resolution &
                operator=(const Resolution & rhs)
                {
                    if (this != &rhs)
                    {
                        x_ = rhs.x_;
                        y_ = rhs.y_;
                    }
                    return *this;
                }

                gint32
                get_x() const
                {
                    return x_;
                }

                void
                set_x(gint32 x)
                {
                    x_ = x;
                }

                gint32
                get_y() const
                {
                    return y_;
                }

                void
                set_y(gint32 y)
                {
                    y_ = y;
                }

                bool
                operator==(const Resolution & rhs) const throw()
                {
                    return x_ == rhs.x_ && y_ == rhs.y_;
                }

                bool
                operator>(const Resolution & rhs) const throw()
                {
                    return x_ > rhs.x_ || ( x_ == rhs.x_ && y_ > rhs.y_);
                }

                bool
                operator<(const Resolution &rhs) const throw()
                {
                    return x_ < rhs.x_ || (x_ == rhs.x_ && y_ < rhs.y_);
                }
        };

    public:
        Thumbnail();

#if 0
        Thumbnail(
                  const Glib::ustring & path,
                  const Resolution & resolution);
#endif
        Thumbnail( const Thumbnail &rhs ); 
        ~Thumbnail() throw();

        Thumbnail &
        operator =( const Thumbnail &rhs );

        inline const Glib::ustring &
        get_path() const;

        void
        set_path(const Glib::ustring & path);
            
        inline const Resolution &
        get_resolution() const throw();

        void
        set_resolution(const Resolution & resolution);

        void
        generate(const Resolution & new_resolution,
                 const DatabasePtr & db,
                 const Photo &photo) throw(Error);

        void
        generate( const Photo &photo) throw(Error);

        void
        generate(Exiv2::ExifData & exifData,
                 const Photo &photo) throw(Error);

        void
        insert(std::vector<Gnome::Gda::Value> &values);

        void
        update(DataModelPtr & model, gint32 row) throw(Glib::Error);

        void
        create(const DataModelPtr & data_model, gint32 row);

    private:
        void
        make_thumb_path() throw(Error);

        void
        generate_using_gdkpixbuf(const Glib::ustring & path,
                           const Resolution & new_resolution_hint);

        static const gint32 PHOTOID_COL;
        static const gint32 PATH_COL;
        static const gint32 RES_X_COL;
        static const gint32 RES_Y_COL;

        Glib::ustring path_;
        Resolution resolution_;

};

inline const Glib::ustring &
Thumbnail::get_path() const
{
    return path_;
}

inline const Thumbnail::Resolution &
Thumbnail::get_resolution() const throw()
{
    return resolution_;
}

} // namespace Solang

#endif // SOLANG_THUMBNAIL_H
