#include <sstream>

#include "id-base.h"
#include "modification-date.h"


namespace Solang
{

const gint32 ModificationDate::MOD_DAY_COL   = 2;
const gint32 ModificationDate::MOD_MONTH_COL = 3;
const gint32 ModificationDate::MOD_YEAR_COL  = 4;

Glib::ustring
ModificationDate::get_query_criteria() const throw()
{
    std::ostringstream sout;
    sout<<" ( mod_day="<<get_day()
        <<" and mod_month="<<get_month()
        <<" and mod_year="<<get_year()
        <<" )";
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
    sout<<get_day()
        <<"/"<<get_month()
        <<"/"<<get_year();
    return "Date: "+sout.str();
}

Glib::ustring
ModificationDate::get_criteria_icon_path() const throw()
{
    //TBD::CORRECT
    return PACKAGE_DATA_DIR"/"PACKAGE_NAME"/pixmaps/tag-16.png";
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


} //namespace Solang
