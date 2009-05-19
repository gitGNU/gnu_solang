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
	if( camera_ )
	{
		gp_camera_free( camera_ );
	}
	if( cameras_ )
	{
		gp_list_free( cameras_ );
	}
	//TBD CLEANUP
}

} //namespace Solang

