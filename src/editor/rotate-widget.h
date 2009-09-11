#ifndef SOLANG_ROTATE_WIDGET_H
#define SOLANG_ROTATE_WIDGET_H

#include <gtkmm.h>
#include "stock-button.h"
#include "edit-action-widget.h"
#include "rotate.h"

namespace Solang
{
class EditorToolbar;

class RotateWidget
    : public EditActionWidget
{
    private:
        class DirectionSelectionColumns
                : public Gtk::TreeModel::ColumnRecord
        {
            public:
                DirectionSelectionColumns()
                {
                    add( direction_ );
                    add( name_ );
                }
                Gtk::TreeModelColumn<Rotate::Direction> direction_;
                Gtk::TreeModelColumn<Glib::ustring> name_;
        };
    public:
        RotateWidget( EditorToolbar &toolbar,
                        Editor &editor );
        ~RotateWidget();

    protected:

        virtual EditActionPtr
        get_action();

    private:
        DirectionSelectionColumns columns_;
        Glib::RefPtr<Gtk::ListStore> dirStore_;
        Gtk::ComboBox cmbDirection_;
        Gtk::HBox  hbDir_;
};

}


#endif //SOLANG_ROTATE_WIDGET_H
