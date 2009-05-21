#include "date-photo-info.h"

namespace Solang
{

DatePhotoInfo::DatePhotoInfo()
    :date_(),
    count_(0)
{
}

DatePhotoInfo::DatePhotoInfo(
                    const ModificationDate &date, gint count )
    :date_( date ),
    count_( count )
{
}

DatePhotoInfo::DatePhotoInfo( const DatePhotoInfo &rhs )
    :date_( rhs.date_ ),
    count_( rhs.count_ )
{
}


DatePhotoInfo::~DatePhotoInfo()
{
}

DatePhotoInfo &
DatePhotoInfo::operator =( const DatePhotoInfo &rhs )
{
    if( this != &rhs )
    {
        date_ = rhs.date_;
        count_ = rhs.count_;
    }
    return *this;
}

void
DatePhotoInfo::set_date( const ModificationDate &date) throw()
{
    date_ = date;
}

void
DatePhotoInfo::set_count(gint count) throw()
{
    count_ = count;
}

}
