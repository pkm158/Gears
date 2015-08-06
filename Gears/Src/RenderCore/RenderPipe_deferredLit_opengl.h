/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef IRENDERPIPE_DEFERREDLIT_OPENGL
#define IRENDERPIPE_DEFERREDLIT_OPENGL

#include "IRenderpipe.h"
#include "RenderPipe_DLPre_opengl.h"
#include "RenderPipe_DLFin_opengl.h"
#include "RenderPipe_DLDiffBlend_opengl.h"
#include "RenderPipe_DLEmiss_opengl.h"

class RenderPipe_deferredLit_opengl :
	public IRenderPipe
{
public:
	RenderPipe_deferredLit_opengl(void);
	virtual ~RenderPipe_deferredLit_opengl(void);

	bool init(Device_opengl *device);
	void release(void);

	void render(void);
	void render(GLuint target);
	// light_vmat and light_pmat means the light for shadow
	void setArgs( const CMatrix& light_vmat, const CMatrix& light_pmat, int shadowMapSize,GLuint shadowTexture );

	GLuint getDepthTexture(void);
	void getBloomTexAndBuffer(GLuint &tex, GLuint &buffer);

private:

	GLuint m_framebuffer;
	GLuint m_textures[BUFFER_NUM];

	GLuint m_depthTexture;

	RenderPipe_DLPre_opengl			*m_pipe_pre;
	RenderPipe_DLFin_opengl			*m_pipe_fin;
	RenderPipe_DLDiffBlend_opengl	*m_pipe_DiffBlend;
	RenderPipe_DLEmiss_opengl		*m_pipe_emiss;
};

#endif