/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPEMANAGER
#define RENDERPIPEMANAGER

#include "RenderPipe_bloom_opengl.h"
#include "RenderPipe_blur_opengl.h"
#include "RenderPipe_brightness_opengl.h"
#include "RenderPipe_deferredLit_opengl.h"
#include "RenderPipe_DLFin_opengl.h"
#include "RenderPipe_DLPre_opengl.h"
#include "RenderPipe_img_opengl.h"
#include "RenderPipe_imgBlend_opengl.h"
#include "RenderPipe_opengl.h"
#include "RenderPipe_zpass_opengl.h"
#include "RenderPipe_simple_opengl.h"
#include "RenderPipe_DLDiffBlend_opengl.h"
#include "RenderPipe_DLEmiss_opengl.h"
#include "RenderPipe_depth_opengl.h"

enum RenderPipeType
{
	PIPE_BLOOM = 0,
	PIPE_BLUR,
	PIPE_BRIGHTNESS,
	PIPE_DEFERREDlIT,
	PIPE_DLFINAL,
	PIPE_DLPRE,
	PIPE_IMG,
	PIPE_IMGBLEND,
	PIPE_MAIN,
	PIPE_ZPASS,
	PIPE_SIMPLEDIFF,
	PIPE_DLDIFFBLEND,
	PIPE_DLEMISS,
	PIPE_DEPTHOFFIELD,
	PIPE_NUM
};

class RenderPipeManager
{
public:
	RenderPipeManager(void);
	virtual ~RenderPipeManager(void);

	static void CreateRenderPipeManager(void);
	static void ReleaseRenderPipeManager(void);
	static RenderPipeManager *GetHandle(void);

	void init(Device_opengl *device);
	void release(void);

	IRenderPipe	*renderPipes[PIPE_NUM];
};

// only call this function
IRenderPipe *GetRenderPipe(RenderPipeType type);

extern RenderPipeManager *gpRenderPipeManager;
#endif