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

#ifndef SOLANG_HISTOGRAM_VIEWER_H
#define SOLANG_HISTOGRAM_VIEWER_H

#include <gtkmm.h>

#include "histogram.h"

namespace Solang
{

class HistogramViewer
    : public Gtk::VBox
{
    private:
      class TypeSelectColumns
                : public Gtk::TreeModel::ColumnRecord
      {
          public:

            TypeSelectColumns()
            {
                add( type_ );
                add( name_ );
            }

            Gtk::TreeModelColumn<Histogram::Type> type_;
            Gtk::TreeModelColumn<Glib::ustring> name_;
      };
      class ChannelSelectColumns
                : public Gtk::TreeModel::ColumnRecord
      {
          public:

            ChannelSelectColumns()
            {
                add( channel_ );
                add( name_ );
            }

            Gtk::TreeModelColumn<Histogram::Channel> channel_;
            Gtk::TreeModelColumn<Glib::ustring> name_;
      };


    public:
        HistogramViewer( bool autoupdate = false );
        ~HistogramViewer();

        void
        set( const Glib::RefPtr<Gdk::Pixbuf> &image );

        void
        set_auto_update( bool autoupdate = true );

        void
        update(); //Not required if autoupdate is set

    private:
        void
        on_size_request(Gtk::Requisition *req);
        void
        size_changed(Gtk::Requisition *req);

        Histogram  histogram_;
        gsize width_;
        gsize height_;
        Glib::RefPtr<Gdk::Pixbuf> image_;
        Gtk::Image histogramView_;
        Gtk::HBox  hbType_;
        Gtk::Label lblType_;
        TypeSelectColumns typeCols_;
        Glib::RefPtr<Gtk::ListStore> typeStore_;
        Gtk::ComboBox cmbType_;
        Gtk::HBox  hbChannels_;
        Gtk::Label lblChannels_;
        ChannelSelectColumns channelCols_;
        Glib::RefPtr<Gtk::ListStore> channelStore_;
        Gtk::ComboBox cmbChannels_;
        bool autoupdate_;

};

} // namespace Solang

#endif // SOLANG_HISTOGRAM_VIEWER_H
