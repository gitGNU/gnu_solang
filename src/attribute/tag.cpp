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

#include <iostream>
#include <sstream>

#include "id-base.h"
#include "tag.h"

namespace Solang
{

const gint32 Tag::ALL_PHOTOS_TAGID = 0;

const gint32 Tag::TAGID_COL = 0;
const gint32 Tag::NAME_COL = 1;
const gint32 Tag::DESCRIPTION_COL = 2;
const gint32 Tag::ICONPATH_COL = 3;

Tag::Tag() throw() :
    DBObject(),
    PhotoSearchCriteria()
{
}

Tag::Tag(gint tagId, const Glib::ustring & name) throw() :
    tagId_(tagId),
    name_(name)
{
}

Tag::~Tag() throw()
{
}

void
Tag::set_tag_id(gint tag_id) throw()
{
    tagId_ = tag_id;
}

void
Tag::set_name(const Glib::ustring & name) throw()
{
    name_ = name;
}

void
Tag::set_description(const Glib::ustring & description) throw()
{
    description_ = description;
}

void
Tag::insert(DataModelPtr & model, gint32 lastIndex) throw(Error)
{
    std::vector<Gnome::Gda::Value> values;
    values.push_back( Gnome::Gda::Value( lastIndex + 1 ) ); //tagid
    values.push_back( Gnome::Gda::Value( get_name() ) );
    values.push_back( Gnome::Gda::Value( get_description() ) ); 
    values.push_back( Gnome::Gda::Value( get_icon_path() ) ); 

    gint32 row = model->append_values( values );

    if( -1 == row )
    {
        //TBD::Error
    }
    set_tag_id( lastIndex + 1 );
    set_row( row );

    return ;

}

void
Tag::update(DataModelPtr & model, gint32 row) throw(Error)
{
    try
    {
        std::vector<Gnome::Gda::Value> values;
        values.push_back( Gnome::Gda::Value( get_tag_id() ) );
        values.push_back( Gnome::Gda::Value( get_name() ) );
        values.push_back( Gnome::Gda::Value( get_description() ) );
        values.push_back( Gnome::Gda::Value( get_icon_path() ) );
        model->set_values( row, values );
    }
    catch (Glib::Error & e)
    {
        std::cerr << "Error::Could not save: "
                  << e.what() << std::endl;
    }

    return;    
}

void
Tag::create(const DataModelPtr & dataModel, gint32 row) throw(Error)
{
    set_row( row );
    set_tag_id( dataModel->get_value_at( 
                        TAGID_COL, row ).get_int() );
    set_name( dataModel->get_value_at( 
                        NAME_COL, row ).get_string() );
    set_description( dataModel->get_value_at( 
                        DESCRIPTION_COL, row ).get_string() );
    set_icon_path( dataModel->get_value_at( 
                        ICONPATH_COL, row ).get_string() );
    return;
}

#if 0
Glib::ustring Tag::getCreateSQL() throw(Error)
{
    std::ostringstream sout;
    Glib::ustring sql("insert into tags(tagid, tag, description, iconPath) values(NULL,'");
    sout<<get_name()<<"','"<<;
    sout<<get_description()<<"','";
    sout<<get_icon_path()<<"')";
    sql += sout.str();
    
}
#endif

Glib::ustring
Tag::get_db_object_type_name() const throw()
{
    return "tags";
}

DeleteActionPtr
Tag::get_delete_action() throw()
{
    DeleteActionPtr action(
            new DeleteAction(get_criteria_description(), this ));
    if( get_is_deleted() )
        return action;

    {
        std::ostringstream sout;
        sout<<"delete from tags where tagid="<<get_tag_id();
        action->add_command( sout.str() );
    }
    {
        std::ostringstream sout;
        sout<<"delete from photo_tags where tagid="<<get_tag_id();
        action->add_command( sout.str() );
    }
    set_is_deleted( true );
    return action;
}

Glib::ustring
Tag::get_query_criteria() const throw()
{
    std::ostringstream sout;

    sout << " photo_tags.tagid ="<<get_tag_id() << " ";

    return sout.str();
}

void
Tag::set_icon_path(Glib::ustring const & value)
{
    iconPath_ = value;
}

PhotoSearchCriteria::ClubbingOperationType
Tag::get_clubbing_type() const throw()
{
    return PhotoSearchCriteria::CLUB_OR;
}

gint32
Tag::get_id() const throw()
{
    return IDBase<Tag>::get_id();
}

Glib::ustring
Tag::get_criteria_description() const throw()
{
    return "Tag: " + get_name();
}

Glib::ustring
Tag::get_criteria_icon_path() const throw()
{
    return PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/tag-16.png";
}

} // namespace Solang
