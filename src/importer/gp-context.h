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

#ifndef SOLANG_GP_CONTEXT_H
#define SOLANG_GP_CONTEXT_H

#include <gphoto2.h>

namespace Solang
{

struct GPhotoContext
{
    Camera *camera_;
    GPContext *context_;
    GPPortInfoList *portinfoList_;
    CameraAbilitiesList *abilitiesList_;
    CameraList *cameras_;

    void cleanup();

    GPhotoContext();
    ~GPhotoContext();
};

} // namespace Solang

#endif // SOLANG_GP_CONTEXT_H
