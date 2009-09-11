#ifndef SOLANG_EDITOR_TOOLBAR_H
#define SOLANG_EDITOR_TOOLBAR_H

#include <gtkmm.h>
#include "histogram-viewer.h"

namespace Solang
{

class EditActionWidget;

class EditorToolbar
    : public Gtk::ScrolledWindow
{
    public:
        typedef sigc::signal<void, EditActionPtr> ActionSignal;
        typedef std::vector<Glib::RefPtr<EditActionWidget> > WidgetList;
    public:

        EditorToolbar();

        ~EditorToolbar();

        inline HistogramViewer &
        get_histogram_viewer();

        inline ActionSignal &
        signal_apply();

        void
        add( const Glib::RefPtr<EditActionWidget> &widget );

        void
        setup() throw();

    private:
        Gtk::VBox  filters_;
        Gtk::Expander histogramExpander_;
        HistogramViewer histoGram_;
        ActionSignal signalApply_;
        WidgetList widgets_;
};

inline HistogramViewer &
EditorToolbar::get_histogram_viewer()
{
    return histoGram_;
}

inline EditorToolbar::ActionSignal &
EditorToolbar::signal_apply()
{
    return signalApply_;
}


} //namespace Solang
#endif //SOLANG_EDITOR_TOOLBAR_H
