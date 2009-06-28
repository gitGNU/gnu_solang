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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <glibmm.h>

#include "flickr-context.h"

namespace Solang
{

FlickrContext::FlickrContext() throw() :
    fc_(flickcurl_new()),
    userName_()
{
    flickcurl_set_user_agent(fc_,
                             Glib::get_application_name().c_str());
}

FlickrContext::~FlickrContext() throw()
{
    flickcurl_free(fc_);
}

flickcurl *
FlickrContext::get_flickr_session() throw()
{
    return fc_;
}

const std::string &
FlickrContext::get_user_name() const throw()
{
    return userName_;
}

void
FlickrContext::set_flickr_session(flickcurl * fc) throw()
{
    fc_ = fc;
}

void
FlickrContext::set_user_name(const std::string & user_name) throw()
{
    userName_ = user_name;
}

} // namespace Solang
