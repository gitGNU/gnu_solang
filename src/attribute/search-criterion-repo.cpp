/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
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
#endif // HAVE_CONFIG_H

#include "search-criterion-repo.h"
#include "search-criterion-source.h"

namespace Solang
{

SearchCriterionRepo::SearchCriterionRepo()
    :mutex_(),
    criterion_(),
    sources_()
{
}

SearchCriterionRepo::~SearchCriterionRepo()
{
}

void
SearchCriterionRepo::register_source(
                const SearchCriterionSourcePtr &source) throw()
{
    sources_.push_back( source );
}

void
SearchCriterionRepo::update() throw()
{
    Glib::Mutex::Lock lock( mutex_ );
    criterion_.clear();
    for( SearchCriterionSourceList::iterator it = sources_.begin();
                                it != sources_.end(); it++ )
    {
        (*it)->get_criterion( criterion_ );
    }

    return;
}

} // namespace Solang
