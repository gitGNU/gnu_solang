#ifndef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glibmm/i18n.h>

#include "application.h"
#include "editable-photo.h"
#include "editor-toolbar.h"
#include "editor.h"
#include "gegl-operation.h"
#include "operation.h"
#include "scale.h"
#include "scale-widget.h"

namespace Solang
{

ScaleWidget::ScaleWidget( EditorToolbar &toolbar,
                                    Editor &editor)
    :EditActionWidget( _("Scale"), toolbar, editor ),
    fraction_(0, 201.0, 1.0),
    hbFraction_(),
    engine_()
{
    //hbDir_.pack_start( lblDirection_, Gtk::PACK_SHRINK, 6);
    hbFraction_.pack_start( fraction_, Gtk::PACK_EXPAND_WIDGET, 6 );
    hbFraction_.pack_end( btApply_, Gtk::PACK_SHRINK, 0 );
    add( hbFraction_ );

    fraction_.set_increments( 1.0, 10.0 );
    fraction_.set_update_policy( Gtk::UPDATE_CONTINUOUS );
    fraction_.set_value( 100.0 );
    connection_ = btApply_.signal_clicked()
                    .connect( sigc::mem_fun( *this,
                    &EditActionWidget::on_apply_clicked ) );

    return;
}

ScaleWidget::~ScaleWidget()
{
}

EditActionPtr
ScaleWidget::get_action()
{
    double fraction = fraction_.get_value();
    return EditActionPtr( new Scale( fraction / 100.0 ) );
}

}//namespace Solang
