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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <glib/gstdio.h>

#include "content-type-repo.h"
#include "photo.h"
#include "pixbuf-maker.h"

namespace Solang
{

static const guint CHUNK = 4096;

PixbufMaker::PixbufMaker(bool rotate) throw() :
    rotate_(rotate),
    fd_(-1),
    pixbufLoader_(),
    signalIO_()
{
}

PixbufMaker::~PixbufMaker() throw()
{
    stop_async();
}

PixbufMakerPtr
PixbufMaker::create(bool rotate) throw()
{
    return PixbufMakerPtr(new PixbufMaker(rotate));
}

gint
PixbufMaker::create_fd(const std::string & path) const
                       throw(Glib::FileError)
{
    const gint fd = g_open(path.c_str(), O_RDONLY);

    if (-1 == fd)
    {
        throw Glib::FileError(Glib::FileError::FAILED,
                              Glib::ustring::compose(
                                                 "%1: Failed to open",
                                                 path));
    }

    return fd;
}

PixbufLoaderPtr
PixbufMaker::create_pixbuf_loader(const std::string & path) const
                                  throw(Gdk::PixbufError)
{
    const ContentTypeRepoPtr content_type_repo
                                 = ContentTypeRepo::instance();
    const Glib::ustring content_type
        = content_type_repo->get_content_type(path);

    if (false == content_type_repo->is_gdk_supported(content_type))
    {
        throw Gdk::PixbufError(Gdk::PixbufError::UNKNOWN_TYPE, "");
    }

    return Gdk::PixbufLoader::create(content_type, true);
}

void
PixbufMaker::make_async(const SlotAsyncReady & slot,
                        const PhotoPtr & photo)
                        throw(Gdk::PixbufError, Glib::ConvertError,
                              Glib::FileError)
{
    const std::string path = Glib::filename_from_utf8(
                                 photo->get_disk_file_path());
    fd_ = create_fd(path);
    pixbufLoader_ = create_pixbuf_loader(path);

    signalIO_
        = Glib::signal_io().connect(
              sigc::bind(sigc::mem_fun(*this,
                                       &PixbufMaker::on_signal_io),
                         slot),
              fd_,
              Glib::IO_IN | Glib::IO_HUP,
              Glib::PRIORITY_DEFAULT_IDLE);
}

PixbufPtr
PixbufMaker::make_sync(const PhotoPtr & photo)
                       throw(Gdk::PixbufError, Glib::ConvertError,
                             Glib::FileError)
{
    const std::string path = Glib::filename_from_utf8(
                                 photo->get_disk_file_path());
    fd_ = create_fd(path);
    pixbufLoader_ = create_pixbuf_loader(path);
    gssize nread;
    guint8 buffer[CHUNK];

    while (0 < (nread = read(fd_, buffer, sizeof(buffer))))
    {
        try
        {
            pixbufLoader_->write(buffer, nread);
        }
        catch (const Glib::FileError & e)
        {
            g_warning("%s", e.what().c_str());
            break;
        }
        catch (const Gdk::PixbufError & e)
        {
            g_warning("%s", e.what().c_str());
            break;
        }
    }

    close(fd_);
    fd_ = -1;

    try
    {
        pixbufLoader_->close();
    }
    catch (const Glib::FileError & e)
    {
        g_warning("%s", e.what().c_str());
    }
    catch (const Gdk::PixbufError & e)
    {
        g_warning("%s", e.what().c_str());
    }

    PixbufPtr pixbuf = pixbufLoader_->get_pixbuf();
    if (0 == pixbuf)
    {
        throw Gdk::PixbufError(Gdk::PixbufError::FAILED, "");
    }

    if (true == rotate_)
    {
        pixbuf = Glib::wrap(gdk_pixbuf_apply_embedded_orientation(
                                pixbuf->gobj()), false);
    }

    return pixbuf;
}

bool
PixbufMaker::on_signal_io(Glib::IOCondition condition,
                          const SlotAsyncReady & slot) throw()
{
    guint8 buffer[CHUNK];
    const gssize nread = read(fd_, buffer, sizeof(buffer));

    try
    {
        if (0 == nread)
        {
            throw 1;
        }
        pixbufLoader_->write(buffer, nread);
    }
    catch (...)
    {
        stop_async();
    }

    if (-1 == fd_)
    {
        PixbufPtr pixbuf = pixbufLoader_->get_pixbuf();
        if (0 != pixbuf && true == rotate_)
        {
            pixbuf = Glib::wrap(gdk_pixbuf_apply_embedded_orientation(
                                    pixbuf->gobj()), false);
        }

        slot(pixbuf);
    }

    return -1 != fd_;
}

void
PixbufMaker::stop_async() throw()
{
    if (false == signalIO_)
    {
        return;
    }

    signalIO_.disconnect();
    close(fd_);
    fd_ = -1;

    try
    {
        pixbufLoader_->close();
    }
    catch (const Glib::FileError & e)
    {
        g_warning("%s", e.what().c_str());
    }
    catch (const Gdk::PixbufError & e)
    {
        g_warning("%s", e.what().c_str());
    }
}

} // namespace Solang
