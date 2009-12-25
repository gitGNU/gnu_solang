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

#include "database.h"
#include "id-base.h"
#include "tag.h"

namespace Solang
{

Tag::Tag(const Glib::ustring & name, const std::string & urn)
         throw() :
    DBObject(),
    PhotoSearchCriteria(),
    name_(name),
    urn_(urn)
{
}

Tag::~Tag() throw()
{
}

void
Tag::delete_async(Database & database, const SlotAsyncReady & slot)
                  const throw()
{
    database.delete_async(*this, slot);
}

Glib::ustring
Tag::get_delete_query() const throw()
{
    return Glib::ustring::compose("DELETE {"
                                  "  <%1> a nao:Tag ."
                                  "}",
                                  urn_);
}

Glib::ustring
Tag::get_query_criteria() const throw()
{
    return Glib::ustring::compose("nao:hasTag <%1> .", urn_);
}

Glib::ustring
Tag::get_save_query() const throw()
{
    return Glib::ustring::compose("INSERT {"
                                  "  _:tag a nao:Tag ;"
                                  "  nao:prefLabel '%1' ."
                                  "} "
                                  "WHERE {"
                                  "  OPTIONAL {"
                                  "     ?tag a nao:Tag ;"
                                  "     nao:prefLabel '%2'"
                                  "  } ."
                                  "  FILTER (!bound(?tag)) "
                                  "}",
                                  name_, name_);
}

void
Tag::save_async(Database & database, const SlotAsyncReady & slot)
                const throw()
{
    database.save_async(*this, slot);
}

void
Tag::edit_async(const Glib::ustring & name,
                Database & database,
                const SlotAsyncReady & slot) throw()
{
    database.edit_async(*this, name, slot);
}

Glib::ustring
Tag::get_edit_query(const Glib::ustring & name) const throw()
{
    return Glib::ustring::compose("DELETE {"
                                  "  <%1> nao:prefLabel '%2' ."
                                  "} "
                                  "INSERT {"
                                  "  <%3> nao:prefLabel '%4' ."
                                  "}",
                                  urn_, name_, urn_, name);
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

DeleteActionPtr
Tag::get_delete_action() throw()
{
    DeleteActionPtr action(
            new DeleteAction(get_criteria_description(), this ));
    return action;
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

const std::string &
Tag::get_urn() const throw()
{
    return urn_;
}

} // namespace Solang
