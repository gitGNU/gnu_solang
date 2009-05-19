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

#ifndef SOLANG_ERROR_H
#define SOLANG_ERROR_H

#include <exception>
#include <stack>
#include <string>

#include <glibmm.h>

namespace Solang 
{

class Error :
    public std::exception
{
    public:
        struct FrameInfo
        {
            std::string function_;
            std::string filename_;
            guint64 line_;
        };

    private:
        guint64 errno_;

        Glib::ustring message_;

        std::stack<FrameInfo> callStack_;

    public:
        Error(guint64 errno,
              const Glib::ustring & message, 
              const std::string & function,
              const std::string & filename, 
              guint64 line);

        Error(const Error & rhs);

        Error &
        operator=(const Error & rhs);

        ~Error() throw();

        inline guint64
        get_errno() const throw();

        inline const Glib::ustring &
        get_message() const throw();

        inline const std::stack<FrameInfo> &
        get_call_stack() const throw();

        void
        add_call_info(const std::string & function,
                      const std::string & filename,
                      guint64 line);

        const char *
        what() const throw()
        {
            return message_.c_str();
        }
};

inline guint64
Error::get_errno() const throw()
{
    return errno_;
}

inline const Glib::ustring &
Error::get_message() const throw()
{
    return message_;
}

inline const std::stack<Error::FrameInfo> &
Error::get_call_stack() const throw()
{
    return callStack_;
}

} // namespace Solang

#endif // SOLANG_ERROR_H
