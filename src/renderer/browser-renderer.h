/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * browser-renderer.h
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * browser-renderer.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * browser-renderer.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_BROWSER_RENDERER_H
#define SOLANG_BROWSER_RENDERER_H

#include <string>

#include <gdl/gdl.h>
#include <glibmm.h>
#include <gtkmm.h>
#include <sigc++/sigc++.h>

#include "browser-model-column-record.h"
#include "renderer.h"
#include "thumbnail-view.h"
#include "types.h"

namespace Solang
{

class BrowserRenderer :
    public Renderer,
    public sigc::trackable
{
    public:
        BrowserRenderer() throw();

        virtual
        ~BrowserRenderer() throw();

        virtual void
        init(Application & application) throw();

        virtual void
        render(const PhotoPtr & photo) throw();

        virtual void
        render(const PhotoList & photos) throw();

        virtual void
        final(Application & application) throw();

    protected:
        void
        on_photo_render_begin() throw();

        ApplicationPtr application_;

        BrowserModelColumnRecord modelColumnRecord_;

        ListStorePtr listStore_;

        Glib::RefPtr<Gtk::IconFactory> iconFactory_;

        const std::string dockItemName_;
    
        const Glib::ustring dockItemTitle_;
    
        GdlDockItemBehavior dockItemBehaviour_;

        GtkWidget * dockItem_;
    
        Gtk::ScrolledWindow scrolledWindow_;

        ThumbnailView thumbnailView_;

        sigc::connection photoRenderBegin_;

    private:
};

} // namespace Solang

#endif // SOLANG_BROWSER_RENDERER_H
