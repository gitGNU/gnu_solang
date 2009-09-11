#ifndef SOLANG_SCALE_WIDGET_H
#define SOLANG_SCALE_WIDGET_H

#include <gtkmm.h>
#include "edit-action-widget.h"

namespace Solang
{
class EditorToolbar;

class ScaleWidget
    : public EditActionWidget
{
    public:
        ScaleWidget( EditorToolbar &toolbar,
                     Editor &editor );
        ~ScaleWidget();

    protected:

        virtual EditActionPtr
        get_action();

    private:
        Gtk::HScale fraction_;
        Gtk::HBox  hbFraction_;
        EditEnginePtr engine_;
};

}


#endif //SOLANG_SCALE_WIDGET_H
