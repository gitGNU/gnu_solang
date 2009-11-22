/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009 Debarshi Ray <rishi@gnu.org>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <cstdlib>

#include <flickcurl.h>
#include <gegl.h>
#include <giomm.h>
#include <glibmm.h>
#include <glibmm/i18n.h>
#include <gtkmm.h>
#include <libgdamm.h>

#ifdef ENABLE_NLS
#include <libintl.h>
#endif // ENABLE_NLS

#include "application.h"

int
main(int argc, char *argv[])
{
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

    Glib::thread_init();
    Gio::init();
    Gnome::Gda::init();
    gegl_init( &argc, &argv );

    if (0 != flickcurl_init())
    {
        g_warning ("Failed to initialize FlickCurl");
        return EXIT_FAILURE;
    }

    Glib::set_prgname(PACKAGE_TARNAME);
    Glib::set_application_name(_(PACKAGE_NAME));

    Gtk::Main kit(argc, argv, true);
#if (GEGL_MINOR_VERSION == 0 )
    GObject *pConfig = gegl_config();
#else
    GeglConfig *pConfig = gegl_config();
#endif
    {
        GValue cacheSize = {0};
        g_value_init( &cacheSize, G_TYPE_INT );
        g_value_set_int( &cacheSize, 102400 );
        g_object_set_property( (GObject *)pConfig, "cache-size", &cacheSize );
        g_value_unset( &cacheSize );
    }
    {
        GValue quality = {0};
        g_value_init( &quality, G_TYPE_DOUBLE );
        g_value_set_double( &quality, 0.1 );
        g_object_set_property( (GObject *)pConfig, "quality", &quality );
        g_value_unset( &quality );
    }
    {
        GValue swap = {0};
        g_value_init( &swap, G_TYPE_STRING );
        g_value_set_string( &swap, "/tmp" );
        g_object_set_property( (GObject *)pConfig, "swap", &swap );
        g_value_unset( &swap );
    }
    Solang::Application application(argc, argv);

    application.init();
    application.run();
    application.final();

    flickcurl_finish();

    return EXIT_SUCCESS;
}
