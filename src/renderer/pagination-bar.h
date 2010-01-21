/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009, 2010 Debarshi Ray <rishi@gnu.org>
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

#include "types.h"

namespace Solang
{

class PaginationBar
{
    public:
        PaginationBar() throw();

        ~PaginationBar() throw();

        guint
        get_step() const throw();

        guint
        get_total() const throw();

        guint
        get_lower_limit() const throw();

        guint
        get_upper_limit() const throw();

        void
        go_previous() throw();

        void
        go_next() throw();

        void
        go_first() throw();

        void
        go_last() throw();

        sigc::signal<void> &
        limits_changed() throw();

        void
        scroll_to_position(guint position) throw();

        void
        set_step(guint step) throw();

        void
        set_total(guint total) throw();

    protected:
        guint step_;

        guint total_;

        guint lowerLimit_;

        guint upperLimit_;

        sigc::signal<void> limitsChanged_;

    private:
};

} // namespace Solang

#endif // SOLANG_PAGINATION_BAR_H
