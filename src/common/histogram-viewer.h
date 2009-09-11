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

} //namespace Solang

#endif //SOLANG_HISTOGRAM_VIEWER_H
