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

#include "database.h"
#include "photo.h"
#include "photo-tag.h"
#include "tag.h"

namespace Solang
{

PhotoTag::PhotoTag(const PhotoPtr & photo, const TagPtr & tag)
                   throw() :
    DBObject(),
    photo_(photo),
    tag_(tag)
{
}

PhotoTag::~PhotoTag() throw()
{
}

void
PhotoTag::delete_async(Database & database,
                       const SlotAsyncReady & slot) const throw()
{
    database.delete_async(*this, slot);
}

Glib::ustring
PhotoTag::get_delete_query() const throw()
{
    return Glib::ustring::compose("DELETE {"
                                  "  ?photo nao:hasTag <%1> ."
                                  "}"
                                  "WHERE {"
                                  "  ?photo nie:url '%2' ."
                                  "}",
                                  tag_->get_urn(), photo_->get_uri());
}

Glib::ustring
PhotoTag::get_save_query() const throw()
{
    return Glib::ustring::compose("INSERT {"
                                  "  ?photo nao:hasTag <%1> ."
                                  "} "
                                  "WHERE {"
                                  "  ?photo nie:url '%2' ."
                                  "}",
                                  tag_->get_urn(), photo_->get_uri());
}

void
PhotoTag::save_async(Database & database, const SlotAsyncReady & slot)
                     const throw()
{
    database.save_async(*this, slot);
}

DeleteActionPtr
PhotoTag::get_delete_action() throw()
{
    DeleteActionPtr action( new DeleteAction( "PhotoTag" , NULL ) );
    return action;
}

} // namespace Solang
