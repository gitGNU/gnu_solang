#ifndef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glibmm/i18n.h>

#include "editor-toolbar.h"
#include "rotate-widget.h"
#include "rotate.h"

namespace Solang
{

RotateWidget::RotateWidget( EditorToolbar &toolbar,
                            Editor &editor )
    :EditActionWidget( _("Rotate"), toolbar, editor ),
    columns_(),
    dirStore_( Gtk::ListStore::create( columns_ )),
    cmbDirection_(),
    hbDir_()
{
    //hbDir_.pack_start( lblDirection_, Gtk::PACK_SHRINK, 6);
    hbDir_.pack_start( cmbDirection_, Gtk::PACK_EXPAND_WIDGET, 6 );
    //hbDir_.pack_start( btApply_ );
    add( hbDir_ );

    cmbDirection_.set_model( dirStore_ );
    cmbDirection_.pack_start( columns_.name_ );
    {
        //Directions
        Gtk::TreeModel::Row row = *(dirStore_->append());
        row[ columns_.direction_ ] = Rotate::ROTATE_CLOCKWISE;
        row[ columns_.name_ ] = _("Clockwise");
    }
    {
        //Directions
        Gtk::TreeModel::Row row = *(dirStore_->append());
        row[ columns_.direction_ ] = Rotate::ROTATE_COUNTERCLOCKWISE;
        row[ columns_.name_ ] = _("Counter-Clockwise");
    }
    //cmbDirection_.set_active( 0 );
    connection_ = cmbDirection_.signal_changed().connect(
                    sigc::mem_fun( *this,
                    &EditActionWidget::on_apply_clicked ) );

    return;
}

RotateWidget::~RotateWidget()
{
}

EditActionPtr
RotateWidget::get_action()
{
    Gtk::TreeModel::const_iterator dirI
                                = cmbDirection_.get_active();
    Gtk::TreeModel::Row dirR = (*dirI);
    Rotate::Direction direction = dirR[ columns_.direction_ ];
    return EditActionPtr( new Rotate( direction ) );
}

}//namespace Solang
