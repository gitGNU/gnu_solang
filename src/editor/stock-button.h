#ifndef SOLANG_STOCK_BUTTON_H
#define SOLANG_STOCK_BUTTON_H

#include <gtkmm.h>

namespace Solang
{

class StockButton
	: public Gtk::Button
{
	public:
		StockButton(const Gtk::StockID &stockID );
		~StockButton();

	private:
		Gtk::Image image_;
};

} //namespace Solang

#endif //SOLANG_STOCK_BUTTON_H
