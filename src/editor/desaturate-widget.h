#ifndef SOLANG_DESATURATE_WIDGET_H
#define SOLANG_DESATURATE_WIDGET_H

#include <gtkmm.h>
#include "edit-action-widget.h"

namespace Solang
{
class EditorToolbar;

class DesaturateWidget
    : public EditActionWidget
{
    public:
        DesaturateWidget( EditorToolbar &toolbar,
                     Editor &editor );
        ~DesaturateWidget();

    protected:

        virtual EditActionPtr
        get_action();

    private:
        Gtk::HScale fraction_;
        Gtk::HBox  hbFraction_;
        EditEnginePtr engine_;
};

}


#endif //SOLANG_DESATURATE_WIDGET_H
