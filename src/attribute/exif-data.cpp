/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * tag.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * tag.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * tag.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sstream>

#include "config.h"
#include "exif-data.h"

namespace Solang
{

const gint32 ExifData::APERTURE_COL               = 8;
const gint32 ExifData::SHUTTER_SPEED_COL          = 9;
const gint32 ExifData::EXPOSURE_PROGRAM_COL       = 10;
const gint32 ExifData::ISO_COL                    = 11;
const gint32 ExifData::EXPOSURE_METERING_MODE_COL = 12;
const gint32 ExifData::FOCAL_LENGTH_COL           = 13;
const gint32 ExifData::WHITE_BALANCE_COL          = 14;
const gint32 ExifData::FILM_FOCAL_LENGTH_COL      = 15;
const gint32 ExifData::PICTURE_TAKEN_TIME         = 16;

ExifData::ExifData()
    : aperture_(),
    shutterSpeed_(),
    exposureProgram_(),
    isoSpeed_(-1),
    exposureMeteringMode_(),
    focalLength_(),
    whiteBalance_(),
    focalLengthInFilm_(),
    pictureTakenTime_()
{
}

ExifData::ExifData( const Exiv2::ExifData &data )
    : aperture_(),
    shutterSpeed_(),
    exposureProgram_(),
    isoSpeed_(-1),
    exposureMeteringMode_(),
    focalLength_(),
    whiteBalance_(),
    focalLengthInFilm_(),
    pictureTakenTime_()
{
    //Read the values one by one
    if( data.empty() )
    {
        return;
    }


    {
        Exiv2::ExifData::const_iterator it 
                = data.findKey(Exiv2::ExifKey("Exif.Photo.FNumber"));
        if( it != data.end() )
        {
            std::ostringstream sout;
            print0x829d (sout, (*it).value() );
            aperture_ = sout.str();
        }
        
    }

    {
        Exiv2::ExifData::const_iterator it 
                = data.findKey(Exiv2::ExifKey("Exif.Photo.ExposureTime"));
        if( it != data.end() )
        {

            std::ostringstream sout;
            print0x829a( sout, (*it).value() );
            
            shutterSpeed_ = sout.str();
        }
        
    }

    {
        Exiv2::ExifData::const_iterator it 
                = data.findKey(Exiv2::ExifKey("Exif.Photo.ExposureProgram"));
        if( it != data.end() )
        {
            std::ostringstream sout;
            print0x8822 (sout, (*it).value() );
            exposureProgram_ = sout.str();
        }
        
    }

    {
        Exiv2::ExifData::const_iterator it 
                = data.findKey(Exiv2::ExifKey("Exif.Photo.ISOSpeedRatings"));
        if( it != data.end() )
        {
            isoSpeed_ = (*it).value().toLong();
        }
        
    }

    {
        Exiv2::ExifData::const_iterator it 
                = data.findKey(Exiv2::ExifKey("Exif.Photo.MeteringMode"));
        if( it != data.end() )
        {
            std::ostringstream sout;
            print0x9207(sout, (*it).value() );
            exposureMeteringMode_ = sout.str();
        }
        
    }

    {
        Exiv2::ExifData::const_iterator it 
                = data.findKey(Exiv2::ExifKey("Exif.Photo.FocalLength"));
        if( it != data.end() )
        {
            std::ostringstream sout;
            print0x920a (sout, (*it).value() );
            focalLength_ = sout.str();
        }
        
    }

    {
        Exiv2::ExifData::const_iterator it 
                = data.findKey(Exiv2::ExifKey("Exif.Photo.WhiteBalance"));
        if( it != data.end() )
        {
            std::ostringstream sout;
            print0xa403(sout, (*it).value() );
            whiteBalance_ = sout.str();
        }
        
    }

    {
        Exiv2::ExifData::const_iterator it 
                = data.findKey(
                Exiv2::ExifKey("Exif.Photo.FocalLengthIn35mmFilm"));
        if( it != data.end() )
        {
            std::ostringstream sout;
            print0xa405(sout, (*it).value() );
            focalLengthInFilm_ = sout.str();
        }
        
    }
    
    {
        Exiv2::ExifData::const_iterator it 
                = data.findKey(
                Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
        if( it != data.end() )
        {
            pictureTakenTime_ = (*it).toString();
        }
        
    }
}

ExifData::ExifData(const ExifData &rhs)
    : aperture_(rhs.aperture_),
    shutterSpeed_(rhs.shutterSpeed_),
    exposureProgram_(rhs.exposureProgram_),
    isoSpeed_(rhs.isoSpeed_),
    exposureMeteringMode_(rhs.exposureMeteringMode_),
    focalLength_(rhs.focalLength_),
    whiteBalance_(rhs.whiteBalance_),
    focalLengthInFilm_(rhs.focalLengthInFilm_),
    pictureTakenTime_( rhs.pictureTakenTime_ )
{
}

ExifData::~ExifData() throw()
{
}

ExifData &ExifData::operator =(const ExifData &rhs)
{
    if( this != &rhs )
    {
        aperture_ = rhs.aperture_;
        shutterSpeed_ = rhs.shutterSpeed_;
        exposureProgram_ = rhs.exposureProgram_;
        isoSpeed_ = rhs.isoSpeed_;
        exposureMeteringMode_ = rhs.exposureMeteringMode_;
        focalLength_ = rhs.focalLength_;
        whiteBalance_ = rhs.whiteBalance_;
        focalLengthInFilm_ = rhs.focalLengthInFilm_;
        pictureTakenTime_ = rhs.pictureTakenTime_;
    }
    return *this;
}


void ExifData::insert( std::list<Gnome::Gda::Value> &values )
{
    values.push_back( Gnome::Gda::Value( get_aperture() ) );
    values.push_back( Gnome::Gda::Value( get_shutter_speed() ) ); 
    values.push_back( Gnome::Gda::Value( get_exposure_program() ) ); 
    values.push_back( Gnome::Gda::Value( (gint)get_iso_speed() ) ); 
    values.push_back( 
                Gnome::Gda::Value( get_exposure_metering_mode() ) ); 
    values.push_back( Gnome::Gda::Value( get_focal_length() ) ); 
    values.push_back( Gnome::Gda::Value( get_white_balance() ) ); 
    values.push_back( Gnome::Gda::Value( get_focal_length_film() ) ); 
    values.push_back( Gnome::Gda::Value( 
                        Glib::ustring( get_picture_taken_time() ) ) );
    //

    return ;

}

void ExifData::update( DataModelPtr &model, gint32 row)
{
    //TBD::check and update
    model->set_value_at( APERTURE_COL, row,
                Gnome::Gda::Value( get_aperture() ) );
    model->set_value_at( SHUTTER_SPEED_COL, row,
                Gnome::Gda::Value( get_shutter_speed() ) );
    model->set_value_at( EXPOSURE_PROGRAM_COL, row,
                Gnome::Gda::Value( get_exposure_program() ) );
    model->set_value_at( ISO_COL, row,
                Gnome::Gda::Value( get_iso_speed() ) );
    model->set_value_at( EXPOSURE_METERING_MODE_COL, row,
                Gnome::Gda::Value( get_exposure_metering_mode() ) );
    model->set_value_at( FOCAL_LENGTH_COL, row,
                Gnome::Gda::Value( get_exposure_metering_mode() ) );
    model->set_value_at( WHITE_BALANCE_COL, row,
                Gnome::Gda::Value( get_white_balance() ) );
    model->set_value_at( FILM_FOCAL_LENGTH_COL, row,
                Gnome::Gda::Value( get_focal_length_film() ) );
    model->set_value_at( PICTURE_TAKEN_TIME, row,
                Gnome::Gda::Value( get_picture_taken_time() ) );
    return;    

}

void ExifData::create( DataModelPtr& dataModel, gint32 row)
{
    aperture_ =  dataModel->get_value_at( 
                        APERTURE_COL, row ).get_string() ;
    shutterSpeed_ =  dataModel->get_value_at( 
                        SHUTTER_SPEED_COL, row ).get_string() ;
    exposureProgram_ =  dataModel->get_value_at( 
                        EXPOSURE_PROGRAM_COL, row ).get_string() ;
    isoSpeed_ =  dataModel->get_value_at( 
                        ISO_COL, row ).get_int() ;
    exposureMeteringMode_ =  dataModel->get_value_at( 
                    EXPOSURE_METERING_MODE_COL, row ).get_string() ;
    focalLength_ =  dataModel->get_value_at( 
                        FOCAL_LENGTH_COL, row ).get_string() ;
    whiteBalance_ =  dataModel->get_value_at( 
                        WHITE_BALANCE_COL, row ).get_string() ;
    focalLengthInFilm_ =  dataModel->get_value_at( 
                        FILM_FOCAL_LENGTH_COL, row ).get_string() ;
    pictureTakenTime_ = dataModel->get_value_at(
                        PICTURE_TAKEN_TIME, row ).get_string();
    return;
}

} //namespace Solang
