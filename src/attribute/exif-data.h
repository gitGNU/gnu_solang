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

#ifndef SOLANG_EXIFDATA_H
#define SOLANG_EXIFDATA_H

#include <exception>
#include <vector>

#include <glibmm.h>
#include <exiv2/exif.hpp>

#include "types.h"

namespace Solang
{

class ExifData
{
    private:

        static const gint32 PHOTOID_COL;
        static const gint32 APERTURE_COL;
        static const gint32 SHUTTER_SPEED_COL;
        static const gint32 EXPOSURE_PROGRAM_COL;
        static const gint32 ISO_COL;
        static const gint32 EXPOSURE_METERING_MODE_COL;
        static const gint32 FOCAL_LENGTH_COL;
        static const gint32 WHITE_BALANCE_COL;
        static const gint32 FILM_FOCAL_LENGTH_COL;
        static const gint32 PICTURE_TAKEN_TIME;


        Glib::ustring        aperture_;
        Glib::ustring        shutterSpeed_;
        Glib::ustring        exposureProgram_;
        gint32               isoSpeed_;
        Glib::ustring        exposureMeteringMode_;
        Glib::ustring        focalLength_;
        Glib::ustring        whiteBalance_;
        Glib::ustring        focalLengthInFilm_;
        Glib::ustring        pictureTakenTime_;


    public:
        ExifData();
        ExifData( const Exiv2::ExifData &data );
        ExifData( const ExifData &rhs );
        ExifData &operator = ( const ExifData &rhs );

        ~ExifData() throw();

        inline const Glib::ustring &
        get_aperture() const throw()
        {
            return aperture_;
        }    

        inline const Glib::ustring &
        get_shutter_speed() const throw()
        {
            return shutterSpeed_;
        }

        inline const Glib::ustring &
        get_exposure_program() const throw()
        {
            return exposureProgram_;
        }

        inline gint16
        get_iso_speed() const throw()
        {
            return isoSpeed_;
        }

        inline const Glib::ustring &
        get_exposure_metering_mode() const throw()
        {
            return exposureMeteringMode_;
        }

        inline const Glib::ustring &
        get_focal_length() const throw()
        {
            return focalLength_;
        }

        inline const Glib::ustring &
        get_white_balance() const throw()
        {
            return whiteBalance_;
        }

        inline const Glib::ustring &
        get_focal_length_film() const throw()
        {
            return focalLengthInFilm_;
        }        

        inline const Glib::ustring &
        get_picture_taken_time() const throw()
        {
            return pictureTakenTime_;
        }    

        void insert( std::vector<Gnome::Gda::Value> &values );
        void update( DataModelPtr &model, gint32 row );
        void create( DataModelPtr &model, gint32 row );

};

} // namespace Solang

#endif // SOLANG_EXIFDATA_H
