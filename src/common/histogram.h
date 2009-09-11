#ifndef SOLANG_HISTOGRAM_H
#define SOLANG_HISTOGRAM_H

#include <gtkmm.h>
#include "types.h"

namespace Solang
{

class Histogram
{
    public:
        enum Channel
        {
            RED = 0,
            GREEN,
            BLUE,
            RGB,
            //ALPHA,
            //VALUE
        };
        enum Type
        {
            LINEAR,
            LOGARITHMIC
        };

        struct DataBuffer
        {
            guint8 *data_;
            gsize size_;
            guint8 bitsPerChannel_;
            guint8 numChannels_;
            gsize width_;
            gsize height_;
            gsize rowstride_;
        };

        Histogram( );
        ~Histogram();

        void
        createData( const DataBuffer &data );

        Glib::RefPtr<Gdk::Pixbuf>
        get_pixbuf( Type type, gsize width, gsize height,
                            Channel channel = RGB ) throw();

    private:
        Glib::RefPtr<Gdk::Pixbuf>
        get_logarithmic_histogram(
                gsize width, gsize height,
                Channel resultChannel ) throw();

        Glib::RefPtr<Gdk::Pixbuf>
        get_linear_histogram(
                gsize width, gsize height,
                Channel resultChannel ) throw();

        void
        cleanup();

        struct Pixel
        {
            guint8 channels_[3];
        };
        guint32 **data_;
        guint32 max[3];
        guint32 maxLevel_;
        DataBuffer buffer_;

};

} //namespace Solang
#endif //SOLANG_HISTOGRAM_H
