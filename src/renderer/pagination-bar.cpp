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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cmath>

#include <sigc++/sigc++.h>

#include "pagination-bar.h"

namespace Solang
{

PaginationBar::PaginationBar() throw() :
//    pageSizeLabel_(_("Photos per page")),
//    pageSizeSpinButton_(1.0, 0),
    step_(50),
    total_(0),
    lowerLimit_(0),
    upperLimit_(step_)
{
//    pageSizeSpinButton_.set_increments(1.0, 10.0);
//    pageSizeSpinButton_.set_numeric(true);
//    pageSizeSpinButton_.set_range(1.0, 500.0);
//    pageSizeSpinButton_.set_snap_to_ticks(false);
//    pageSizeSpinButton_.set_value(50.0);
//    pageSizeSpinButton_.set_wrap(false);
}

PaginationBar::~PaginationBar() throw()
{
}

guint
PaginationBar::get_step() const throw()
{
    return step_;
}

guint
PaginationBar::get_total() const throw()
{
    return total_;
}

guint
PaginationBar::get_lower_limit() const throw()
{
    return lowerLimit_;
}

guint
PaginationBar::get_upper_limit() const throw()
{
    return upperLimit_;
}

void
PaginationBar::go_previous() throw()
{
    if (step_ > lowerLimit_)
    {
        lowerLimit_ = 0;
        upperLimit_ = step_;
    }
    else
    {
        lowerLimit_ -= step_;
        upperLimit_ = lowerLimit_ + step_;
    }

    limitsChanged_.emit();
}

void
PaginationBar::go_next() throw()
{
    upperLimit_ += step_;
    if (total_ < upperLimit_)
    {
        upperLimit_ = total_;
    }

    if (step_ > upperLimit_)
    {
        lowerLimit_ = 0;
    }
    else
    {
        lowerLimit_ = upperLimit_ - step_;
    }

    limitsChanged_.emit();
}

void
PaginationBar::go_first() throw()
{
    scroll_to_position(0);
}

void
PaginationBar::go_last() throw()
{
    scroll_to_position(total_ - 1);
}

sigc::signal<void> &
PaginationBar::limits_changed() throw()
{
    return limitsChanged_;
}

void
PaginationBar::scroll_to_position(guint position) throw()
{
    if (position >= total_)
    {
        return;
    }
    else if (position >= lowerLimit_ && position < upperLimit_)
    {
        return;
    }

    const guint old_lower_limit = lowerLimit_;
    const guint old_upper_limit = upperLimit_;

    if (position < lowerLimit_)
    {
        const double delta = lowerLimit_ - position;
        const guint pages = static_cast<guint>(
                                ceil(delta
                                     / static_cast<double>(step_)));

        if (pages * step_ > lowerLimit_)
        {
            lowerLimit_ = 0;
            upperLimit_ = step_;
        }
        else
        {
            lowerLimit_ -= pages * step_;
            upperLimit_ = lowerLimit_ + step_;
        }
    }
    else if (position >= upperLimit_)
    {
        const double delta = position - upperLimit_ + 1;
        const guint pages = static_cast<guint>(
                                ceil(delta
                                     / static_cast<double>(step_)));

        upperLimit_ += pages * step_;
        if (total_ < upperLimit_)
        {
            upperLimit_ = total_;
        }

        if (step_ > upperLimit_)
        {
            lowerLimit_ = 0;
        }
        else
        {
            lowerLimit_ = upperLimit_ - step_;
        }
    }

    if (old_lower_limit != lowerLimit_
        || old_upper_limit != upperLimit_)
    {
        limitsChanged_.emit();
    }
}

void
PaginationBar::set_step(guint step) throw()
{
    step_ = step;

    upperLimit_ = lowerLimit_ + step_;
    if (total_ < upperLimit_)
    {
        upperLimit_ = total_;
    }

    limitsChanged_.emit();
}

void
PaginationBar::set_total(guint total) throw()
{
    total_ = total;

    lowerLimit_ = 0;
    upperLimit_ = step_;
    if (total_ < upperLimit_)
    {
        upperLimit_ = total_;
    }

    limitsChanged_.emit();
}

} // namespace Solang
