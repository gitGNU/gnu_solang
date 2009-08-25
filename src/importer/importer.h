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

#ifndef SOLANG_IMPORTER_H
#define SOLANG_IMPORTER_H

#include <tr1/memory>

#include <glibmm.h>
#include <gtkmm.h>

#include "plugin.h"
#include "types.h"

namespace Solang
{

class Application;
class IPhotoSource;
class RendererRegistry;

class Importer :
    public Plugin 
{
    public:
        Importer(const IPhotoSourcePtr & photo_source,
                 bool standard = false) throw();

        virtual
        ~Importer() throw();

        virtual void
        init(Application & application) throw();

        virtual void
        final(Application & application) throw();

        virtual void
        visit_renderer(BrowserRenderer & browser_renderer) throw();

        virtual void
        visit_renderer(ConsoleRenderer & console_renderer) throw();

        virtual void
        visit_renderer(EditorRenderer & editor_renderer) throw();

        virtual void
        visit_renderer(EnlargedRenderer & enlarged_renderer) throw();

        virtual void
        visit_renderer(SlideshowRenderer & slideshow_renderer) throw();

    protected:
        void
        on_action_photo_import() throw();

        void
        on_photo_import_begin() throw();

        void
        on_photo_import_end() throw();

        void
        on_photo_source_init_end(bool status) throw();

        void
        on_importer_dialog_response(
            gint response_id,
            ImporterDialogPtr & importer_dialog) throw();

        void
        on_renderer_changed(RendererRegistry & renderer_registry)
                            throw();

        void
        ui_hide() throw();

        void
        ui_show() throw();
 
        ApplicationPtr application_;

        IPhotoSourcePtr photoSource_;

        bool standard_;

        ActionGroupPtr actionGroup_;

        Gtk::UIManager::ui_merge_id standardUIID_;

        Gtk::UIManager::ui_merge_id uiID_;

        sigc::connection signalPhotoImportBegin_;

        sigc::connection signalPhotoImportEnd_;

    private:
};

} // namespace Solang

#endif // SOLANG_IMPORTER_H
