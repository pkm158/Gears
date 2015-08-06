/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERCORE
#define RENDERCORE


#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include "RenderEntityManager.h"
#include "RenderPipe_opengl.h"

#ifdef D3D10_CORE
#include "Device_D3D10.h"
#endif

#ifdef OPENGL_CORE
#include "Device_opengl.h"
#endif

#include "WindowCallBackManager.h"

class RenderCore
{
public:
	RenderCore();
	virtual ~RenderCore();

	void initDevice( WindowCallBackManager *winCallBack );
	void draw( void );
private:

#ifdef D3D10_CORE
	Device_D3D10 *m_pDevice_D3D10;
#endif

#ifdef OPENGL_CORE
	Device_opengl *m_pDevice_opengl;
	IRenderPipe	  *m_renderPipe;
#endif

	void wtf(void);
};

#endif