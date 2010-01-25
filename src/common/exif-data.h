/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009, 2010 Debarshi Ray <rishi@gnu.org>
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

#include "types.h"

namespace Solang
{

class ExifData
{
    private:
        static const Glib::ustring flashOff_;

        static const Glib::ustring flashOn_;

        static const Glib::ustring meteringModeAverage_;

        static const Glib::ustring meteringModeCenterWeightedAverage_;

        static const Glib::ustring meteringModeSpot_;

        static const Glib::ustring meteringModeMultiSpot_;

        static const Glib::ustring meteringModePattern_;

        static const Glib::ustring meteringModePartial_;

        static const Glib::ustring whiteBalanceAuto_;

        static const Glib::ustring whiteBalanceManual_;

        Glib::ustring        camera_;
        Glib::ustring        exposureTime_;
        Glib::ustring        flash_;
        Glib::ustring        flashEnum_;
        Glib::ustring        fnumber_;
        Glib::ustring        isoSpeed_;
        Glib::ustring        meteringMode_;
        Glib::ustring        meteringModeEnum_;
        Glib::ustring        focalLength_;
        Glib::ustring        whiteBalance_;
        Glib::ustring        whiteBalanceEnum_;
        Glib::ustring        pictureTakenTime_;


    public:
        ExifData();
        ExifData( const ExifData &rhs );
        ExifData &operator = ( const ExifData &rhs );

        ~ExifData() throw();

        const Glib::ustring &
        get_camera() const throw();

        const Glib::ustring &
        get_flash() const throw();

        const Glib::ustring &
        get_flash_enum() const throw();

        const Glib::ustring &
        get_fnumber() const throw();

        const Glib::ustring &
        get_exposure_time() const throw();

        inline const Glib::ustring &
        get_iso_speed() const throw()
        {
            return isoSpeed_;
        }

        inline const Glib::ustring &
        get_metering_mode() const throw()
        {
            return meteringMode_;
        }

        const Glib::ustring &
        get_metering_mode_enum() const throw();

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

        const Glib::ustring &
        get_white_balance_enum() const throw();

        inline const Glib::ustring &
        get_picture_taken_time() const throw()
        {
            return pictureTakenTime_;
        }    

        void
        set_camera(const Glib::ustring & camera) throw();

        void
        set_exposure_time(const Glib::ustring & exposure_time)
                          throw();

        void
        set_flash(const Glib::ustring & flash) throw();

        void
        set_fnumber(const Glib::ustring & fnumber) throw();

        void
        set_focal_length(const Glib::ustring & focal_length) throw();

        void
        set_iso_speed(const Glib::ustring & iso_speed) throw();

        void
        set_metering_mode(const Glib::ustring & metering_mode)
                          throw();

        void
        set_white_balance(const Glib::ustring & white_balance)
                          throw();
};

} // namespace Solang

#endif // SOLANG_EXIFDATA_H
