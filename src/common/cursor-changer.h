#ifndef SOLANG_CURSOR_CHANGER_H
#define SOLANG_CURSOR_CHANGER_H

#include <gtkmm.h>

namespace Solang
{

class CursorChanger
{
    public:
        CursorChanger(Gtk::Widget &widget);
        ~CursorChanger();

    private:

        static void
        set_cursor();

        struct State
        {
            Gtk::Widget *widget_;
            Glib::Dispatcher change_;
            bool busy_;
            sigc::connection conn_;

            State();
            ~State();
        };

        static State state_;
        friend struct CursorChanger::State;

};

}

#endif //SOLANG_CURSOR_CHANGER_H

