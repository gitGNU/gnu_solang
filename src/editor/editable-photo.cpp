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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <geglmm.h>
#include <geglmm/buffer.h>

#include "buffer-maker.h"
#include "buffer-pixbuf-converter.h"
#include "editable-photo.h"
#include "i-operation.h"
#include "photo.h"

namespace Solang
{

EditablePhoto::EditablePhoto(const PhotoPtr & photo,
                             const ProgressObserverPtr & observer) throw() :
    buffer_(0),
    photo_(photo),
    pixbuf_(0),
    pending_(),
    observer_(observer),
    applyEnd_(),
    threadPool_(1, false)
{
    applyEnd_.connect(sigc::mem_fun(*this,
                                    &EditablePhoto::on_apply_end));
}

EditablePhoto::~EditablePhoto() throw()
{
}

void
EditablePhoto::apply_async(const IOperationPtr & operation,
                           const SlotAsyncReady & slot) throw()
{
    const SlotAsyncReadyPtr slot_copy(new SlotAsyncReady(slot));

    pending_.push(std::make_pair(operation, slot_copy));
    if (1 < pending_.size())
    {
        return;
    }

    apply_begin();
}


void
EditablePhoto::apply_begin() throw()
{
    const std::pair<IOperationPtr, SlotAsyncReadyPtr> & current
        = pending_.front();

    threadPool_.push(sigc::bind(
                         sigc::mem_fun(*this,
                                       &EditablePhoto::apply_worker),
                         current.first));
}

void
EditablePhoto::apply_worker(const IOperationPtr & operation)
                            throw(Glib::Thread::Exit)
{
    if (0 == buffer_)
    {
        std::string path;
        try
        {
            path = Glib::filename_from_uri(photo_->get_uri());
        }
        catch (const Glib::ConvertError & e)
        {
            g_warning("%s", e.what().c_str());
            throw Glib::Thread::Exit();
        }

        BufferMaker buffer_maker;
        buffer_ = buffer_maker(path);
    }

    buffer_ = operation->apply(buffer_, observer_);
g_warning("done op");
    BufferPixbufConverter buffer_pixbuf_converter;
    pixbuf_ = buffer_pixbuf_converter(buffer_);
g_warning("done pixbuf");
    applyEnd_.emit();
}

void
EditablePhoto::on_apply_end() throw()
{
    const std::pair<IOperationPtr, SlotAsyncReadyPtr> current
        = pending_.front();

    pending_.pop();
    photo_->set_buffer(pixbuf_);

    if (0 != current.second && 0 != *current.second)
    {
        (*current.second)();
    }

    if (true == pending_.empty())
    {
        return;
    }

    apply_begin();
}

PhotoPtr &
EditablePhoto::get_photo() throw()
{
    return photo_;
}

} // namespace Solang
