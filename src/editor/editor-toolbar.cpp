#ifdef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H

#include <glibmm/i18n.h>

#include "edit-action-widget.h"
#include "editor-toolbar.h"

namespace Solang
{
EditorToolbar::EditorToolbar()
    :ScrolledWindow(),
    filters_(),
    histogramExpander_( _("Histogram") ),
    histoGram_(),
    widgets_()
{
    set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    Gtk::ScrolledWindow::add(filters_);
    filters_.pack_start( histogramExpander_, Gtk::PACK_SHRINK, 6 );
    histogramExpander_.add( histoGram_ );
    histogramExpander_.set_expanded( true );
}

EditorToolbar::~EditorToolbar()
{
}

void
EditorToolbar::add(
            const Glib::RefPtr<EditActionWidget> &widget )
{
    widgets_.push_back( widget );
    return;
}

void
EditorToolbar::setup() throw()
{
    for( WidgetList::iterator widget = widgets_.begin();
            widget != widgets_.end(); widget++ )
    {
        Glib::RefPtr<EditActionWidget> editWidget = *widget;
        filters_.pack_start(
                    *(editWidget.operator ->()), Gtk::PACK_SHRINK, 6 );
    }
}

} //namespace Solang


