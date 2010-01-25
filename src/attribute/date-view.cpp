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

#include "application.h"
#include "date-view.h"
#include "date-photo-info.h"

namespace Solang
{

DateView::DateView() throw() :
    Gtk::TreeView(),
    application_(),
    modelColumnRecord_(),
    treeStore_(Gtk::TreeStore::create(modelColumnRecord_)),
    percentage_( )
{
    configure();
}

DateView::~DateView() throw()
{
    unset_model();
}

void
DateView::configure() throw()
{
    set_model(treeStore_);

    append_column("Entry",
                  modelColumnRecord_.get_column_entry());
    int col = append_column("Percentage", percentage_ );
    Gtk::TreeViewColumn* pColumn = get_column(col - 1);
#ifdef GLIBMM_PROPERTIES_ENABLED
    pColumn->add_attribute(percentage_.property_value(),
                  modelColumnRecord_.get_column_percentage());
    pColumn->add_attribute(percentage_.property_text(),
                  modelColumnRecord_.get_column_photo_count());
#else
    pColumn->add_attribute(percentage_, "value",
                  modelColumnRecord_.get_column_percentage());
    pColumn->add_attribute(percentage_, "text",
                  modelColumnRecord_.get_column_photo_count());
#endif

    set_tooltip_column( modelColumnRecord_.get_column_photo_count_num() );
    set_enable_search(true);
    // set_fixed_height_mode(true);
    set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_NONE);
    set_headers_clickable(false);
    set_headers_visible(false);

    std::vector<Gtk::TargetEntry> targets;
    targets.push_back(Gtk::TargetEntry("STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    targets.push_back(Gtk::TargetEntry("UTF8_STRING",
                                       Gtk::TARGET_SAME_APP, 0));
    enable_model_drag_source(targets,
                            Gdk::MODIFIER_MASK, Gdk::ACTION_COPY);

}

void
DateView::populate() throw()
{
    treeStore_->clear();

    typedef DatePhotoInfoList::const_iterator DPIterator;

    Engine &engine = application_->get_engine();
    DatePhotoInfoList years = engine.get_dates_with_picture_count();
    for( DPIterator year = years.begin(); year != years.end(); year++ )
    {
        Gtk::TreeModel::Row yr = *(treeStore_->append());

        DatePhotoInfo info = (*year);
        ModificationDate dateYr = info.get_date();
        yr[ modelColumnRecord_.get_column_date() ]  = dateYr;
        std::ostringstream sout;
        sout<<dateYr.get_year();
        yr[ modelColumnRecord_.get_column_entry() ] = sout.str();
        yr[ modelColumnRecord_.get_column_percentage() ] = 100;
        yr[ modelColumnRecord_.get_column_photo_count() ]
                                                = info.get_count();
        DatePhotoInfoList months
                            = engine.get_dates_with_picture_count(
                                                dateYr.get_year());
        for( DPIterator month = months.begin();
                                month != months.end(); month++ )
        {

            Gtk::TreeModel::Row mth = *(treeStore_->append(
                                                    yr.children() ));

            DatePhotoInfo infoM = (*month);
            ModificationDate dateM = infoM.get_date();
            double percentage = 100.0
                        * (static_cast<double>(infoM.get_count())
                            / static_cast<double>( info.get_count()));
            mth[ modelColumnRecord_.get_column_date() ]  = dateM;
            mth[ modelColumnRecord_.get_column_entry() ]
                            = map_month_to_name( dateM.get_month() );
            mth[ modelColumnRecord_.get_column_percentage() ]
                                                          = percentage;
            mth[ modelColumnRecord_.get_column_photo_count() ]
                                                = infoM.get_count();
            DatePhotoInfoList days
                                = engine.get_dates_with_picture_count(
                                  dateM.get_year(), dateM.get_month());
            for( DPIterator day = days.begin();
                                    day != days.end(); day++ )
            {

                Gtk::TreeModel::Row dy
                            = *(treeStore_->append(mth.children()));

                DatePhotoInfo infoD = (*day);
                ModificationDate dateD = infoD.get_date();
                std::ostringstream sout;
                sout<<dateD.get_day();
                double percentage = 100.0
                        * (static_cast<double>(infoD.get_count())
                            / static_cast<double>( infoM.get_count()));
                dy[ modelColumnRecord_.get_column_date() ]  = dateD;
                dy[ modelColumnRecord_.get_column_entry() ]
                                                        = sout.str();
                dy[ modelColumnRecord_.get_column_percentage() ]
                                                          = percentage;
                dy[ modelColumnRecord_.get_column_photo_count() ]
                                                = infoD.get_count();
            }
        }
    }
}

void
DateView::on_drag_data_get(
          const Glib::RefPtr<Gdk::DragContext>& context,
          Gtk::SelectionData& selection_data, guint info, guint time)
{
    if( !application_ )
    {
        return;
    }
    const TreeSelectionPtr selected = get_selection();
    if( 0 == selected->count_selected_rows() )
        return;

    Gtk::TreeModel::iterator item = selected->get_selected();

    Gtk::TreeModel::Row row= (*item);
    ModificationDatePtr key( new ModificationDate(
                        row[modelColumnRecord_.get_column_date()] ) );
    Glib::ustring criteria = key->get_query_criteria();
    application_->set_drag_item( criteria, key );
    selection_data.set_text( criteria );

    return;

}

void
DateView::set_application( ApplicationPtr application ) throw()
{
    application_ = application;
}

Glib::ustring
DateView::map_month_to_name(gint month) const throw()
{
    switch( month )
    {
        case 1:
            return "January";
        case 2:
            return "February";
        case 3:
            return "March";
        case 4:
            return "April";
        case 5:
            return "May";
        case 6:
            return "June";
        case 7:
            return "July";
        case 8:
            return "August";
        case 9:
            return "September";
        case 10:
            return "October";
        case 11:
            return "November";
    }
    return "December";
}

} // namespace Solang
