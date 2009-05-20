#ifndef SOLANG_GP_CONTEXT_H
#define SOLANG_GP_CONTEXT_H

#include <gphoto2.h>

namespace Solang
{

struct GPhotoContext
{
    Camera *camera_;
    GPContext *context_;
    GPPortInfoList *portinfoList_;
    CameraAbilitiesList *abilitiesList_;
    CameraList *cameras_;

	void cleanup();

	GPhotoContext();
	~GPhotoContext();
};

} //namespace Solang

#endif
