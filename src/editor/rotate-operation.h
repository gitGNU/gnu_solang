/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2010 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_ROTATE_OPERATION_H
#define SOLANG_ROTATE_OPERATION_H

#include <string>

#include <glibmm.h>

#include "operation.h"

namespace Solang
{

class RotateOperation :
    public Operation
{
    public:
        RotateOperation(gdouble origin_x = 0.0,
                        gdouble origin_y = 0.0,
                        const std::string & filter = "linear",
                        bool hard_edges = false,
                        gint lanczos_width = 3,
                        gdouble degrees = 0.0) throw();

        virtual
        ~RotateOperation() throw();

        virtual Glib::ustring
        get_description() const throw();

        virtual Gtk::Widget *
        get_widget() throw() = 0;

        virtual SignalReady &
        signal_ready() throw();

    protected:
        virtual NodePtr
        get_node(const BufferPtr & buffer, const NodePtr & root) const
                 throw();

        void
        set_hard_edges(bool hard_edges) throw();

        void
        set_degrees(gdouble degrees) throw();

        void
        set_origin_x(gdouble origin_x) throw();

        void
        set_origin_y(gdouble origin_y) throw();

        void
        set_lanczos_width(gdouble lanczos_width) throw();

        void
        set_filter(const std::string & filter) throw();

    private:
        bool hardEdges_;

        gdouble degrees_;

        gdouble originX_;

        gdouble originY_;

        gint lanczosWidth_;

        std::string filter_;

        SignalReady signalReady_;
};

} // namespace Solang

#endif // SOLANG_ROTATE_OPERATION_H
