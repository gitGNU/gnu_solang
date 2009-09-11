#ifndef SOLANG_BRIGHTNESS_WIDGET_H
#define SOLANG_BRIGHTNESS_WIDGET_H

#include <gtkmm.h>
#include "edit-action-widget.h"

namespace Solang
{
class EditorToolbar;

class BrightnessWidget
    : public EditActionWidget
{
    public:
        BrightnessWidget( EditorToolbar &toolbar,
                     Editor &editor );
        ~BrightnessWidget();

    protected:

        virtual EditActionPtr
        get_action();

    private:
        Gtk::HScale brightness_;
        Gtk::HBox  hbFraction_;
        EditEnginePtr engine_;
};

}


#endif //SOLANG_BRIGHTNESS_WIDGET_H
