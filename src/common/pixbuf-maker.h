/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_PIXBUF_MAKER_H
#define SOLANG_PIXBUF_MAKER_H

#include <gdkmm.h>
#include <glibmm.h>
#include <sigc++/sigc++.h>

#include "types.h"

namespace Solang
{

class PixbufMaker
{
    public:
        typedef sigc::slot<void, const PixbufPtr &> SlotAsyncReady;

        ~PixbufMaker() throw();

        static PixbufMakerPtr
        create(bool rotate) throw();

        void
        make_async(const SlotAsyncReady & slot,
                   const PhotoPtr & photo) throw(Gdk::PixbufError,
                                                 Glib::ConvertError,
                                                 Glib::FileError);

        PixbufPtr
        make_sync(const PhotoPtr & photo) throw(Gdk::PixbufError,
                                                Glib::ConvertError,
                                                Glib::FileError);

        void
        stop_async() throw();

    protected:
        PixbufMaker(bool rotate) throw();

        PixbufMaker(const PixbufMaker & source) throw();

        PixbufMaker &
        operator=(const PixbufMaker & source) throw();

        gint
        create_fd(const std::string & path) const
                  throw(Glib::FileError);

        PixbufLoaderPtr
        create_pixbuf_loader(const std::string & path) const
                             throw(Gdk::PixbufError);

        bool
        on_signal_io(Glib::IOCondition condition,
                     const SlotAsyncReady & slot) throw();

        bool rotate_;

        gint fd_;

        PixbufLoaderPtr pixbufLoader_;

        sigc::connection signalIO_;

    private:
};

} // namespace Solang

#endif // SOLANG_PIXBUF_MAKER_H
