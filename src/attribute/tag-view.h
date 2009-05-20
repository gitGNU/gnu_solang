/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * tag-view.h
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * tag-view.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * tag-view.h is distributed in the hope that it will be useful, but
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

#include "search-criterion-source.h"
#include "tag-view-model-column-record.h"
#include "types.h"

namespace Solang
{

class TagView :
    public Gtk::TreeView,
    public SearchCriterionSource
{
    public:
        TagView() throw();

        TagView(const TagList & tags) throw();

        ~TagView() throw();

        void
        populate(const TagList & tags) throw();

        //Signal handlers
        void 
        on_row_activated (     
                const Gtk::TreeModel::Path& path,
                Gtk::TreeViewColumn*      column );

        void
        get_criterion(PhotoSearchCriteriaList &) const throw();

        void 
        get_selected_tags(TagList &) const throw();

        void
        clear_tag_selection() throw();

        inline const ListStorePtr &
        get_store() const throw();

        inline const TagViewModelColumnRecord &
        get_column_records() const throw();

    protected:
        TagViewModelColumnRecord modelColumnRecord_;

        ListStorePtr listStore_;

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
