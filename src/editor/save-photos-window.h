#ifndef SOLANG_SAVE_PHOTOS_WINDOW_H
#define SOLANG_SAVE_PHOTOS_WINDOW_H

#include <glibmm.h>
#include <gtkmm.h>

#include "types.h"

namespace Solang
{

class SavePhotosWindow :
        public Gtk::Dialog
{
        class PhotoSelectColumns : public Gtk::TreeModel::ColumnRecord
        {
            public:
                PhotoSelectColumns()
                {
                    add(select_);
                    add(preview_);
                    add(name_);
                    add(photo_);
                }
                Gtk::TreeModelColumn<bool> select_;
                Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > preview_;
                Gtk::TreeModelColumn<Glib::ustring> name_;
                Gtk::TreeModelColumn<EditablePhotoPtr> photo_;
        };

    public:
        SavePhotosWindow(
                Engine &engine,
                const EditablePhotoList &photos );
        ~SavePhotosWindow();

    private:
        void
        on_row_activated(
                const Gtk::TreeModel::Path& path,
                Gtk::TreeViewColumn* column);

    private:
        Engine &engine_;
        Gtk::VBox vBox_;
        Gtk::Label lblPhotos_;
        Gtk::TreeView photosView_;
        PhotoSelectColumns modifiedPhotos_;
        Glib::RefPtr<Gtk::ListStore> photosModel_;
};


} //namespace Solang

#endif //SOLANG_SAVE_PHOTOS_WINDOW_H
