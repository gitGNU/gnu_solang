#ifndef SOLANG_CONTRAST_WIDGET_H
#define SOLANG_CONTRAST_WIDGET_H

#include <gtkmm.h>
#include "edit-action-widget.h"

namespace Solang
{
class EditorToolbar;

class ContrastWidget
    : public EditActionWidget
{
    public:
        ContrastWidget( EditorToolbar &toolbar,
                     Editor &editor );
        ~ContrastWidget();

    protected:

        virtual EditActionPtr
        get_action();

    private:
        Gtk::HScale contrast_;
        Gtk::HBox  hbFraction_;
        EditEnginePtr engine_;
};

}


#endif //SOLANG_CONTRAST_WIDGET_H
