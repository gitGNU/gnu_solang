#ifndef SOLANG_DATE_PHOTO_INFO_H
#define SOLANG_DATE_PHOTO_INFO_H

#include "modification-date.h"

namespace Solang
{

class DatePhotoInfo
{
    public:
        DatePhotoInfo();
        DatePhotoInfo( const ModificationDate &, gint);
        DatePhotoInfo( const DatePhotoInfo & );
        ~DatePhotoInfo();

        DatePhotoInfo &
        operator =( const DatePhotoInfo & );

        inline const ModificationDate &
        get_date() const throw();

        void
        set_date( const ModificationDate &) throw();

        inline gint
        get_count() const throw();

        void
        set_count(gint) throw();

    private:
        ModificationDate date_;
        gint count_;

};

inline const ModificationDate &
DatePhotoInfo::get_date() const throw()
{
    return date_;
}

inline gint
DatePhotoInfo::get_count() const throw()
{
    return count_;
}

} //namespace Solang


#endif //SOLANG_DATE_PHOTO_INFO_H
