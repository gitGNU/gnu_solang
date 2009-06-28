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

#ifndef SOLANG_FLICKR_CHOOSER_DIALOG_H
#define SOLANG_FLICKR_CHOOSER_DIALOG_H

#include <string>
#include <vector>

#include <flickcurl.h>
#include <glibmm.h>
#include <gtkmm.h>
#include <libsoup/soup.h>

#include "flickr-enums.h"
#include "spinner.h"
#include "types.h"

namespace Solang
{

class FlickrChooserDialog :
    public Gtk::Dialog
{
    public:
        FlickrChooserDialog(const FlickrContextPtr & flickr_context,
                            flickcurl_photo ** photo) throw();

        virtual
        ~FlickrChooserDialog() throw();

        std::list<std::string>
        get_uris() const throw();

        void
        on_file_loaded(SoupMessage * soup_message,
                       const Gtk::TreeModel::iterator & iter) throw();

    protected:
        virtual bool
        on_delete_event(GdkEventAny * event);

        virtual void
        on_response(int response_id);

        void
        on_read_flickr_end() throw();

        void
        on_read_flickr_progress() throw();

        void
        on_tree_view_row_activated(const Gtk::TreeModel::Path & path,
                                   Gtk::TreeViewColumn * column)
                                   throw();

        void
        read_flickr() throw();

        void
        set_busy(bool busy) throw();

        void
        spawn_read_flickr() throw();

        FlickrContextPtr flickrContext_;

        FlickrType flickrType_;

        flickcurl_photo ** photos_;

        Glib::ThreadPool threadPool_;

        Gtk::VBox vBox_;

        Gtk::ScrolledWindow scrolledWindow_;

        ListStorePtr listStore_;

        Gtk::TreeView treeView_;

        Gtk::CellRendererText rendererText_;

        Gtk::HBox hBox_;

        Spinner spinner_;

        Gtk::Label spinnerLabel_;

        Glib::Dispatcher readFlickrEnd_;

        Glib::Dispatcher readFlickrProgress_;

        gint current_;

        ProgressObserverPtr observer_;

        SoupSession * soupSession_;

        std::vector<std::string> uris_;

    private:
};

} // namespace Solang

#endif // SOLANG_FLICKR_CHOOSER_DIALOG_H
