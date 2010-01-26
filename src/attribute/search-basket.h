/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009, 2010 Debarshi Ray <rishi@gnu.org>
 *
 * Solang is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solang is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_SEARCH_BASKET_H
#define SOLANG_SEARCH_BASKET_H

#include <string>

#include <gdkmm.h>
#include <gtkmm.h>

#include "types.h"

namespace Solang
{

class SearchBasket :
    public Gtk::TreeView
{
    public:
        SearchBasket(const TreeModelPtr & model) throw();

        virtual
        ~SearchBasket() throw();

        const UIManagerPtr &
        get_ui_manager() throw();

    protected:
        virtual bool
        on_button_press_event(GdkEventButton * event) throw();

        UIManagerPtr uiManager_;

        Gtk::UIManager::ui_merge_id uiID_;

        Gtk::Menu * menu_;

    private:
        static const std::string uiFile_;
};

} // namespace Solang

#endif // SOLANG_SEARCH_BASKET_H
