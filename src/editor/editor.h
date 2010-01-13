/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2010 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_EDITOR_H
#define SOLANG_EDITOR_H

#include <string>
#include <tr1/memory>

#include <glibmm.h>
#include <gtkmm.h>

#include "plugin.h"
#include "types.h"

namespace Solang
{

class IOperationFactory;
typedef std::tr1::shared_ptr<IOperationFactory> IOperationFactoryPtr;

class Editor :
    public Plugin
{
    public:
        Editor() throw();

        virtual
        ~Editor() throw();

        virtual void
        init(Application & application) throw();

        virtual void
        final(Application & application) throw();

        virtual void
        visit_renderer(BrowserRenderer & browser_renderer) throw();

        virtual void
        visit_renderer(ConsoleRenderer & browser_renderer) throw();

        virtual void
        visit_renderer(EnlargedRenderer & editor_renderer) throw();

        virtual void
        visit_renderer(SlideshowRenderer & editor_renderer) throw();

    private:
        void
        apply_async(const IOperationPtr & operation) throw();

        void
        on_action_edit_photo_end(
            const EditablePhotoPtr & editable_photo) throw();

        void
        on_action_edit_photo(const IOperationFactoryPtr & factory)
                             throw();

        void
        on_renderer_changed(RendererRegistry & renderer_registry)
                            throw();

        void
        on_selection_changed() throw();

        void
        ui_hide() throw();

        void
        ui_show() throw();

        static const std::string uiFile_;

        ApplicationPtr application_;

        ActionGroupPtr actionGroup_;

        Gtk::UIManager::ui_merge_id uiID_;

        EditablePhotoMap editablePhotos_;

        PhotoPtr currentPhoto_;

        sigc::connection signalRendererChanged_;

        sigc::connection signalSelectionChanged_;
};

} // namespace solang

#endif // SOLANG_EDITOR_H
