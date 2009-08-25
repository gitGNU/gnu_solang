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

#ifndef SOLANG_BASIC_EXIF_VIEW_H
#define SOLANG_BASIC_EXIF_VIEW_H

#include <gtkmm.h>

#include "basic-exif-view-model-column-record.h"
#include "types.h"

namespace Solang
{

class BasicExifView :
    public Gtk::TreeView
{
    public:
        BasicExifView() throw();

        virtual
        ~BasicExifView() throw();

        void
        populate(const ExifData & data) throw();

        void
        set_application( ApplicationPtr application ) throw();

    protected:
        virtual void
        on_drag_data_get(const DragContextPtr & context,
                         Gtk::SelectionData & selection_data,
                         guint info, guint time);

        ApplicationPtr application_;

        BasicExifViewModelColumnRecord modelColumnRecord_;

        ListStorePtr listStore_;

    private:
        void
        configure() throw();
};

} // namespace Solang

#endif // SOLANG_BASIC_EXIF_VIEW_H
