/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * error.cpp
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * error.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * error.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "error.h"

namespace Solang
{

Error::Error(guint64 errno, const Glib::ustring & message,
             const std::string & function, const std::string & filename,
             guint64 line) :
    std::exception(),
    errno_(errno),
    message_(message),
    callStack_()
{
    add_call_info(function, filename, line);    
}

Error::Error(const Error & rhs) :
    std::exception(rhs),
    errno_(rhs.errno_),
    message_(rhs.message_),
    callStack_(rhs.callStack_)
{
}

Error &
Error::operator=(const Error & rhs)
{
    if (this != &rhs)
    {
        errno_ = rhs.errno_;
        message_ = rhs.message_;
        callStack_ = rhs.callStack_;
    }

    return *this;
}

Error::~Error() throw()
{
}

void
Error::add_call_info(const std::string & function, 
                     const std::string & filename, 
                     guint64 line)
{
    FrameInfo info;

    info.function_ = function;
    info.filename_ = filename;
    info.line_ = line;

    callStack_.push(info);
}


} // namespace Solang
