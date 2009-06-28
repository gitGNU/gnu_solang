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

#include <gdkmm.h>
#include <glibmm/i18n.h>
#include <gtk/gtk.h>

#include "flickr-chooser-button.h"
#include "flickr-chooser-dialog.h"
#include "flickr-context.h"
#include "flickr-enums.h"
#include "flickr-photo-uri-builder.h"

namespace Solang
{

enum
{
    COLUMN_PIXBUF = 0,
    COLUMN_NAME,
    COLUMN_ROW_TYPE,
    COLUMN_PHOTOS,
    COLUMN_COUNT
};

class FlickrButtonModelColumnRecord :
    public Gtk::TreeModelColumnRecord
{
    public:
        FlickrButtonModelColumnRecord() throw();

        virtual
        ~FlickrButtonModelColumnRecord() throw();

        const Gtk::TreeModelColumn<PixbufPtr> &
        get_column_pixbuf() const throw();

        gint
        get_column_pixbuf_num() const throw();

        const Gtk::TreeModelColumn<Glib::ustring> &
        get_column_name() const throw();

        gint
        get_column_name_num() const throw();

        const Gtk::TreeModelColumn<FlickrType> &
        get_column_row_type() const throw();

        gint
        get_column_row_type_num() const throw();

        const Gtk::TreeModelColumn<flickcurl_photo **> &
        get_column_photos() const throw();

        gint
        get_column_photos_num() const throw();

    protected:
        Gtk::TreeModelColumn<PixbufPtr> columnPixbuf_;

        Gtk::TreeModelColumn<Glib::ustring> columnName_;

        Gtk::TreeModelColumn<FlickrType> columnRowType_;

        Gtk::TreeModelColumn<flickcurl_photo **> columnPhotos_;

    private:
};

FlickrButtonModelColumnRecord::FlickrButtonModelColumnRecord()
                                   throw() :
    Gtk::TreeModelColumnRecord(),
    columnPixbuf_(),
    columnName_(),
    columnRowType_(),
    columnPhotos_()
{
    add(columnPixbuf_);
    add(columnName_);
    add(columnRowType_);
    add(columnPhotos_);
}

FlickrButtonModelColumnRecord::~FlickrButtonModelColumnRecord()
                                   throw()
{
}

const Gtk::TreeModelColumn<PixbufPtr> &
FlickrButtonModelColumnRecord::get_column_pixbuf() const throw()
{
    return columnPixbuf_;
}

gint
FlickrButtonModelColumnRecord::get_column_pixbuf_num() const throw()
{
    return COLUMN_PIXBUF;
}

const Gtk::TreeModelColumn<Glib::ustring> &
FlickrButtonModelColumnRecord::get_column_name() const throw()
{
    return columnName_;
}

gint
FlickrButtonModelColumnRecord::get_column_name_num() const throw()
{
    return COLUMN_NAME;
}

const Gtk::TreeModelColumn<FlickrType> &
FlickrButtonModelColumnRecord::get_column_row_type() const throw()
{
    return columnRowType_;
}

gint
FlickrButtonModelColumnRecord::get_column_row_type_num() const
                                   throw()
{
    return COLUMN_ROW_TYPE;
}

const Gtk::TreeModelColumn<flickcurl_photo **> &
FlickrButtonModelColumnRecord::get_column_photos() const throw()
{
    return columnPhotos_;
}

gint
FlickrButtonModelColumnRecord::get_column_photos_num() const throw()
{
    return COLUMN_PHOTOS;
}

FlickrChooserButton::FlickrChooserButton(
                         const FlickrContextPtr & flickr_context)
                         throw() :
    Gtk::HBox(false, 12),
    flickrContext_(flickr_context),
    favoritePhotos_(0),
    photosets_(0),
    photosetPhotos_(),
    otherPhotos_(0),
    threadPool_(1, false),
    listStore_(Gtk::ListStore::create(
                   FlickrButtonModelColumnRecord())),
    comboBox_(listStore_),
    rendererPixbuf_(),
    rendererText_(),
    selectButton_(),
    selectImage_(Gtk::Stock::SELECT_COLOR,
                 Gtk::ICON_SIZE_SMALL_TOOLBAR),
    readFlickrEnd_(),
    uris_()
{
    GtkCellLayout * const combo_box
                              = GTK_CELL_LAYOUT(comboBox_.gobj());
    GtkCellRenderer * const renderer_pixbuf
        = GTK_CELL_RENDERER(rendererPixbuf_.gobj());
    GtkCellRenderer * const renderer_text
        = GTK_CELL_RENDERER(rendererText_.gobj());

    gtk_cell_layout_pack_start(combo_box, renderer_pixbuf, FALSE);
    gtk_cell_layout_set_attributes(
        combo_box, renderer_pixbuf, "pixbuf",
        FlickrButtonModelColumnRecord().get_column_pixbuf_num(),
        NULL);

    gtk_cell_layout_pack_start(combo_box, renderer_text, TRUE);
    gtk_cell_layout_set_attributes(
        combo_box, renderer_text, "text",
        FlickrButtonModelColumnRecord().get_column_name_num(),
        NULL);

    comboBox_.set_row_separator_func(
        sigc::mem_fun(*this,
                      &FlickrChooserButton::on_row_separator));
    pack_start(comboBox_, Gtk::PACK_EXPAND_WIDGET, 0);

    selectButton_.add(selectImage_);
    selectButton_.set_tooltip_text(_("Select photos"));
    pack_start(selectButton_, Gtk::PACK_SHRINK, 0);

    comboBox_.signal_changed().connect(
        sigc::mem_fun(*this,
                      &FlickrChooserButton::on_combo_box_changed));

    selectButton_.signal_clicked().connect(
        sigc::mem_fun(*this,
                      &FlickrChooserButton::on_select_button_clicked));

    readFlickrEnd_.connect(
        sigc::mem_fun(*this,
                      &FlickrChooserButton::on_read_flickr_end));

    show_all_children();

    spawn_read_flickr();
}

FlickrChooserButton::~FlickrChooserButton() throw()
{
    while (0 != threadPool_.get_num_threads())
    {
        while (true == Gtk::Main::events_pending())
        {
            Gtk::Main::iteration();
        }

        Glib::usleep(10);
    }
}

void
FlickrChooserButton::add_flickr_favorites() throw()
{
    const Gtk::TreeModel::iterator iter = listStore_->append();
    Gtk::TreeModel::Row row = *iter;

    FlickrButtonModelColumnRecord flickr_button_model_column_record;

    const IconThemePtr icon_theme = Gtk::IconTheme::get_default();
    const PixbufPtr icon_favorites
        = icon_theme->load_icon(
              "emblem-favorite", Gtk::ICON_SIZE_MENU)->copy();

    row[flickr_button_model_column_record.get_column_pixbuf()]
        = icon_favorites;

    row[flickr_button_model_column_record.get_column_name()]
        = _("Favorites");

    row[flickr_button_model_column_record.get_column_row_type()]
        = FLICKR_FAVORITES;

    row[flickr_button_model_column_record.get_column_photos()]
        = favoritePhotos_;
}

void
FlickrChooserButton::add_flickr_other() throw()
{
    const Gtk::TreeModel::iterator iter = listStore_->append();
    Gtk::TreeModel::Row row = *iter;

    FlickrButtonModelColumnRecord flickr_button_model_column_record;

    row[flickr_button_model_column_record.get_column_pixbuf()]
        = PixbufPtr(0);

    row[flickr_button_model_column_record.get_column_name()]
        = _("Other...");

    row[flickr_button_model_column_record.get_column_row_type()]
        = FLICKR_OTHER;

    row[flickr_button_model_column_record.get_column_photos()]
        = otherPhotos_;
}

void
FlickrChooserButton::add_flickr_photosets() throw()
{
    const IconThemePtr icon_theme = Gtk::IconTheme::get_default();
    const PixbufPtr icon_photoset
        = icon_theme->load_icon(
              "emblem-photos", Gtk::ICON_SIZE_MENU)->copy();

    FlickrButtonModelColumnRecord flickr_button_model_column_record;

    gint i;
    std::list<flickcurl_photo **>::iterator list_iter;

    for (i = 0, list_iter = photosetPhotos_.begin();
         0 != photosets_[i] && photosetPhotos_.end() != list_iter;
         i++, list_iter++)
    {
        const Gtk::TreeModel::iterator model_iter
                                           = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;

        row[flickr_button_model_column_record.get_column_pixbuf()]
            = icon_photoset;

        row[flickr_button_model_column_record.get_column_name()]
            = photosets_[i]->title;

        row[flickr_button_model_column_record.get_column_row_type()]
            = FLICKR_PHOTOSET;

        row[flickr_button_model_column_record.get_column_photos()]
            = *list_iter;

        while (true == Gtk::Main::events_pending())
        {
            Gtk::Main::iteration();
        }
    }
}

void
FlickrChooserButton::add_separator() throw()
{
    const Gtk::TreeModel::iterator iter = listStore_->append();
    Gtk::TreeModel::Row row = *iter;

    FlickrButtonModelColumnRecord flickr_button_model_column_record;

    row[flickr_button_model_column_record.get_column_pixbuf()]
        = PixbufPtr(0);

    row[flickr_button_model_column_record.get_column_row_type()]
        = FLICKR_SEPARATOR;

    row[flickr_button_model_column_record.get_column_photos()] = 0;
}

const std::list<std::string> &
FlickrChooserButton::get_uris() throw()
{
    if (true == uris_.empty())
    {
        const Gtk::TreeModel::const_iterator iter
            = comboBox_.get_active();

        if (false == iter)
        {
            return uris_;
        }

        const Gtk::TreeModel::Row row = *iter;

        FlickrButtonModelColumnRecord
            flickr_button_model_column_record;

        flickcurl_photo ** photos = row[
            flickr_button_model_column_record.get_column_photos()];

        gint i;

        for (i = 0; 0 != photos && 0 != photos[i]; i++)
        {
            const char * const uri = photos[i]->uri;

            if (0 == uri)
            {
                continue;
            }

            uris_.push_back(uri);
        }
    }

    return uris_;
}

void
FlickrChooserButton::on_flickr_chooser_dialog_response(
    gint response_id,
    FlickrChooserDialogPtr & flickr_chooser_dialog) throw()
{
    switch (response_id)
    {
        case Gtk::RESPONSE_CANCEL:
        case Gtk::RESPONSE_DELETE_EVENT:
        {
            break;
        }

        case Gtk::RESPONSE_OK:
        {
            uris_ = flickr_chooser_dialog->get_uris();
            break;
        }

        default:
        {
            break;
        }
    }

    Glib::signal_timeout().connect_seconds(sigc::bind(
        sigc::mem_fun(*this,
            &FlickrChooserButton::on_flickr_chooser_dialog_destroy),
        flickr_chooser_dialog),
        30, Glib::PRIORITY_DEFAULT);

    flickr_chooser_dialog.reset();
}

void
FlickrChooserButton::on_combo_box_changed() throw()
{
    uris_.clear();
}

bool
FlickrChooserButton::on_flickr_chooser_dialog_destroy(
    FlickrChooserDialogPtr & flickr_chooser_dialog) throw()
{
    flickr_chooser_dialog.reset();
    return false;
}

void
FlickrChooserButton::on_read_flickr_end() throw()
{
    add_flickr_favorites();
    add_separator();
    add_flickr_photosets();
    add_separator();
    add_flickr_other();

    comboBox_.set_active(0);

    set_busy(false);
}

bool
FlickrChooserButton::on_row_separator(
                         const TreeModelPtr & model,
                         const Gtk::TreeModel::iterator & iter)
                         throw()
{
    const Gtk::TreeModel::Row row = *iter;
    FlickrButtonModelColumnRecord flickr_button_model_column_record;

    const FlickrType row_type = row[
        flickr_button_model_column_record.get_column_row_type()];

    return (FLICKR_SEPARATOR == row_type);
}

void
FlickrChooserButton::on_select_button_clicked() throw()
{
    const Gtk::TreeModel::const_iterator iter
        = comboBox_.get_active();
    const Gtk::TreeModel::Row row = *iter;

    FlickrButtonModelColumnRecord flickr_button_model_column_record;

    FlickrChooserDialogPtr flickr_chooser_dialog(new
        FlickrChooserDialog(flickrContext_,
        row[flickr_button_model_column_record.get_column_photos()]));

    Gtk::Window * const toplevel
        = dynamic_cast<Gtk::Window *>(get_toplevel());

    if (0 != toplevel)
    {
        flickr_chooser_dialog->set_transient_for(*toplevel);
    }

    flickr_chooser_dialog->signal_response().connect(sigc::bind(
        sigc::mem_fun(
            *this,
            &FlickrChooserButton::on_flickr_chooser_dialog_response),
        flickr_chooser_dialog));

    flickr_chooser_dialog->show_all();
}

void
FlickrChooserButton::read_flickr() throw()
{
    const std::string nsid
        = flickcurl_people_findByUsername(
              flickrContext_->get_flickr_session(),
              flickrContext_->get_user_name().c_str());

    read_flickr_favorites(nsid);
    read_flickr_photosets(nsid);
    read_flickr_other(nsid);

    readFlickrEnd_.emit();
}

void
FlickrChooserButton::read_flickr_favorites(const std::string & nsid)
                                           throw()
{
    if (0 != favoritePhotos_)
    {
        flickcurl_free_photos(favoritePhotos_);
    }

    favoritePhotos_ = flickcurl_favorites_getList(
                          flickrContext_->get_flickr_session(),
                          nsid.c_str(), "license", 100, 1);

    FlickrPhotoUriBuilder flickr_photo_uri_builder('\0');
    gint i;

    for (i = 0; 0 != favoritePhotos_[i]; i++)
    {
        favoritePhotos_[i]->uri = flickr_photo_uri_builder(
                                      favoritePhotos_[i]);
    }
}

void
FlickrChooserButton::read_flickr_other(const std::string & nsid)
                                       throw()
{
    if (0 != otherPhotos_)
    {
        flickcurl_free_photos(otherPhotos_);
    }

    otherPhotos_ = flickcurl_people_getPublicPhotos(
                       flickrContext_->get_flickr_session(),
                       nsid.c_str(), 0, 100, 1);

    FlickrPhotoUriBuilder flickr_photo_uri_builder('\0');
    gint i;

    for (i = 0; 0 != otherPhotos_[i]; i++)
    {
        otherPhotos_[i]->uri = flickr_photo_uri_builder(
                                   otherPhotos_[i]);
    }
}

void
FlickrChooserButton::read_flickr_photosets(const std::string & nsid)
                                           throw()
{
    photosets_ = flickcurl_photosets_getList(
                     flickrContext_->get_flickr_session(),
                     nsid.c_str());

    std::list<flickcurl_photo **>::iterator iter;

    for (iter = photosetPhotos_.begin();
         photosetPhotos_.end() != iter;
         iter++)
    {
        if (0 != *iter)
        {
            flickcurl_free_photos(*iter);
            *iter = 0;
        }
    }

    photosetPhotos_.clear();

    FlickrPhotoUriBuilder flickr_photo_uri_builder('\0');
    gint i;

    for (i = 0; 0 != photosets_[i]; i++)
    {
        flickcurl_photo ** const photos
            = flickcurl_photosets_getPhotos(
                  flickrContext_->get_flickr_session(),
                  photosets_[i]->id, 0, -1, 100, 1);

        gint j;

        for (j = 0; 0 != photos[j]; j++)
        {
            photos[j]->uri = flickr_photo_uri_builder(photos[j]);
        }

        photosetPhotos_.push_back(photos);
    }
}

void
FlickrChooserButton::set_busy(bool busy) throw()
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

            selectButton_.set_sensitive(false);
            break;
        }

        case false:
        {
            if (0 != window)
            {
                window->set_cursor();
            }

            selectButton_.set_sensitive(true);
            break;
        }
    }
}

void
FlickrChooserButton::spawn_read_flickr() throw()
{
    set_busy(true);

    threadPool_.push(
        sigc::mem_fun(*this,
                      &FlickrChooserButton::read_flickr));
}

} // namespace Solang
