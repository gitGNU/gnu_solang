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

#ifndef SOLANG_DIRECTORY_DESTINATION_H
#define SOLANG_DIRECTORY_DESTINATION_H

#include <glibmm.h>

#include "photo-destination.h"

namespace Solang
{

class DirectoryDestination :
    public PhotoDestination
{
    public:
        DirectoryDestination(const Glib::ustring & path) throw();

        virtual
        ~DirectoryDestination() throw();

        virtual void
        init() throw();

        virtual void
        export_photo(const PhotoPtr & photo,
                     const ProgressObserverPtr & observer) throw();

        virtual void
        export_photos(const PhotoList & photos,
                      const ProgressObserverPtr & observer) throw();

        virtual void
        final() throw();

    protected:
        Glib::ustring path_;

    private:
};

} // namespace Solang

#endif // SOLANG_DIRECTORY_DESTINATION_H
