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

#ifndef SOLANG_DATABASE_H
#define SOLANG_DATABASE_H

#include <set>
#include <tr1/memory>

#include <libgdamm.h>

#include "db-table.h"
#include "photo.h"
#include "photo-search-criteria.h"
#include "tag.h"
#include "types.h"

namespace Solang
{

class Database
{
    private:
        static const Glib::ustring DB_NAME;
        Glib::ustring path_; //Path to SQLLite database
        Glib::RefPtr<Gnome::Gda::Client> gdaClient_;
        Glib::RefPtr<Gnome::Gda::Connection> gdaConnection_;
        Glib::RefPtr<Gnome::Gda::Dict> gdaDict_;
        std::map<Glib::ustring, DBTablePtr> tables_;

    public:
        Database(const Glib::ustring & path);

        Database( const Database &rhs );

        Database & operator =( const Database &rhs );

        ~Database();

        inline const Glib::ustring &
        get_path() const;

        void
        set_path(const Glib::ustring & path);

        void
        open() throw(Error);

        //Base functions

        //Save DBObject(s) to table
        void
        save( DBObject &object ) throw(Error);

        void
        save( const DBObjectList &objects,
              ProgressObserverPtr &observer ) throw(Error);

        //Get table object
        DBTablePtr
        getTable(const Glib::ustring &tableName) const;

        void
        run_sql(const Glib::ustring &sql) throw(Error);

        void
        close() throw(Error);

        //Utility functions
        //The following function list of valid photo id's
        PhotoList
        search( const PhotoSearchCriteriaList &criterion,
                const ProgressObserverPtr & observer) throw(Error);

        //Group by year
        DatePhotoInfoList
        get_dates_with_picture_count( const ProgressObserverPtr &);

        //Group by year, month
        DatePhotoInfoList
        get_dates_with_picture_count( gint year,
                            const ProgressObserverPtr &);

        //Group by year, month, day
        DatePhotoInfoList
        get_dates_with_picture_count(
            gint year, gint month,
            const ProgressObserverPtr &);

    private:
        DatePhotoInfoList
        get_dates_with_picture_count(
            const Glib::ustring & sql,
            const ProgressObserverPtr &);

        bool
        db_file_exists() const throw();

        void
        create_db() throw(Error);

//      TagList getTags(const PhotoPtr &photo) const throw(Error);
//      void saveTags(const PhotoPtr &photo) throw(Error);
};

inline const Glib::ustring &
Database::get_path() const
{
    return path_;
}

} // namespace Solang

#endif // SOLANG_DATABASE_H
