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

#ifndef SOLANG_FLICKR_CHOOSER_BUTTON_H
#define SOLANG_FLICKR_CHOOSER_BUTTON_H

#include <string>

#include <glibmm.h>
#include <gtkmm.h>

#include <flickcurl.h>

#include "types.h"

namespace Solang
{

class FlickrChooserButton :
    public Gtk::HBox
{
    public:
        FlickrChooserButton(const FlickrContextPtr & flickr_context)
                            throw();

        virtual
        ~FlickrChooserButton() throw();

        const std::list<std::string> &
        get_uris() throw();

    protected:
        void
        add_flickr_favorites() throw();

        void
        add_flickr_other() throw();

        void
        add_flickr_photosets() throw();

        void
        add_separator() throw();

        void
        on_flickr_chooser_dialog_response(
            gint response_id,
            FlickrChooserDialogPtr & flickr_chooser_dialog) throw();

        void
        on_combo_box_changed() throw();

        bool
        on_flickr_chooser_dialog_destroy(
            FlickrChooserDialogPtr & flickr_chooser_dialog) throw();

        void
        on_read_flickr_end() throw();

        bool
        on_row_separator(const TreeModelPtr & model,
                         const Gtk::TreeModel::iterator & iter)
                         throw();

        void
        on_select_button_clicked() throw();

        void
        read_flickr() throw();

        void
        read_flickr_favorites(const std::string & nsid) throw();

        void
        read_flickr_other(const std::string & nsid) throw();

        void
        read_flickr_photosets(const std::string & nsid) throw();

        void
        set_busy(bool busy) throw();

        void
        spawn_read_flickr() throw();

        FlickrContextPtr flickrContext_;

        flickcurl_photo ** favoritePhotos_;

        flickcurl_photoset ** photosets_;

        std::list<flickcurl_photo **> photosetPhotos_;

        flickcurl_photo ** otherPhotos_;

        Glib::ThreadPool threadPool_;

        ListStorePtr listStore_;

        Gtk::ComboBox comboBox_;

        Gtk::CellRendererPixbuf rendererPixbuf_;

        Gtk::CellRendererText rendererText_;

        Gtk::Button selectButton_;

        Gtk::Image selectImage_;

        Glib::Dispatcher readFlickrEnd_;

        std::list<std::string> uris_;

    private:
};

} // namespace Solang

#endif // SOLANG_FLICKR_CHOOSER_BUTTON_H
