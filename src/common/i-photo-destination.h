/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * i-photo-destination.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * i-photo-destination.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * i-photo-destination.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_I_PHOTO_DESTINATION_H
#define SOLANG_I_PHOTO_DESTINATION_H

#include "non-copyable.h"
#include "types.h"

namespace Solang
{

class IPhotoDestination :
    public NonCopyable
{
    public:
        virtual
        ~IPhotoDestination() throw() = 0;

		virtual void
        init() throw() = 0;

		virtual void
        export_photo(const PhotoPtr & photo,
                     const ProgressObserverPtr & observer) throw() = 0;
		virtual void
        export_photos(const PhotoList & photos,
                      const ProgressObserverPtr & observer) throw() = 0;

		virtual void
        final() throw() = 0;

    protected:
        IPhotoDestination() throw();

    private:
};

} // namespace Solang

#endif // SOLANG_I_PHOTO_DESTINATION_H
