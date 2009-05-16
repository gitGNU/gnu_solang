/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * photo-source.h
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * photo-source.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * photo-source.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_PHOTO_SOURCE_H
#define SOLANG_PHOTO_SOURCE_H

#include "i-photo-source.h"

namespace Solang
{

class PhotoSource :
    public IPhotoSource 
{
    public:
        virtual
        ~PhotoSource() throw() = 0;

    protected:
        PhotoSource() throw();

    private:
};

} // namespace Solang

#endif // SOLANG_PHOTO_SOURCE_H
