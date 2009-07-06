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

#ifndef SOLANG_PHOTO_SOURCE_ENUMS_H
#define SOLANG_PHOTO_SOURCE_ENUMS_H

namespace Solang
{

enum PhotoSourceOption
{
    PHOTO_SOURCE_COPY_PHOTOS_TRUE = 1,
    PHOTO_SOURCE_COPY_PHOTOS_FALSE = 2,
    PHOTO_SOURCE_COPY_PHOTOS_OPTIONAL = 4,

    PHOTO_SOURCE_INCLUDE_SUBFOLDERS_TRUE = 8,
    PHOTO_SOURCE_INCLUDE_SUBFOLDERS_FALSE = 16,
    PHOTO_SOURCE_INCLUDE_SUBFOLDERS_OPTIONAL = 32
};

} // namespace Solang

#endif // SOLANG_PHOTO_SOURCE_ENUMS_H
