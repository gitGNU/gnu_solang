/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_SCALE_ACTION_H
#define SOLANG_SCALE_ACTION_H

#include <glibmm.h>
#include <gtkmm.h>
#include <sigc++/sigc++.h>

namespace Solang
{

class ScaleAction :
    public Gtk::Action
{
    public:
        virtual
        ~ScaleAction() throw();

        static Glib::RefPtr<ScaleAction>
        create(const Glib::ustring & name,
               const Glib::ustring & tooltip,
               const Gtk::Adjustment & adjustment) throw();

        double
        get_value() const throw();

        void
        set_value(double value) throw();

        sigc::signal<void, Gtk::ScrollType, double> &
        signal_change_value() throw();

        sigc::signal<void> &
        signal_value_changed() throw();

    protected:
        ScaleAction(const Glib::ustring & name,
                    const Glib::ustring & tooltip,
                    const Gtk::Adjustment & adjustment) throw();

        virtual void
        connect_proxy_vfunc(Gtk::Widget * proxy);

        virtual Gtk::Widget *
        create_tool_item_vfunc();

        bool
        on_delegate_change_value(Gtk::ScrollType scroll,
                                 double new_value) throw();

        void
        on_delegate_value_changed() throw();

        Gtk::Adjustment adjustment_;

        sigc::signal<void, Gtk::ScrollType, double> changeValue_;

        sigc::signal<void> valueChanged_;

    private:
};

} // namespace Solang

#endif // SOLANG_SCALE_ACTION_H
