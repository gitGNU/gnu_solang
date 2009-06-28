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

#ifndef SOLANG_FLICKR_CONTEXT_H
#define SOLANG_FLICKR_CONTEXT_H

#include <string>

#include <flickcurl.h>

namespace Solang
{

class FlickrContext
{
    public:
        FlickrContext() throw();

        ~FlickrContext() throw();

        flickcurl *
        get_flickr_session() throw();

        const std::string &
        get_user_name() const throw();

        void
        set_flickr_session(flickcurl * fc) throw();

        void
        set_user_name(const std::string & user_name) throw();

    protected:

    private:
        flickcurl * fc_;

        std::string userName_;
};

} // namespace Solang

#endif // SOLANG_FLICKR_CONTEXT_H
