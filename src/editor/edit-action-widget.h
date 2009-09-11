#ifndef SOLANG_EDIT_ACTION_WIDGET_H
#define SOLANG_EDIT_ACTION_WIDGET_H

#include <gtkmm.h>
#include "editor-toolbar.h"
#include "stock-button.h"
#include "types.h"

namespace Solang
{

class Editor;

class EditActionWidget
        : public Gtk::Expander
{
    public:
        EditActionWidget( const Glib::ustring &label,
                        EditorToolbar &toolBar,
                        Editor &editor );
        virtual ~EditActionWidget();

        void
        on_apply_clicked();

    protected:
        virtual EditActionPtr
        get_action() = 0;

    protected:
        StockButton btApply_;
        EditorToolbar::ActionSignal &signalApplyClicked_;
        sigc::connection connection_;
        Editor &editor_;
};

} //namespace Solang

#endif //SOLANG_EDIT_ACTION_WIDGET_H
