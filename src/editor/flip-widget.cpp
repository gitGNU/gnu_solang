#ifndef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glibmm/i18n.h>

#include "editor-toolbar.h"
#include "flip-widget.h"
#include "flip.h"

namespace Solang
{

FlipWidget::FlipWidget( EditorToolbar &toolbar,
                            Editor &editor )
    :EditActionWidget( _("Flip"), toolbar, editor ),
    columns_(),
    dirStore_( Gtk::ListStore::create( columns_ )),
    cmbDirection_(),
    hbDir_()
{
    //hbDir_.pack_start( lblDirection_, Gtk::PACK_SHRINK, 6);
    hbDir_.pack_start( cmbDirection_, Gtk::PACK_EXPAND_WIDGET, 6 );
//    hbDir_.pack_start( btApply_ );
    add( hbDir_ );

    cmbDirection_.set_model( dirStore_ );
    cmbDirection_.pack_start( columns_.name_ );
    {
        //Directions
        Gtk::TreeModel::Row row = *(dirStore_->append());
        row[ columns_.direction_ ] = true;
        row[ columns_.name_ ] = _("Horizontal");
    }
    {
        //Directions
        Gtk::TreeModel::Row row = *(dirStore_->append());
        row[ columns_.direction_ ] = false;
        row[ columns_.name_ ] = _("Vertical");
    }
    //cmbDirection_.set_active( 0 );
    connection_ = cmbDirection_.signal_changed().connect(
                    sigc::mem_fun( *this,
                    &EditActionWidget::on_apply_clicked ) );

    return;
}

FlipWidget::~FlipWidget()
{
}

EditActionPtr
FlipWidget::get_action()
{
    Gtk::TreeModel::const_iterator dirI
                                = cmbDirection_.get_active();
    Gtk::TreeModel::Row dirR = (*dirI);
    bool direction = dirR[ columns_.direction_ ];
    return EditActionPtr( new Flip( direction ) );
}

}//namespace Solang
