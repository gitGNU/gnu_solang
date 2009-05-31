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

/*
 * Based on code by: the gedit Team
 *
 * This widget was originally written in C as a part of gedit:
 * gedit/gedit-spinner.h
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "spinner.h"

namespace Solang
{

static const Gtk::IconSize LAST_ICON_SIZE(Gtk::ICON_SIZE_DIALOG + 1);

static const guint SPINNER_TIMEOUT = 125; // ms

static const std::string SPINNER_ICON_NAME = "process-working";
static const std::string SPINNER_FALLBACK_ICON_NAME = "gnome-spinner";

static Images * const SPINNER_IMAGES_INVALID
                          = reinterpret_cast<Images *>(0x1);

class Images
{
    public:
        Images(const Gtk::IconSize & size,
               gint width, gint height,
               const PixbufList & animation_pixbufs,
               guint n_animation_pixbufs) throw();

        ~Images() throw();

        static ImagesPtr
        load(const ScreenPtr & screen,
             const IconThemePtr & icon_theme,
             Gtk::IconSize icon_size) throw();

        gint
        get_width() const throw();

        gint
        get_height() const throw();

        const PixbufList &
        get_animation_pixbufs() const throw();

        PixbufList &
        get_animation_pixbufs() throw();

        guint
        get_n_animation_pixbufs() const throw();

    protected:

    private:
        static PixbufPtr
        extract_frame(const PixbufPtr & grid_pixbuf,
                      gint x, gint y, gint size) throw();

        static PixbufPtr
        scale_to_size(const PixbufPtr & pixbuf,
                      gint dw, gint dh);

        Gtk::IconSize size_;

        gint width_;

        gint height_;

        PixbufList animationPixbufs_;

        guint nAnimationPixbufs_;
};

Images::Images(const Gtk::IconSize & size,
               gint width, gint height,
               const PixbufList & animation_pixbufs,
               guint n_animation_pixbufs) throw() :
    size_(size),
    width_(width),
    height_(height),
    animationPixbufs_(animation_pixbufs),
    nAnimationPixbufs_(n_animation_pixbufs)
{
}

Images::~Images() throw()
{
    PixbufList::iterator iter;

    for (iter = animationPixbufs_.begin();
         animationPixbufs_.end() != iter;
         iter++)
    {
        iter->reset();
    }
}

PixbufPtr
Images::extract_frame(const PixbufPtr & grid_pixbuf,
                      gint x, gint y, gint size) throw()
{
    if (x + size > grid_pixbuf->get_width()
        || y + size > grid_pixbuf->get_height())
    {
        return PixbufPtr(0);
    }

    const PixbufPtr pixbuf = Gdk::Pixbuf::create_subpixbuf(
                                 grid_pixbuf,
                                 x, y,
                                 size, size);

    return pixbuf;
}

ImagesPtr
Images::load(const ScreenPtr & screen,
             const IconThemePtr & icon_theme,
             Gtk::IconSize icon_size) throw()
{
    const SettingsPtr settings = Gtk::Settings::get_for_screen(screen);
    gint ish;
    gint isw;

    if (false == Gtk::IconSize::lookup(icon_size, isw, ish, settings))
    {
        return ImagesPtr();
    }

    const gint requested_size = std::max(ish, isw);
    Gtk::IconInfo icon_info
                      = icon_theme->lookup_icon(
                            SPINNER_ICON_NAME,
                            requested_size,
                            static_cast<Gtk::IconLookupFlags>(0));

    if (false == icon_info)
    {
        g_warning("Throbber animation not found");

        // If the icon naming spec compliant name wasn't found, try
        // the old name.
        icon_info = icon_theme->lookup_icon(
                        SPINNER_FALLBACK_ICON_NAME,
                        requested_size,
                        static_cast<Gtk::IconLookupFlags>(0));

        if (false == icon_info)
        {
            g_warning("Throbber fallback animation not found either");
        }
    }

    g_assert(icon_info != 0);

    const gint size = icon_info.get_base_size();
    const Glib::ustring icon = icon_info.get_filename();

    if (true == icon.empty())
    {
        return ImagesPtr();
    }

    PixbufPtr icon_pixbuf;

    try
    {
        icon_pixbuf = Gdk::Pixbuf::create_from_file(icon);
    }
    catch (const Gdk::PixbufError & e)
    {
        g_warning("%s", e.what().c_str());
        return ImagesPtr();
    }

    const gint grid_width = icon_pixbuf->get_width();
    const gint grid_height = icon_pixbuf->get_height();

    gint n = 0;
    gint x;
    gint y;
    PixbufList list;

    for (y = 0; y < grid_height; y += size)
    {
        for (x = 0; x < grid_width; x += size)
        {
            const PixbufPtr pixbuf = extract_frame(icon_pixbuf,
                                                   x, y, size);
            if (0 == pixbuf)
            {
                continue;
            }

            list.push_back(pixbuf);
            ++n;
        }
    }

    if (size > requested_size)
    {
        PixbufList::iterator iter;
        for (iter = list.begin(); list.end() != iter; iter++)
        {
            const PixbufPtr scaled_pixbuf = scale_to_size(*iter,
                                                          isw, ish);
            iter->reset();
            *iter = scaled_pixbuf;
        }
    }

    ImagesPtr images(new Images(icon_size, requested_size,
                                requested_size, list, n));

    return images;
}

PixbufPtr
Images::scale_to_size(const PixbufPtr & pixbuf, gint dw, gint dh)
{
    if (0 == pixbuf)
    {
        return PixbufPtr(0);
    }

    const gint pw = pixbuf->get_width();
    const gint ph = pixbuf->get_height();

    if (pw != dw || ph != dh)
    {
        const PixbufPtr result = pixbuf->scale_simple(dw, dh,
                                     Gdk::INTERP_BILINEAR);
        return result;
    }

    return pixbuf;
}

gint
Images::get_width() const throw()
{
    return width_;
}

gint
Images::get_height() const throw()
{
    return height_;
}

const PixbufList &
Images::get_animation_pixbufs() const throw()
{
    return animationPixbufs_;
}

PixbufList &
Images::get_animation_pixbufs() throw()
{
    return animationPixbufs_;
}

guint
Images::get_n_animation_pixbufs() const throw()
{
    return nAnimationPixbufs_;
}

class CacheData :
    public sigc::trackable
{
    public:
        CacheData(const ScreenPtr & screen) throw();

        ~CacheData() throw();

        void
        unload() throw();

        const IconThemePtr &
        get_icon_theme() const throw();

        const ImagesList &
        get_images() const throw();

        ImagesList &
        get_images() throw();

    protected:

    private:
        ScreenPtr screen_;

        IconThemePtr iconTheme_;

        ImagesList images_;

        sigc::connection signalChanged_;
};

CacheData::CacheData(const ScreenPtr & screen) throw() :
    sigc::trackable(),
    screen_(screen),
    iconTheme_(Gtk::IconTheme::get_for_screen(screen)),
    images_(LAST_ICON_SIZE),
    signalChanged_()
{
    signalChanged_
        = iconTheme_->signal_changed().connect(
              sigc::mem_fun(*this,
                            &CacheData::unload));
}

CacheData::~CacheData() throw()
{
    signalChanged_.disconnect();
    unload();
}

void
CacheData::unload() throw()
{
    gint size;

    for (size = Gtk::ICON_SIZE_INVALID; size < LAST_ICON_SIZE; size++)
    {
        if (0 != images_[size]
            && SPINNER_IMAGES_INVALID != images_[size].get())
        {
            images_[size].reset();
        }
    }
}

const IconThemePtr &
CacheData::get_icon_theme() const throw()
{
    return iconTheme_;
}

const ImagesList &
CacheData::get_images() const throw()
{
    return images_;
}

ImagesList &
CacheData::get_images() throw()
{
    return images_;
}

class Cache
{
    public:
        ~Cache() throw();

        static CachePtr
        create() throw();

        ImagesPtr
        get_images(ScreenPtr screen, Gtk::IconSize icon_size) throw();

    protected:
        Cache() throw();

    private:
        std::map<ScreenPtr, CacheData> hash_;
};

Cache::Cache() throw() :
    hash_()
{
}

Cache::~Cache() throw()
{
}

CachePtr
Cache::create() throw()
{
    static std::tr1::weak_ptr<Cache> weak_spinner;

    if (false == weak_spinner.expired())
    {
        return weak_spinner.lock();
    }

    CachePtr spinner(new Cache());

    weak_spinner = spinner;
    return spinner;
}

ImagesPtr
Cache::get_images(ScreenPtr screen, Gtk::IconSize icon_size)
                         throw()
{
    if (icon_size < 0 || icon_size >= LAST_ICON_SIZE)
    {
        return ImagesPtr();
    }

    if (Gtk::ICON_SIZE_INVALID == icon_size)
    {
        icon_size = Gtk::ICON_SIZE_DIALOG;
    }

    std::map<ScreenPtr, CacheData>::iterator iter
        = hash_.find(screen);

    if (hash_.end() == iter)
    {
        CacheData cache_data(screen);
        iter = hash_.insert(iter, std::make_pair(screen, cache_data));
    }

    ImagesList & images_list = iter->second.get_images();
    ImagesPtr images = images_list[icon_size];

    if (SPINNER_IMAGES_INVALID == images.get())
    {
        return ImagesPtr();
    }

    if (0 != images)
    {
        return images;
    }

    images = Images::load(screen, iter->second.get_icon_theme(),
                          icon_size);
    if (0 == images)
    {
        //images_list[icon_size] = SPINNER_IMAGES_INVALID;
        return ImagesPtr();
    }

    images_list[icon_size] = images;
    return images;
}

Spinner::Spinner() throw() :
    Gtk::Widget(),
    iconTheme_(),
    cache_(Cache::create()),
    size_(Gtk::ICON_SIZE_DIALOG),
    images_(),
    currentImage_(),
    timeout_(SPINNER_TIMEOUT),
    spinning_(false),
    needLoad_(true),
    signalChanged_(),
    signalTimeout_()
{
    set_flags(Gtk::NO_WINDOW);
}

Spinner::~Spinner() throw()
{
    if (true == signalChanged_.connected())
    {
        signalChanged_.disconnect();
    }

    if (true == signalTimeout_.connected())
    {
        signalTimeout_.disconnect();
    }

    unload_images();
}

bool
Spinner::load_images() throw()
{
    if (true == needLoad_)
    {
        images_ = cache_->get_images(get_screen(), size_);
        currentImage_ = 0;
        needLoad_ = false;
    }

    return 0 != images_;
}

void
Spinner::unload_images() throw()
{
    if (0 != images_)
    {
        images_.reset();
    }

    currentImage_ = 0;
    needLoad_ = true;
}

bool
Spinner::on_expose_event(GdkEventExpose * event)
{
    if (false == is_drawable())
    {
        return false;
    }

    if (true == needLoad_ && false == load_images())
    {
        return false;
    }

    if (0 == images_)
    {
        return false;
    }

    g_assert(images_->get_n_animation_pixbufs() > 0);
    g_assert(currentImage_ >= 0
             && currentImage_ < images_->get_n_animation_pixbufs());

    PixbufList & images = images_->get_animation_pixbufs();
    PixbufPtr pixbuf = images[currentImage_];
    g_assert(true == pixbuf);

    const gint width = pixbuf->get_width();
    const gint height = pixbuf->get_height();

    const Gtk::Allocation allocation = get_allocation();

    const gint x_offset = (allocation.get_width() - width) / 2;
    const gint y_offset = (allocation.get_height() - height) / 2;

    Gdk::Rectangle pix_area(x_offset + allocation.get_x(),
                            y_offset + allocation.get_y(),
                            width, height);
    Gdk::Rectangle event_area(&event->area);

    bool result;
    Gdk::Rectangle & dest = event_area.intersect(pix_area, result);

    if (false == result)
    {
        return false;
    }

    WindowPtr window = get_window();
    GCPtr gc = Gdk::GC::create(window);

    window->draw_pixbuf(gc, pixbuf,
                        dest.get_x() - x_offset - allocation.get_x(),
                        dest.get_y() - y_offset - allocation.get_y(),
                        dest.get_x(), dest.get_y(),
                        dest.get_width(), dest.get_height(),
                        Gdk::RGB_DITHER_MAX, 0, 0);

    return false;
}

void
Spinner::on_icon_theme_changed() throw()
{
    unload_images();
    queue_resize();
}

void
Spinner::on_map()
{
    Gtk::Widget::on_map();
    if (true == spinning_)
    {
        start();
    }
}

void
Spinner::on_screen_changed(const ScreenPtr & old_screen)
{
    Gtk::Widget::on_screen_changed(old_screen);

    const ScreenPtr screen = get_screen();

    // FIXME: this seems to be happening when the spinner is
    //        destroyed!?
    if (old_screen == screen)
    {
        return;
    }

    signalTimeout_.disconnect();
    unload_images();

    if (true == old_screen)
    {
        signalChanged_.disconnect();
    }

    iconTheme_ = Gtk::IconTheme::get_for_screen(screen);
    signalChanged_
        = iconTheme_->signal_changed().connect(
              sigc::mem_fun(*this,
                            &Spinner::on_icon_theme_changed));
}

void
Spinner::on_size_request(Gtk::Requisition * requisition)
{
    if ((true == needLoad_ && false == load_images())
        || 0 == images_)
    {
        requisition->width = requisition->height = 0;
        const SettingsPtr settings = get_settings();
        Gtk::IconSize::lookup(size_, requisition->width,
                              requisition->height, settings);
        return;
    }

    requisition->width = images_->get_width();
    requisition->height = images_->get_height();

    // FIXME: fix this hack.
    // Allocate some extra margin so we don't butt up against
    // toolbar edges.
    if (Gtk::ICON_SIZE_MENU != size_)
    {
        requisition->width += 2;
        requisition->height += 2;
    }
}

bool
Spinner::on_timeout() throw()
{
    if (0 == images_)
    {
        return true;
    }

    currentImage_++;
    if (currentImage_ >= images_->get_n_animation_pixbufs())
    {
        // The 0th frame is the 'rest' icon.
        currentImage_ = std::min(
                            static_cast<guint>(1),
                            images_->get_n_animation_pixbufs());
    }

    queue_draw();
    return true;
}

void
Spinner::on_unmap()
{
    signalTimeout_.disconnect();
    Gtk::Widget::on_unmap();
}

void
Spinner::start() throw()
{
    spinning_ = true;

    if (true == is_mapped()
        && false == signalTimeout_.connected()
        && load_images())
    {
        // The 0th frame is the 'rest' icon.
        currentImage_ = std::min(
                            static_cast<guint>(1),
                            images_->get_n_animation_pixbufs());

        signalTimeout_
            = Glib::signal_timeout().connect(
                  sigc::mem_fun(*this,
                                &Spinner::on_timeout),
                  timeout_, Glib::PRIORITY_LOW);
    }
}

void
Spinner::stop() throw()
{
    spinning_ = false;
    currentImage_ = 0;

    if (true == signalTimeout_.connected())
    {
        signalTimeout_.disconnect();
        if (true == is_mapped())
        {
            queue_draw();
        }
    }
}

void
Spinner::set_size(const Gtk::IconSize & size) throw()
{
    if (Gtk::ICON_SIZE_INVALID == size)
    {
        size_ = Gtk::ICON_SIZE_DIALOG;
    }

    if (size != size_)
    {
        unload_images();
        size_ = size;
        queue_resize();
    }
}

} // namespace Solang
