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

extern "C"
{
#include <babl/babl.h>
}

inline Babl *get_babl_format_from_cairo()
{
   //The following code is taken from nipce
    //Copyright (C) 2008-2009 Hubert Figuiere
    return babl_format_new(babl_model("R'G'B'A"),
                           babl_type ("u8"),
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
                           babl_component ("B'"),
                           babl_component ("G'"),
                           babl_component ("R'"),
                           babl_component ("A"),
#elif G_BYTE_ORDER == G_BIG_ENDIAN
                           babl_component ("A"),
                           babl_component ("R'"),
                           babl_component ("G'"),
                           babl_component ("B'"),
#else
#error unknown endian
#endif
                           NULL);

}
