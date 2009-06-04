/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
 * Copyright (C) 2009 Santanu Sinha <santanu.sinha@gmail.com>
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

#ifndef SOLANG_PAGINATION_BAR_H
#define SOLANG_PAGINATION_BAR_H

#include <glibmm.h>
#include <gtkmm.h>

namespace Solang
{

class PaginationBar :
    public Gtk::HBox
{
    public:
        PaginationBar() throw();

        ~PaginationBar() throw();

        guint
        get_step() const throw();

        guint
        get_lower_limit() const throw();

        guint
        get_upper_limit() const throw();

        sigc::signal<void> &
        limits_changed() throw();

        void
        scroll_to_position(guint position) throw();

        void
        set_total(guint total) throw();

    protected:
        void
        on_action_go_previous() throw();

        void
        on_action_go_next() throw();

        void
        on_limits_changed() throw();

        void
        on_spin_button_activate() throw();

        Gtk::HBox firstHBox_;

        Gtk::Button previousButton_;

        Gtk::Image previousImage_;

        Gtk::Label shownItemsLabel_;

        Gtk::Button nextButton_;

        Gtk::Image nextImage_;

        Gtk::HBox secondHBox_;

        Gtk::Label pageSizeLabel_;

        Gtk::SpinButton pageSizeSpinButton_;

        guint lowerLimit_;

        guint upperLimit_;

        guint total_;

        sigc::signal<void> limitsChanged_;

    private:
};

} // namespace Solang

#endif // SOLANG_PAGINATION_BAR_H
