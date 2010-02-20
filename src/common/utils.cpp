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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "utils.h"

namespace Solang
{

/*
 * Based on code by: Calvin Williamson
 *
 * This code was originally written in C as a part of GEGL:
 * gegl/gegl-utils.c
 */

const gdouble EPSILON = 1e-5;

bool
is_zero(gdouble value)
{
    return value > -EPSILON && value < EPSILON;
}

bool
is_equal(gdouble v1, gdouble v2)
{
    const register gdouble diff = v1 - v2;
    return diff > -EPSILON && diff < EPSILON;
}

} // namespace Solang
