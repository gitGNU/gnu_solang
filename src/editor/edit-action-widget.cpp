#include "edit-action-widget.h"
#include "editor-toolbar.h"

namespace Solang
{

EditActionWidget::EditActionWidget(
                        const Glib::ustring &label,
                        EditorToolbar &toolBar,
                        Editor &editor )
    : Gtk::Expander( label ),
    btApply_( Gtk::Stock::APPLY ),
    signalApplyClicked_( toolBar.signal_apply() ),
    connection_(),
    editor_( editor )
{
//    connection_ = btApply_.signal_clicked().connect(
//                sigc::mem_fun( *this,
//                    &EditActionWidget::on_apply_clicked ) );
    btApply_.set_relief( Gtk::RELIEF_NONE );
}

EditActionWidget::~EditActionWidget()
{
    connection_.disconnect();
}

void
EditActionWidget::on_apply_clicked()
{
    signalApplyClicked_.emit( get_action() );
}


} //namespace Solang

