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
#endif //HAVE_CONFIG_H

#include <glibmm/i18n.h>

#include "histogram-viewer.h"

namespace Solang
{

HistogramViewer::HistogramViewer( bool autoupdate )
    :histogram_(),
    width_( 192 ),
    height_( 144 ),
    image_(),
    histogramView_(),
    hbType_(),
    lblType_( _("Type") ),
    typeCols_(),
    typeStore_( Gtk::ListStore::create( typeCols_ ) ),
    cmbType_(),
    hbChannels_(),
    lblChannels_( _("Channels") ),
    channelCols_(),
    channelStore_( Gtk::ListStore::create( channelCols_ ) ),
    cmbChannels_(),
    autoupdate_( true )
{
    hbType_.pack_start( lblType_, Gtk::PACK_SHRINK, 6 );
    hbType_.pack_start( cmbType_, Gtk::PACK_EXPAND_WIDGET, 6 );
    pack_start( hbType_, Gtk::PACK_EXPAND_WIDGET, 6);

    histogramView_.set_size_request( 192, 144 );
    pack_start(histogramView_, Gtk::PACK_EXPAND_WIDGET, 6);

    hbChannels_.pack_start( lblChannels_, Gtk::PACK_SHRINK, 6 );
    hbChannels_.pack_end( cmbChannels_, Gtk::PACK_EXPAND_WIDGET, 6 );
    pack_start( hbChannels_, Gtk::PACK_SHRINK, 6 );

    cmbType_.set_model( typeStore_ );
    cmbType_.pack_start( typeCols_.name_ );
    cmbChannels_.set_model( channelStore_ );
    cmbChannels_.pack_start( channelCols_.name_ );
    //Types
    {
        Gtk::TreeModel::Row row = *(typeStore_->append());
        row[ typeCols_.type_ ] = Histogram::LINEAR;
        row[ typeCols_.name_ ] =  _("Linear") ;
    }
    {
        Gtk::TreeModel::Row row = *(typeStore_->append());
        row[ typeCols_.type_ ] = Histogram::LOGARITHMIC;
        row[ typeCols_.name_ ] =  _("Logarithmic") ;
    }
    cmbType_.set_active( 0 );

    //Channels
    {
        Gtk::TreeModel::Row row = *(channelStore_->append());
        row[ channelCols_.channel_ ] = Histogram::RGB;
        row[ channelCols_.name_ ] =  _("RGB") ;
    }
    {
        Gtk::TreeModel::Row row = *(channelStore_->append());
        row[ channelCols_.channel_ ] = Histogram::RED;
        row[ channelCols_.name_ ] =  _("Red") ;
    }
    {
        Gtk::TreeModel::Row row = *(channelStore_->append());
        row[ channelCols_.channel_ ] = Histogram::GREEN;
        row[ channelCols_.name_ ] =  _("Green") ;
    }
    {
        Gtk::TreeModel::Row row = *(channelStore_->append());
        row[ channelCols_.channel_ ] = Histogram::BLUE;
        row[ channelCols_.name_ ] =  _("Blue") ;
    }
    cmbChannels_.set_active( 0 );

    cmbType_.signal_changed().connect(
        sigc::mem_fun( *this, &HistogramViewer::update ) );
    cmbChannels_.signal_changed().connect(
        sigc::mem_fun( *this, &HistogramViewer::update ) );
    show_all_children();
    signal_size_request().connect(
        sigc::mem_fun( *this,
                    &HistogramViewer::size_changed ));
    update();
}

HistogramViewer::~HistogramViewer()
{
}

void
HistogramViewer::set( const Glib::RefPtr<Gdk::Pixbuf> &image )
{
    image_ = image;
    if( image_ )
    {
        Histogram::DataBuffer imageData;
        imageData.data_ = image_->get_pixels();
        imageData.bitsPerChannel_ = image_->get_bits_per_sample();
        imageData.numChannels_ = image_->get_n_channels();
        imageData.width_ = image_->get_width();
        imageData.height_ = image_->get_height();
        imageData.rowstride_ = image_->get_rowstride();
        histogram_.createData( imageData );
    }
    if( true == autoupdate_ )
        update();
}

void
HistogramViewer::set_auto_update( bool autoupdate )
{
    autoupdate_ = autoupdate;
}

void
HistogramViewer::update()
{
    Gtk::TreeModel::const_iterator typeI
                                = cmbType_.get_active();
    Gtk::TreeModel::Row typeR = (*typeI);
    Histogram::Type type = typeR[ typeCols_.type_ ];
    Gtk::TreeModel::const_iterator channelI
                                = cmbChannels_.get_active();
    Gtk::TreeModel::Row channelR = (*channelI);
    Histogram::Channel channel
                         = channelR[ channelCols_.channel_ ];
    Glib::RefPtr<Gdk::Pixbuf> pixbuf
        = histogram_.get_pixbuf( type, width_, height_, channel );
    histogramView_.set( pixbuf );
    return;
}
void
HistogramViewer::on_size_request( Gtk::Requisition *requisition )
{
//    gint64 oldWidth = get_width();
//    gint64 oldHeight = get_height();
    Gtk::VBox::on_size_request( requisition );
#if 0
    gint64 width = width_ + requisition->width - oldWidth;
    gint64 height = height_ + requisition->height - oldHeight;
    width_ = (width > 0 ) ? width : width_;
    height_ = (height > 0 ) ? height : height_;
#endif
    //if( width_ > 0 && height_ > 0 )
    //    histogramView_.set_size_request( width_, height_ );
    width_ = requisition->width;
//    height_ = requisition->height;
#if 0
    freeze_child_notify();
    Glib::RefPtr<Gdk::Pixbuf> buffer
                            = histogramView_.get_pixbuf();
    if( !buffer )
        return;
    histogramView_.set( buffer->scale_simple( width_, height_,
                        Gdk::INTERP_HYPER ) );
    thaw_child_notify();
#endif
    //update();
}

void
HistogramViewer::size_changed(Gtk::Requisition *req)
{
#if 0
    int width = req->width;
    int height = get_height();
    width_ = width;
    //update();
#endif

}

} // namespace Solang
