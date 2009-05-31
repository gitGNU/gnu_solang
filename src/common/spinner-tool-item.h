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

/*
 * Based on code by: Christian Persch
 *
 * This widget was originally written in C as a part of Epiphany:
 * lib/widgets/ephy-spinner-tool-item.h
 */

#ifndef SOLANG_SPINNER_TOOL_ITEM_H
#define SOLANG_SPINNER_TOOL_ITEM_H

#include <gtkmm.h>

#include "spinner.h"

namespace Solang
{

class SpinnerToolItem :
    public Gtk::ToolItem
{
    public:
        SpinnerToolItem() throw();

        virtual
        ~SpinnerToolItem() throw();

        void
        set_spinning(bool spinning) throw();

    protected:
        virtual void
        on_toolbar_reconfigured() throw();

        Spinner spinner_;

    private:
};

} // namespace Solang

#endif // SOLANG_SPINNER_TOOL_ITEM_H
