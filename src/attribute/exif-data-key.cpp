#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "exif-data-key.h"
#include "id-base.h"

namespace Solang
{

Glib::ustring
ExifDataKey::get_query_criteria() const throw()
{
    std::ostringstream sout;
    sout<<key_<<" = '"<<value_<<"' ";
    return sout.str();
}

PhotoSearchCriteria::ClubbingOperationType
ExifDataKey::get_clubbing_type() const throw()
{
    return PhotoSearchCriteria::CLUB_AND;
}

gint32
ExifDataKey::get_id() const throw()
{
    return IDBase<ExifDataKey>::get_id();
}

Glib::ustring
ExifDataKey::get_criteria_description() const throw()
{
    return title_ + ": " + value_;
}

Glib::ustring
ExifDataKey::get_criteria_icon_path() const throw()
{
    //TBD::CORRECT
    return PACKAGE_DATA_DIR"/"PACKAGE_TARNAME"/pixmaps/tag-16.png";
}

}
