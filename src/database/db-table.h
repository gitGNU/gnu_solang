/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * db-table.h
 * Copyright (C) Santanu Sinha 2009 <santanu.sinha@gmail.com>
 * 
 * d-b-table.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * d-b-table.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLANG_DB_TABLE_H
#define SOLANG_DB_TABLE_H

#include <libgdamm.h>

#include "error.h"
#include "types.h"

namespace Solang
{

class DBObject;
class DBTableVisitor;

class DBTable
{
    private:
        static const gint32 PRIMARY_KEY_COL;
        Glib::ustring tableName_;
        Glib::RefPtr<Gnome::Gda::Query> query_;
        gint32 lastKey_;

    protected:
        Glib::RefPtr<Gnome::Gda::DataModelQuery> model_;

    public:
        DBTable( const Glib::ustring &tableName );
        DBTable(const DBTable &rhs);
        DBTable &operator = (const DBTable &rhs);
        virtual ~DBTable()
        {
        }

        void open(
                const Glib::RefPtr<Gnome::Gda::Dict> &gdaDict
                                                ) throw(Error);
        virtual void receive( DBTableVisitor &visitor, 
                        ProgressObserverPtr &progressObserver ) = 0;  
            //to extract recordset of leaf level database objects
        virtual void insert( DBObject &object );
        virtual void update( DBObject &object );
        virtual void remove( DBObject &object );

        virtual Glib::ustring getSelectionQuery() const = 0;
        virtual Glib::ustring getInsertQuery() const = 0;
        virtual Glib::ustring getUpdateQuery() const = 0;
        virtual Glib::ustring getDeleteQuery() const = 0;

        inline const DataModelPtr &
        get_model()
        {
            return model_;
        }
        
};


} // namespace Solang

#endif // SOLANG_DB_TABLE_H
