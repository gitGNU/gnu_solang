#include "cursor-changer.h"

namespace Solang
{
CursorChanger::State CursorChanger::state_;

CursorChanger::CursorChanger(Gtk::Widget &widget)
{
    CursorChanger::State *state
                        = &CursorChanger::state_;
    state->widget_ = &widget;
    state->change_.emit();
}

CursorChanger::~CursorChanger()
{
    CursorChanger::state_.change_.emit();
}

void
CursorChanger::set_cursor()
{
    CursorChanger::State *state = &CursorChanger::state_;
    if( !state->busy_ )
    {
        state->widget_->get_window()->set_cursor(
        Gdk::Cursor( state->widget_->get_display(),
                    Gdk::WATCH ) );
        state->widget_->set_sensitive( false );
        state->busy_ = true;
    }
    else
    {
        state->widget_->get_window()->set_cursor();
        state->widget_->set_sensitive( true );
        state->busy_ = false;
    }
}

CursorChanger::State::State()
    :widget_( 0 ),
    change_(),
    busy_( false ),
    conn_()
{
    conn_ = change_.connect(
        &CursorChanger::set_cursor );
}

CursorChanger::State::~State()
{
    conn_.disconnect();
}

} //namespace Solang

