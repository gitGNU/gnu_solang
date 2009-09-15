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

#include <sstream>

#include "id-base.h"
#include "modification-date.h"


namespace Solang
{

const gint32 ModificationDate::MOD_DAY_COL   = 3;
const gint32 ModificationDate::MOD_MONTH_COL = 4;
const gint32 ModificationDate::MOD_YEAR_COL  = 5;

Glib::ustring
ModificationDate::get_query_criteria() const throw()
{
    std::ostringstream sout;
    if( get_year() > 0 )
    {
        sout<<" ( mod_year="<<get_year();
        if( get_month() > 0 )
        {
            sout<<" and mod_month="<<get_month();
            if( get_day() > 0 )
            {
                sout<<" and mod_day="<<get_day();
            }
        }
        sout<<" )";
    }
    return sout.str();
}

PhotoSearchCriteria::ClubbingOperationType
ModificationDate::get_clubbing_type() const throw()
{
    return PhotoSearchCriteria::CLUB_OR;
}

gint32
ModificationDate::get_id() const throw()
{
    return IDBase<ModificationDate>::get_id();
}

Glib::ustring
ModificationDate::get_criteria_description() const throw()
{
    std::ostringstream sout;
    if( get_day() == 0 )
    {
        if( get_month() != 0 )
        {
            sout<<"Month: "<<get_month()<<" ";
        }
        if( get_year() != 0 )
        {
            sout<<"Year: "<<get_year();
        }
    }
    else
    {
        sout<<"Date: "<<get_day()
            <<"/"<<get_month()
            <<"/"<<get_year();
    }

    return sout.str();
}

Glib::ustring
ModificationDate::get_criteria_icon_path() const throw()
{
    //TBD::CORRECT
    return PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/tag-16.png";
}

void
ModificationDate::insert(std::vector<Gnome::Gda::Value> &values)
{
    values.push_back( Gnome::Gda::Value( get_day() ) ); 
    values.push_back( Gnome::Gda::Value( get_month() ) ); 
    values.push_back( Gnome::Gda::Value( get_year() ) ); 
}

void
ModificationDate::update(
                DataModelPtr & model, gint32 row) throw(Glib::Error)
{
    if( get_day() != model->get_value_at(
                                MOD_DAY_COL, row ).get_int())
    {
        model->set_value_at(
                MOD_DAY_COL, row, 
                            Gnome::Gda::Value( get_day() ) );
    }
    
    if( get_month() != model->get_value_at(
                                MOD_MONTH_COL, row ).get_int())
    {
        model->set_value_at(
                MOD_MONTH_COL, row, 
                            Gnome::Gda::Value( get_month() ) );
    }

    if( get_year() != model->get_value_at(
                                MOD_YEAR_COL, row ).get_int())
    {
        model->set_value_at(
                MOD_YEAR_COL, row, 
                            Gnome::Gda::Value( get_year() ) );
    }

}

void
ModificationDate::create(DataModelPtr & data_model, gint32 row)
{
    set_day(
        data_model->get_value_at( MOD_DAY_COL, row ).get_int());    
    set_month(
        data_model->get_value_at( MOD_MONTH_COL, row ).get_int());    
    set_year(
        data_model->get_value_at( MOD_YEAR_COL, row ).get_int());    
}

} // namespace Solang
