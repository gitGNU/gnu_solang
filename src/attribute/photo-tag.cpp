/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * photo-tag.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * photo-tag.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * photo-tag.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <sstream>

#include "photo-tag.h"

namespace Solang
{

const gint32 PhotoTag::PHOTOID_COL = 0;
const gint32 PhotoTag::TAGID_COL = 1;

PhotoTag::PhotoTag()
    :DBObject()
{
}

PhotoTag::PhotoTag( gint32 photoId, gint32 tagId )
    :photoId_(photoId),
    tagId_(tagId)
{
}

PhotoTag::~PhotoTag() throw()
{
}

void PhotoTag::set_tagId_( gint32 tagId )
{
    tagId_ = tagId;
}

void PhotoTag::set_photoId_(  gint32 photoId )
{
    photoId_ = photoId;
}

void PhotoTag::insert(
        DataModelPtr &model, gint32 lastIndex) throw(Error)
{
    std::vector<Gnome::Gda::Value> values;
    values.push_back( Gnome::Gda::Value( get_photoId_() ) );
    values.push_back( Gnome::Gda::Value( get_tagId_() ) ); 

	gint32 row = 0;

    try
    {
        row = model->append_values( values );
        create( model, row );
    }
    catch( Glib::Error &e)
    {
        std::cerr<<"Error:"<<e.what()<<std::endl;
    }

    if( -1 == row )
    {
        //TBD::Error
    }


    return ;

}

void PhotoTag::update(
            DataModelPtr &model, gint32 row) throw(Error)
{

    return;    

}

void PhotoTag::create( DataModelPtr& dataModel, gint32 row) throw(Error)
{
    set_row_( row );
    set_tagId_( dataModel->get_value_at( 
                        PHOTOID_COL, row ).get_int() );
    set_tagId_( dataModel->get_value_at( 
                        TAGID_COL, row ).get_int() );
    return;
}

#if 0
Glib::ustring PhotoTag::getCreateSQL() throw(Error)
{
    std::ostringstream sout;
    Glib::ustring sql("insert into tags(tagid, tag, description, iconPath) values(NULL,'");
    sout<<get_name_()<<"','"<<;
    sout<<get_description_()<<"','";
    sout<<get_iconPath_()<<"')";
    sql += sout.str();
    
}
#endif

Glib::ustring
PhotoTag::get_db_object_type_name() const throw()
{
    return "photo_tags";
}
Glib::ustring PhotoTag::getQueryCriteria() const
{
    return " ";
    
}

} // namespace Solang
