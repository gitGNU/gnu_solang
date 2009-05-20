#include "gp-context.h"

namespace Solang
{

GPhotoContext::GPhotoContext()
    :camera_( NULL ),
    context_( NULL ),
    portinfoList_( NULL ),
    abilitiesList_( NULL ),
    cameras_( NULL )
{
}

GPhotoContext::~GPhotoContext()
{
    cleanup();
}

void GPhotoContext::cleanup()
{
    if( camera_ )
    {
        gp_camera_free( camera_ );
        camera_ = NULL;
    }
    if( context_ )
    {
        gp_context_unref( context_ );
        context_ = NULL;
    }
    if( portinfoList_ )
    {
        gp_port_info_list_free( portinfoList_ );
        portinfoList_ = NULL;
    }
    if( abilitiesList_ )
    {
        gp_abilities_list_free( abilitiesList_ );
        abilitiesList_ = NULL;
    }
    if( cameras_ )
    {
        gp_list_free( cameras_ );
        cameras_ = NULL;
    }
}

} //namespace Solang

