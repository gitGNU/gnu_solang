/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2009, 2010 Debarshi Ray <rishi@gnu.org>
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

#ifndef SOLANG_TAG_VIEW_H
#define SOLANG_TAG_VIEW_H

#include <gtkmm.h>

#include "tag-view-model-column-record.h"
#include "types.h"

namespace Solang
{

class TagView :
    public Gtk::TreeView
{
    public:
        TagView() throw();

        TagView( const TagList & tags ) throw();

        ~TagView() throw();

        void
        populate(const TagList & tags) throw();

        void 
        get_selected_tags(TagList &) const throw();

        void
        set_application( ApplicationPtr application ) throw();

        inline const ListStorePtr &
        get_store() const throw();

        inline const TagViewModelColumnRecord &
        get_column_records() const throw();

        virtual void on_drag_data_get(
          const Glib::RefPtr<Gdk::DragContext>& context,
          Gtk::SelectionData& selection_data, guint info, guint time);


    protected:
        //Signal handlers
        void
        on_row_activated (
                const Gtk::TreeModel::Path& path,
                Gtk::TreeViewColumn*      column );

        ApplicationPtr application_;

        TagViewModelColumnRecord modelColumnRecord_;

        ListStorePtr listStore_;

        bool isSelectionAllowed_;

    private:
        void
        configure() throw();
};

inline const ListStorePtr &
TagView::get_store() const throw()
{
    return listStore_;
}

inline const TagViewModelColumnRecord &
TagView::get_column_records() const throw()
{
    return modelColumnRecord_;
}

} // namespace Solang

#endif // SOLANG_TAG_VIEW_H
