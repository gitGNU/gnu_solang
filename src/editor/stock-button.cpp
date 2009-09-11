#ifdef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H

#include "stock-button.h"

namespace Solang
{

StockButton::StockButton( const Gtk::StockID &stockID )
	:Gtk::Button(),
	image_( stockID, Gtk::ICON_SIZE_BUTTON )
{
	add( image_ );
}

StockButton::~StockButton()
{
}

} //namespace Solang
