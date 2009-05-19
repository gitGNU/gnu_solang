#include "exif-data-key.h"

namespace Solang
{

Glib::ustring
ExifDataKey::get_query_criteria() const throw()
{
    std::ostringstream sout;
    sout<<key_<<" = '"<<value_<<"' ";
    return sout.str();
}

}
