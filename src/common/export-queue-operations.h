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

#ifndef SOLANG_EXPORT_QUEUE_OPERATIONS_H
#define SOLANG_EXPORT_QUEUE_OPERATIONS_H

#include <functional>

#include <gdkmm.h>
#include <glibmm.h>

#include "types.h"

class Application;

namespace Solang
{

class ExportQueueCleaner :
    public std::unary_function<void, void>
{
    public:
        ExportQueueCleaner(Application & application) throw();

        ExportQueueCleaner(const ExportQueueCleaner & source) throw();

        virtual
        ~ExportQueueCleaner() throw();

        ExportQueueCleaner &
        operator=(const ExportQueueCleaner & source) throw();

        void
        operator()() throw();

    protected:
        ApplicationPtr application_;

    private:
};

class ExportQueueInserter :
    public std::unary_function<void, void>
{
    public:
        ExportQueueInserter(Application & application) throw();

        ExportQueueInserter(const ExportQueueInserter & source)
                            throw();

        virtual
        ~ExportQueueInserter() throw();

        ExportQueueInserter &
        operator=(const ExportQueueInserter & source) throw();

        void
        operator()() throw();

    protected:
        ApplicationPtr application_;

    private:
};

} // namespace Solang

#endif // SOLANG_EXPORT_QUEUE_OPERATIONS_H
