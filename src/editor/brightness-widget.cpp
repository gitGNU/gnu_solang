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
#include "brightness-operation.h"
#include "brightness-widget.h"

namespace Solang
{

BrightnessWidget::BrightnessWidget( EditorToolbar &toolbar,
                                    Editor &editor)
    :EditActionWidget( _("Brightness"), toolbar, editor ),
    brightness_(-100.0, 100.1, 0.1),
    hbFraction_(),
    engine_()
{
    //hbDir_.pack_start( lblDirection_, Gtk::PACK_SHRINK, 6);
    hbFraction_.pack_start( brightness_, Gtk::PACK_EXPAND_WIDGET, 6 );
    hbFraction_.pack_end( btApply_, Gtk::PACK_SHRINK, 0 );
    add( hbFraction_ );

    brightness_.set_increments( 0.1, 0.5 );
    brightness_.set_update_policy( Gtk::UPDATE_DELAYED );
    brightness_.set_value( 0.0 );
    connection_ = btApply_.signal_clicked()
                    .connect( sigc::mem_fun( *this,
                    &EditActionWidget::on_apply_clicked ) );

    return;
}

BrightnessWidget::~BrightnessWidget()
{
}

EditActionPtr
BrightnessWidget::get_action()
{
    double fraction = brightness_.get_value();
    FilterPtr filter( new BrightnessOperation(
                            editor_.get_engine(), fraction / 100 ) );
    BufferPtr buffer = editor_.get_current_photo()
                                ->get_edit_buffer();
    OperationPtr op( new Operation(
                            editor_.get_engine(),
                            filter, buffer ) );
    return EditActionPtr( new GeglOperation( op,
        editor_.get_application()
                ->get_engine().get_default_observer() ) );

}

}//namespace Solang
