/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * db-table-factory.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * d-b-table-factory.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * d-b-table-factory.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "db-table-factory.h"
#include "exifs-table.h"
#include "photos-table.h"
#include "photo-tags-table.h"
#include "tags-table.h"
#include "thumbnails-table.h"

namespace Solang
{

DBTablePtr
DBTableFactory::create(const Glib::ustring & table_type_name)
{
    if (table_type_name == "exifs")
        return DBTablePtr(new ExifsTable(table_type_name));

    if (table_type_name == "photos")
        return DBTablePtr(new PhotosTable(table_type_name));

    if (table_type_name == "photo_tags")
        return DBTablePtr(new PhotoTagsTable(table_type_name));

    if (table_type_name == "tags")
        return DBTablePtr(new TagsTable(table_type_name));

    if (table_type_name == "thumbnails")
        return DBTablePtr(new ThumbnailsTable(table_type_name));
}

} //namespace Solang
