/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * exifs-table.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * exifs-table.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * exifs-table.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "db-table-visitor.h"
#include "photo.h"
#include "exifs-table.h"
#include "progress-observer.h"

namespace Solang
{

ExifsTable::ExifsTable( const Glib::ustring &name )
    :DBTable( name )
{
}

ExifsTable::~ExifsTable()
{
}

Glib::ustring ExifsTable::getSelectionQuery() const
{
    return "select * from exifs";
}

Glib::ustring ExifsTable::getInsertQuery() const
{
    return "insert into exifs\
                values(    \
                    ## /*name: +0, type:gint*/, \
                    ## /*name: +1, type:gchararray*/, \
                    ## /*name: +2, type:gchararray*/, \
                    ## /*name: +3, type:gchararray*/, \
                    ## /*name: +4, type:gint*/, \
                    ## /*name: +5, type:gchararray*/, \
                    ## /*name: +6, type:gchararray*/, \
                    ## /*name: +7, type:gchararray*/, \
                    ## /*name: +8, type:gchararray*/)";
                
}

Glib::ustring ExifsTable::getUpdateQuery() const
{
    return "update exifs set \
                photoid=## /* name: +0, type:gint*/, \
                aperture=## /* name: +1, type:gchararray*/, \
                shutter_speed=## /* name: +2, type:gchararray*/, \
                exposure_program=## /* name: +3, type:gchararray*/, \
                iso_speed=## /* name: +4, type:gint*/, \
                metering_mode=## /* name: +5, type:gchararray*/, \
                focal_length=## /* name: +6, type:gchararray*/, \
                white_balance=## /* name: +7, type:gchararray*/, \
                focal_length_in_film=## /* name: +8, type:gshort*/ \
                where photoid=## /* name: -0, type:gint*/";
}

Glib::ustring ExifsTable::getDeleteQuery() const
{
    return "delete from exifs  \
                where photoid=##/* name: -0, type: gint*/";
}

void ExifsTable::receive(Solang::DBTableVisitor& visitor, 
                              Solang::ProgressObserverPtr & observer)
{

}

} //namespace Solang
