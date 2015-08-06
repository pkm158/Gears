/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPE_OPENGL
#define RENDERPIPE_OPENGL

#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include "Device_opengl.h"
#include "IRenderPipe.h"

#include "RenderPipe_zpass_opengl.h"
#include "RenderPipe_deferredLit_opengl.h"
#include "RenderPipe_bloom_opengl.h"

class RenderPipe_opengl : public IRenderPipe
{
public:
	RenderPipe_opengl(void);
	virtual ~RenderPipe_opengl(void);

	bool init(Device_opengl *device);
	void release(void);

	void render(void);
	void render(GLuint target);

private:
	GLuint m_depthBufferID;
	GLuint m_depthTextureID;

	GLuint m_canvasBufferID;
	GLuint m_canvasTextureID;

	int m_shadowmap_size;

	RenderPipe_zpass_opengl		*m_pipe_zpass;
	RenderPipe_deferredLit_opengl *m_pipe_dlit;
	RenderPipe_bloom_opengl		*m_pipe_bloom;

#if _DEBUG
	RenderPipe_img_opengl m_pipe_img;
#endif
};

#endif