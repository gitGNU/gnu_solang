/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
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

#include "config.h"

#include "directory-destination.h"

namespace Solang
{

DirectoryDestination::DirectoryDestination(const Glib::ustring & path)
    throw() :
    PhotoDestination(),
    path_(path)
{
}

DirectoryDestination::~DirectoryDestination() throw()
{
}

void
DirectoryDestination::init() throw()
{
}

void
DirectoryDestination::export_photo(const PhotoPtr & photo,
                                   const ProgressObserverPtr & observer)
                                   throw()
{
}

void
DirectoryDestination::export_photos(
    const PhotoList & photos,
    const ProgressObserverPtr & observer) throw()
{
}

void DirectoryDestination::final() throw()
{
}

} // namespace Solang
