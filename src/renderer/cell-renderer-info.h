/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
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

#ifndef SOLANG_CELL_RENDERER_INFO_H
#define SOLANG_CELL_RENDERER_INFO_H

#include <gdkmm.h>
#include <glibmm.h>
#include <gtkmm.h>

namespace Solang
{

class CellRendererInfo :
    public Gtk::CellRendererText
{
    public:
        CellRendererInfo() throw();

        virtual
        ~CellRendererInfo() throw();

    protected:
        virtual void
        render_vfunc(const Glib::RefPtr<Gdk::Drawable> & window,
                     Gtk::Widget & widget,
                     const Gdk::Rectangle & background_area,
                     const Gdk::Rectangle & cell_area,
                     const Gdk::Rectangle & expose_area,
                     Gtk::CellRendererState flags);

    private:
};

} // namespace Solang

#endif // SOLANG_CELL_RENDERER_INFO_H
