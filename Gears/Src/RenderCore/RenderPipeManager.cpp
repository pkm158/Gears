/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipeManager.h"
#include "UtilityManager.h"
RenderPipeManager *gpRenderPipeManager = NULL;

RenderPipeManager::RenderPipeManager(void)
{

	renderPipes[PIPE_BLOOM]       = new RenderPipe_bloom_opengl();
	renderPipes[PIPE_BLUR]        = new RenderPipe_blur_opengl();
	renderPipes[PIPE_BRIGHTNESS]  = new RenderPipe_brightness_opengl();
	renderPipes[PIPE_DEFERREDlIT] = new RenderPipe_deferredLit_opengl();
	renderPipes[PIPE_DLFINAL]	    = new RenderPipe_DLFin_opengl();
	renderPipes[PIPE_DLPRE]	    = new RenderPipe_DLPre_opengl();
	renderPipes[PIPE_IMG]			= new RenderPipe_img_opengl();
	renderPipes[PIPE_IMGBLEND]	= new RenderPipe_imgBlend_opengl();
	renderPipes[PIPE_MAIN]				= new RenderPipe_opengl();
	renderPipes[PIPE_ZPASS]		= new RenderPipe_zpass_opengl();
	renderPipes[PIPE_SIMPLEDIFF]	= new RenderPipe_simple_opengl();
	renderPipes[PIPE_DLDIFFBLEND] = new RenderPipe_DLDiffBlend_opengl();
	renderPipes[PIPE_DLEMISS]		= new RenderPipe_DLEmiss_opengl();
	renderPipes[PIPE_DEPTHOFFIELD]= new RenderPipe_depth_opengl();
}

RenderPipeManager::~RenderPipeManager(void)
{
	for (int idx=0; idx<PIPE_NUM; idx++)
	{
		SAFE_DELETE(renderPipes[idx]);
	}
}

void RenderPipeManager::CreateRenderPipeManager( void )
{
	DEBUG_PRINT("Create the render pipe manager...");

	if (NULL == gpRenderPipeManager)
	{
		gpRenderPipeManager = new RenderPipeManager();
	}
}

void RenderPipeManager::ReleaseRenderPipeManager( void )
{
	SAFE_DELETE(gpRenderPipeManager);
}

RenderPipeManager * RenderPipeManager::GetHandle( void )
{
	HR(gpRenderPipeManager);
	return gpRenderPipeManager;
}

void RenderPipeManager::init( Device_opengl *device )
{

	for (int idx=0; idx<PIPE_NUM; idx++)
	{
		HR(renderPipes[idx]->init(device));
	}

}

void RenderPipeManager::release( void )
{

	for (int idx=0; idx<PIPE_NUM; idx++)
	{
		renderPipes[idx]->release();
	}
}

IRenderPipe * GetRenderPipe( RenderPipeType type )
{
	return gpRenderPipeManager->renderPipes[type];
}