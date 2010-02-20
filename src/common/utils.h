/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2010 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_UTILS_H
#define SOLANG_UTILS_H

#include <glibmm.h>

namespace Solang
{

/*
 * Based on code by: Calvin Williamson
 *
 * This code was originally written in C as a part of GEGL:
 * gegl/gegl-utils.h
 */

extern const gdouble EPSILON;

bool is_zero(gdouble value);
bool is_equal(gdouble v1, gdouble v2);

} // namespace Solang

#endif // SOLANG_UTILS_H
