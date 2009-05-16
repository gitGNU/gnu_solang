/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * thumbnail-store.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * thumbnail-store.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * thumbnail-store.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_THUMBNAILSTORE_H
#define SOLANG_THUMBNAILSTORE_H

#include <glibmm.h>

namespace Solang
{

class ThumbnailStore
{
private:
    Glib::ustring path_;

public:
    ThumbnailStore(const Glib::ustring & path);

    ~ThumbnailStore();

    inline const Glib::ustring &
    get_path() const;	
};


inline const Glib::ustring &
ThumbnailStore::get_path() const
{
    return path_;
}

} // namespace Solang

#endif // SOLANG_THUMBNAILSTORE_H
