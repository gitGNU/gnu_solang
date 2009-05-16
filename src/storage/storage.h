/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * storage.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * storage.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * storage.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_STORAGE_H
#define SOLANG_STORAGE_H

#include "i-storage.h"
#include "types.h"
#include "thumbnail-store.h"
//#include "renderable-photo.h"

namespace Solang
{

class Storage :
    public IStorage
{
public:
    virtual
    ~Storage() throw() = 0;

protected:
    Storage(const ThumbnailStore & store, const DatabasePtr & db)
            throw();

    ThumbnailStore store_;

    DatabasePtr db_;

private:
    // Blocked.
    Storage(const Storage & source) throw();

    // Blocked.
    Storage &
    operator=(const Storage & source) throw();
};

} // namespace Solang

#endif // SOLANG_STORAGE_H
