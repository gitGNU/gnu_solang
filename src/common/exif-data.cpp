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

#include <sstream>

#include <glibmm/i18n.h>

#include "exif-data.h"

namespace Solang
{

const Glib::ustring ExifData::meteringModeAverage_
    = "http://www.tracker-project.org/temp/nmm#metering-mode-average";

const Glib::ustring ExifData::meteringModeCenterWeightedAverage_
    = "http://www.tracker-project.org/temp/"
          "nmm#metering-mode-center-weighted-average";

const Glib::ustring ExifData::meteringModeSpot_
    = "http://www.tracker-project.org/temp/nmm#metering-mode-spot";

const Glib::ustring ExifData::meteringModeMultiSpot_
    = "http://www.tracker-project.org/temp/"
          "nmm#metering-mode-multispot";

const Glib::ustring ExifData::meteringModePattern_
    = "http://www.tracker-project.org/temp/nmm#metering-mode-pattern";

const Glib::ustring ExifData::meteringModePartial_
    = "http://www.tracker-project.org/temp/nmm#metering-mode-partial";

const Glib::ustring ExifData::whiteBalanceAuto_
    = "http://www.tracker-project.org/temp/nmm#white-balance-auto";

const Glib::ustring ExifData::whiteBalanceManual_
    = "http://www.tracker-project.org/temp/nmm#white-balance-manual";

ExifData::ExifData() :
    camera_(),
    exposureTime_(),
    fnumber_(),
    isoSpeed_(),
    meteringMode_(),
    meteringModeEnum_(),
    focalLength_(),
    whiteBalance_(),
    whiteBalanceEnum_(),
    pictureTakenTime_()
{
}

ExifData::ExifData(const ExifData &rhs) :
    camera_(rhs.camera_),
    exposureTime_(rhs.exposureTime_),
    fnumber_(rhs.fnumber_),
    isoSpeed_(rhs.isoSpeed_),
    meteringMode_(rhs.meteringMode_),
    meteringModeEnum_(rhs.meteringModeEnum_),
    focalLength_(rhs.focalLength_),
    whiteBalance_(rhs.whiteBalance_),
    whiteBalanceEnum_(rhs.whiteBalanceEnum_),
    pictureTakenTime_( rhs.pictureTakenTime_ )
{
}

ExifData::~ExifData() throw()
{
}

const Glib::ustring &
ExifData::get_camera() const throw()
{
    return camera_;
}    

const Glib::ustring &
ExifData::get_exposure_time() const throw()
{
    return exposureTime_;
}

const Glib::ustring &
ExifData::get_fnumber() const throw()
{
    return fnumber_;
}    

const Glib::ustring &
ExifData::get_metering_mode_enum() const throw()
{
    return meteringModeEnum_;
}

const Glib::ustring &
ExifData::get_white_balance_enum() const throw()
{
    return whiteBalanceEnum_;
}

void
ExifData::set_camera(const Glib::ustring & camera) throw()
{
    camera_ = camera;
}

void
ExifData::set_exposure_time(const Glib::ustring & exposure_time)
                            throw()
{
    exposureTime_ = exposure_time;
}

void
ExifData::set_fnumber(const Glib::ustring & fnumber) throw()
{
    fnumber_ = fnumber;
}

void
ExifData::set_focal_length(const Glib::ustring & focal_length) throw()
{
    focalLength_ = focal_length;
}

void
ExifData::set_iso_speed(const Glib::ustring & iso_speed) throw()
{
    isoSpeed_ = iso_speed;
}

void
ExifData::set_metering_mode(const Glib::ustring & metering_mode)
                            throw()
{
    if (meteringModeAverage_ == metering_mode)
    {
        meteringMode_ = _("Average");
    }
    else if (meteringModeCenterWeightedAverage_ == metering_mode)
    {
        meteringMode_ = _("Center Weighted Average");
    }
    else if (meteringModeSpot_ == metering_mode)
    {
        meteringMode_ = _("Spot");
    }
    else if (meteringModeMultiSpot_ == metering_mode)
    {
        meteringMode_ = _("Multi-spot");
    }
    else if (meteringModePattern_ == metering_mode)
    {
        meteringMode_ = _("Pattern");
    }
    else if (meteringModePartial_ == metering_mode)
    {
        meteringMode_ = _("Partial");
    }
    else
    {
        meteringMode_ = "";
    }

    meteringModeEnum_ = metering_mode;
}

void
ExifData::set_white_balance(const Glib::ustring & white_balance)
                            throw()
{
    if (whiteBalanceAuto_ == white_balance)
    {
        whiteBalance_ = _("Automatic");
    }
    else if (whiteBalanceManual_ == white_balance)
    {
        whiteBalance_ = _("Manual");
    }
    else
    {
        whiteBalance_ = "";
    }

    whiteBalanceEnum_ = white_balance;
}

ExifData &ExifData::operator =(const ExifData &rhs)
{
    if( this != &rhs )
    {
        camera_ = rhs.camera_;
        exposureTime_ = rhs.exposureTime_;
        fnumber_ = rhs.fnumber_;
        isoSpeed_ = rhs.isoSpeed_;
        meteringMode_ = rhs.meteringMode_;
        meteringModeEnum_ = rhs.meteringModeEnum_;
        focalLength_ = rhs.focalLength_;
        whiteBalance_ = rhs.whiteBalance_;
        whiteBalanceEnum_ = rhs.whiteBalanceEnum_;
        pictureTakenTime_ = rhs.pictureTakenTime_;
    }
    return *this;
}

} // namespace Solang
