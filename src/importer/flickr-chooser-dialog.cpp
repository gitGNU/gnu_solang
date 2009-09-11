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

#include <cstdlib>

#include <gdkmm.h>
#include <glibmm/i18n.h>
#include <libsoup/soup-method.h>

#include "flickr-chooser-dialog.h"
#include "flickr-context.h"
#include "flickr-photo-uri-builder.h"
#include "progress-observer.h"

namespace Solang
{

enum
{
    COLUMN_SELECTED = 0,
    COLUMN_PIXBUF,
    COLUMN_DETAILS,
    COLUMN_PHOTO,
    COLUMN_COUNT
};

struct SoupSessionCBData
{
    FlickrChooserDialog * flickr_chooser_dialog;
    Gtk::TreeModel::iterator iter;
};

static const Gtk::IconSize FLICKR_THUMBNAIL_SIZE(75);

static void
soup_session_on_file_loaded(SoupSession * soup_session,
                            SoupMessage * soup_message,
                            gpointer user_data) throw()
{
    const SoupSessionCBData * const data
        = static_cast<SoupSessionCBData *>(user_data);

    if (0 == user_data || 0 == data->flickr_chooser_dialog)
    {
        g_warning("Not an instance of FlickrChooserDialog");
        return;
    }

    data->flickr_chooser_dialog->on_file_loaded(soup_message,
                                                data->iter);

    delete data;
}

class FlickrDialogModelColumnRecord :
    public Gtk::TreeModelColumnRecord
{
    public:
        FlickrDialogModelColumnRecord() throw();

        virtual
        ~FlickrDialogModelColumnRecord() throw();

        const Gtk::TreeModelColumn<bool> &
        get_column_selected() const throw();

        gint
        get_column_selected_num() const throw();

        const Gtk::TreeModelColumn<PixbufPtr> &
        get_column_pixbuf() const throw();

        gint
        get_column_pixbuf_num() const throw();

        const Gtk::TreeModelColumn<Glib::ustring> &
        get_column_details() const throw();

        gint
        get_column_details_num() const throw();

        const Gtk::TreeModelColumn<flickcurl_photo *> &
        get_column_photo() const throw();

        gint
        get_column_photo_num() const throw();

    protected:
        Gtk::TreeModelColumn<bool> columnSelected_;

        Gtk::TreeModelColumn<PixbufPtr> columnPixbuf_;

        Gtk::TreeModelColumn<Glib::ustring> columnDetails_;

        Gtk::TreeModelColumn<flickcurl_photo *> columnPhoto_;

    private:
};

FlickrDialogModelColumnRecord::FlickrDialogModelColumnRecord()
                                   throw() :
    columnSelected_(),
    columnPixbuf_(),
    columnDetails_(),
    columnPhoto_()
{
    add(columnSelected_);
    add(columnPixbuf_);
    add(columnDetails_);
    add(columnPhoto_);
}

FlickrDialogModelColumnRecord::~FlickrDialogModelColumnRecord()
                                   throw()
{
}

const Gtk::TreeModelColumn<bool> &
FlickrDialogModelColumnRecord::get_column_selected() const throw()
{
    return columnSelected_;
}

gint
FlickrDialogModelColumnRecord::get_column_selected_num() const throw()
{
    return COLUMN_SELECTED;
}

const Gtk::TreeModelColumn<PixbufPtr> &
FlickrDialogModelColumnRecord::get_column_pixbuf() const throw()
{
    return columnPixbuf_;
}

gint
FlickrDialogModelColumnRecord::get_column_pixbuf_num() const throw()
{
    return COLUMN_PIXBUF;
}

const Gtk::TreeModelColumn<Glib::ustring> &
FlickrDialogModelColumnRecord::get_column_details() const throw()
{
    return columnDetails_;
}

gint
FlickrDialogModelColumnRecord::get_column_details_num() const throw()
{
    return COLUMN_DETAILS;
}

const Gtk::TreeModelColumn<flickcurl_photo *> &
FlickrDialogModelColumnRecord::get_column_photo() const throw()
{
    return columnPhoto_;
}

gint
FlickrDialogModelColumnRecord::get_column_photo_num() const throw()
{
    return COLUMN_PHOTO;
}

FlickrChooserDialog::FlickrChooserDialog(
                         const FlickrContextPtr & flickr_context,
                         flickcurl_photo ** photo) throw() :
    Gtk::Dialog(_("Select Photos"), true, false),
    flickrContext_(flickr_context),
    photos_(photo),
    threadPool_(1, false),
    vBox_(false, 6),
    scrolledWindow_(),
    listStore_(Gtk::ListStore::create(
                   FlickrDialogModelColumnRecord())),
    treeView_(listStore_),
    rendererText_(),
    hBox_(false, 12),
    spinner_(),
    spinnerLabel_(_("Loading photo information..."), Gtk::ALIGN_LEFT,
                  Gtk::ALIGN_CENTER, true),
    readFlickrEnd_(),
    readFlickrProgress_(),
    current_(0),
    observer_(new ProgressObserver()),
    soupSession_(soup_session_async_new_with_options(
                     "user-agent",
                     Glib::get_application_name().c_str(),
                     NULL)),
    uris_()
{
    set_border_width(12);
    set_default_size(450, 600);

    Gtk::VBox * const dialog_vbox = get_vbox();
    dialog_vbox->set_spacing(18);
    dialog_vbox->pack_start(vBox_, Gtk::PACK_EXPAND_WIDGET, 0);

    scrolledWindow_.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC);
    vBox_.pack_start(scrolledWindow_, Gtk::PACK_EXPAND_WIDGET, 0);

    FlickrDialogModelColumnRecord flickr_dialog_model_column_record;

    treeView_.append_column("",
        flickr_dialog_model_column_record.get_column_selected());
    treeView_.append_column("",
        flickr_dialog_model_column_record.get_column_pixbuf());

    const gint text_col_num = treeView_.append_column(
                                  "", rendererText_) - 1;
    Gtk::TreeView::Column * const text_col = treeView_.get_column(
                                                 text_col_num);
    text_col->add_attribute(
        rendererText_.property_markup(),
        flickr_dialog_model_column_record.get_column_details());

    treeView_.set_enable_search(true);
    treeView_.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_NONE);
    treeView_.set_headers_clickable(false);
    treeView_.set_headers_visible(false);
    scrolledWindow_.add(treeView_);

    vBox_.pack_start(hBox_, Gtk::PACK_SHRINK, 0);

    spinner_.set_size(Gtk::ICON_SIZE_LARGE_TOOLBAR);
    hBox_.pack_start(spinner_, Gtk::PACK_SHRINK, 0);

    hBox_.pack_start(spinnerLabel_, Gtk::PACK_EXPAND_WIDGET, 0);

    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
    set_default_response(Gtk::RESPONSE_OK);

    readFlickrEnd_.connect(
        sigc::mem_fun(*this,
                      &FlickrChooserDialog::on_read_flickr_end));

    readFlickrProgress_.connect(
        sigc::mem_fun(*this,
                      &FlickrChooserDialog::on_read_flickr_progress));

    treeView_.signal_row_activated().connect(sigc::mem_fun(
        *this,
        &FlickrChooserDialog::on_tree_view_row_activated));

    show_all_children();

    spawn_read_flickr();
}

FlickrChooserDialog::~FlickrChooserDialog() throw()
{
    observer_->set_stop(true);

    while (0 != threadPool_.get_num_threads())
    {
        while (true == Gtk::Main::events_pending())
        {
            Gtk::Main::iteration();
        }

        Glib::usleep(10);
    }
}

std::list<std::string>
FlickrChooserDialog::get_uris() const throw()
{
    FlickrDialogModelColumnRecord flickr_dialog_model_column_record;
    const Gtk::TreeModel::Children children = listStore_->children();
    Gtk::TreeModel::const_iterator iter;
    std::list<std::string> uris;

    for (iter = children.begin();
         true == iter && children.end() != iter;
         iter++)
    {
        const Gtk::TreeModel::Row row = *iter;

        if (true == row[
            flickr_dialog_model_column_record.get_column_selected()])
        {
            const flickcurl_photo * const photo = row[
                flickr_dialog_model_column_record.get_column_photo()];
            uris.push_back(photo->uri);
        }
    }

    return uris;
}

bool
FlickrChooserDialog::on_delete_event(GdkEventAny * event)
{
    hide();
    observer_->set_stop(true);
    soup_session_abort(soupSession_);
    return false;
}

void
FlickrChooserDialog::on_response(int response_id)
{
    hide();
    observer_->set_stop(true);
    soup_session_abort(soupSession_);
}

void
FlickrChooserDialog::on_file_loaded(
                         SoupMessage * soup_message,
                         const Gtk::TreeModel::iterator & iter)
                         throw()
{
    observer_->receive_event_notifiation();

    PixbufPtr pixbuf;

    if (false == SOUP_STATUS_IS_SUCCESSFUL(soup_message->status_code))
    {
        const IconThemePtr icon_theme = Gtk::IconTheme::get_default();
        pixbuf = icon_theme->load_icon(
                     "image-missing", FLICKR_THUMBNAIL_SIZE)->copy();
    }
    else
    {
        const PixbufLoaderPtr pixbuf_loader
                                  = Gdk::PixbufLoader::create();

        try
        {
            pixbuf_loader->write(
                reinterpret_cast<const guint8 *>(
                    soup_message->response_body->data),
                soup_message->response_body->length);
        }
        catch (const Glib::FileError & e)
        {
            g_warning("%s", e.what().c_str());
        }
        catch (const Gdk::PixbufError & e)
        {
            g_warning("%s", e.what().c_str());
        }

        try
        {
            pixbuf_loader->close();
        }
        catch (const Glib::FileError & e)
        {
            g_warning("%s", e.what().c_str());
        }
        catch (const Gdk::PixbufError & e)
        {
            g_warning("%s", e.what().c_str());
        }

        pixbuf = pixbuf_loader->get_pixbuf();
    }

    Gtk::TreeModel::Row row = *iter;
    FlickrDialogModelColumnRecord flickr_dialog_model_column_record;

    row[flickr_dialog_model_column_record.get_column_pixbuf()]
        = pixbuf;
}

void
FlickrChooserDialog::on_read_flickr_end() throw()
{
    hBox_.hide();
    observer_->reset();
    set_busy(false);
}

void
FlickrChooserDialog::on_read_flickr_progress() throw()
{
    if (0 == photos_ || 0 == photos_[current_])
    {
        return;
    }

    const IconThemePtr icon_theme = Gtk::IconTheme::get_default();
    const PixbufPtr icon_loading
        = icon_theme->load_icon(
              "image-loading", FLICKR_THUMBNAIL_SIZE)->copy();

    FlickrDialogModelColumnRecord flickr_dialog_model_column_record;

    const Gtk::TreeModel::iterator model_iter = listStore_->append();
    Gtk::TreeModel::Row row = *model_iter;

    row[flickr_dialog_model_column_record.get_column_selected()]
        = true;

    row[flickr_dialog_model_column_record.get_column_pixbuf()]
        = icon_loading;

    Glib::ustring tags;

    gint j;

    for (j = 0; 0 != photos_[current_]->tags
                && 0 != photos_[current_]->tags[j]; j++)
    {
        tags += (0 != photos_[current_]->tags[j]->raw)
                ? photos_[current_]->tags[j]->raw
                : photos_[current_]->tags[j]->cooked;
        tags += (0 != photos_[current_]->tags[j+1]) ? ", " : "";
    }

    tags = Glib::Markup::escape_text(tags);

    Glib::ustring title
        = photos_[current_]->fields[PHOTO_FIELD_title].string;

    const Glib::ustring details
        = Glib::ustring::compose(
              _("%1\n<small><i>%2</i></small>"),
              title, tags);

    row[flickr_dialog_model_column_record.get_column_details()]
        = details;

    row[flickr_dialog_model_column_record.get_column_photo()]
        = photos_[current_];

    SoupMessage * soup_message = soup_message_new(
                                     SOUP_METHOD_GET,
                                     uris_[current_].c_str());

    SoupSessionCBData * const data = new SoupSessionCBData();
    data->flickr_chooser_dialog = this;
    data->iter = model_iter;

    soup_session_queue_message(soupSession_, soup_message,
                               soup_session_on_file_loaded, data);

    current_++;
}

void
FlickrChooserDialog::on_tree_view_row_activated(
                         const Gtk::TreeModel::Path & path,
                         Gtk::TreeViewColumn * column) throw()
{
    Gtk::TreeModel::iterator iter = listStore_->get_iter(path);
    Gtk::TreeModel::Row row = *iter;

    FlickrDialogModelColumnRecord flickr_dialog_model_column_record;

    row[flickr_dialog_model_column_record.get_column_selected()]
    = !row[flickr_dialog_model_column_record.get_column_selected()];
}

void
FlickrChooserDialog::read_flickr() throw()
{
    if (true == observer_->get_stop())
    {
        return;
    }

    uris_.clear();

    FlickrPhotoUriBuilder flickr_photo_uri_builder('s');
    gint i;

    for (i = 0; 0 != photos_ && 0 != photos_[i]; i++)
    {
        if (0 == photos_[i]->tags || 0 == photos_[i]->tags[0])
        {
            free(photos_[i]->tags);
            photos_[i]->tags = 0;

            photos_[i]->tags = flickcurl_tags_getListPhoto(
                                   flickrContext_->get_flickr_session(),
                                   photos_[i]->id);

            gint j;

            for (j = 0, photos_[i]->tags_count = 0;
                 0 != photos_[i]->tags && 0 != photos_[i]->tags[j];
                 j++, photos_[i]->tags_count++)
            {
            }

            if (true == observer_->get_stop())
            {
                return;
            }
        }

        char * uri = flickr_photo_uri_builder(photos_[i]);

        if (0 == uri)
        {
            uris_.push_back("");
        }
        else
        {
            uris_.push_back(uri);
            free(uri);
            uri = 0;
        }

        if (true == observer_->get_stop())
        {
            return;
        }

        readFlickrProgress_.emit();
    }

    if (true == observer_->get_stop())
    {
        return;
    }

    readFlickrEnd_.emit();
}

void
FlickrChooserDialog::set_busy(bool busy) throw()
{
    WindowPtr window = get_window();

    switch (busy)
    {
        case true:
        {
            if (0 != window)
            {
                Gdk::Cursor cursor(Gdk::WATCH);
                window->set_cursor(cursor);
            }

            set_response_sensitive(Gtk::RESPONSE_OK, false);
            spinner_.start();
            break;
        }

        case false:
        {
            if (0 != window)
            {
                window->set_cursor();
            }

            set_sensitive(true);
            set_response_sensitive(Gtk::RESPONSE_OK, true);
            spinner_.stop();
            break;
        }
    }
}

void
FlickrChooserDialog::spawn_read_flickr() throw()
{
    current_ = 0;
    hBox_.show_all();
    observer_->reset();
    set_busy(true);

    threadPool_.push(
        sigc::mem_fun(*this,
                      &FlickrChooserDialog::read_flickr));
}

} // namespace Solang
