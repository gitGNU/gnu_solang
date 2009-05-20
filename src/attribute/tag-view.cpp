/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * tag-view.cpp
 * Copyright (C) Debarshi Ray 2009 <rishi@gnu.org>
 * 
 * tag-view.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * tag-view.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include<iostream>


#include "tag.h"
#include "tag-view.h"

namespace Solang
{

TagView::TagView() throw() :
    Gtk::TreeView(),
    modelColumnRecord_(),
    listStore_(Gtk::ListStore::create(modelColumnRecord_))
{
    configure();
}


TagView::TagView(const TagList & tags) throw() :
    Gtk::TreeView(),
    modelColumnRecord_(),
    listStore_(Gtk::ListStore::create(modelColumnRecord_))
{
    configure();
    populate(tags);
}

TagView::~TagView() throw()
{
    unset_model();
}

void
TagView::configure() throw()
{
    set_model(listStore_);
    append_column("Selected", modelColumnRecord_.get_column_selected());
    append_column("Icon", modelColumnRecord_.get_column_pixbuf());
    append_column("Name", modelColumnRecord_.get_column_name());
    set_enable_search(true);
    // set_fixed_height_mode(true);
    set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_NONE);
    set_headers_clickable(false);
    set_headers_visible(false);
    set_tooltip_column(modelColumnRecord_.get_column_description_num());
}

void
TagView::populate(const TagList & tags) throw()
{
    listStore_->clear();
    TagList::const_iterator list_iter;

    for (list_iter = tags.begin(); list_iter != tags.end();
         list_iter++)
    {
        Gtk::TreeModel::iterator model_iter = listStore_->append();
        Gtk::TreeModel::Row row = *model_iter;
        const TagPtr & tag = *list_iter;

        row[modelColumnRecord_.get_column_tag()] = tag;
        row[modelColumnRecord_.get_column_selected()] = false;

        PixbufPtr pixbuf;
        std::string path;

        try
        {
            path = Glib::filename_from_utf8(tag->get_icon_path());
            pixbuf = Gdk::Pixbuf::create_from_file(path, 16, 16, true);
        }
        catch (const Glib::ConvertError & e)
        {
        }
        catch (const Glib::FileError & e)
        {
        }

        row[modelColumnRecord_.get_column_pixbuf()] = pixbuf;
        row[modelColumnRecord_.get_column_name()] = tag->get_name();
        row[modelColumnRecord_.get_column_description()]
            = tag->get_description();
    }
}

void 
TagView::on_row_activated ( const Gtk::TreeModel::Path& path,
                            Gtk::TreeViewColumn* column )
{
    TreeView::on_row_activated( path, column);

    Gtk::TreeModel::iterator current = listStore_->get_iter( path );
    Gtk::TreeModel::Row row = (*current);

    row[modelColumnRecord_.get_column_selected()] 
            = !( row[modelColumnRecord_.get_column_selected()] ) ;
    
}

void
TagView::get_criterion(
                PhotoSearchCriteriaList &selectedTags) const throw()
{

    Gtk::TreeModel::Children children = listStore_->children();
    for( Gtk::TreeModel::const_iterator current = children.begin();
                                current != children.end(); current++) 
    {
        Gtk::TreeModel::Row row = (*current);

        if( row[modelColumnRecord_.get_column_selected()] )
        {
            TagPtr tag = row[modelColumnRecord_.get_column_tag()];

            selectedTags.push_back( tag );
        }
    }
    return ;
}

void
TagView::get_selected_tags(TagList  &selectedTags) const throw()
{
    Gtk::TreeModel::Children children = listStore_->children();
    for( Gtk::TreeModel::const_iterator current = children.begin();
                                current != children.end(); current++) 
    {
        Gtk::TreeModel::Row row = (*current);

        if( row[modelColumnRecord_.get_column_selected()] )
        {
            TagPtr tag = row[modelColumnRecord_.get_column_tag()];

            selectedTags.push_back( tag );
        }
    }
    return;
}

void 
TagView::clear_tag_selection() throw()
{
    Gtk::TreeModel::Children children = listStore_->children();

    for( Gtk::TreeModel::const_iterator current = children.begin();
                                current != children.end(); current++) 
    {
        Gtk::TreeModel::Row row = (*current);

        row[modelColumnRecord_.get_column_selected()] = false;
    }
    return;

}

} // namespace Solang
