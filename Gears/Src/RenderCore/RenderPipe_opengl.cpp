/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_opengl.h"
#include "RenderEntityManager.h"
#include "UtilityManager.h"
#include "RenderPipeManager.h"
#include <list>

RenderPipe_opengl::RenderPipe_opengl( void )
{

}

RenderPipe_opengl::~RenderPipe_opengl( void )
{
	release();
}

bool RenderPipe_opengl::init( Device_opengl *device )
{
	m_pDevice = device;


	m_shadowmap_size =GetShadowMapSize();
	if(!Device_opengl::CreateRenderTargetOpenGL(m_shadowmap_size, m_shadowmap_size, &m_depthBufferID, 
		0, NULL, 
		GL_DEPTH_COMPONENT24, &m_depthTextureID))
	{
		return false;
	}

	glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer (GL_TRUE);
	glReadBuffer (GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, m_depthTextureID);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glBindTexture(GL_TEXTURE_2D, 0);


	int w, h;
	GetWindowWH(w, h);

	if(!Device_opengl::CreateRenderTargetOpenGL(
		w, h, GL_RGBA8, &m_canvasBufferID, &m_canvasTextureID) )
	{
		return false;
	}

	m_pipe_zpass	= dynamic_cast<RenderPipe_zpass_opengl *>(GetRenderPipe(PIPE_ZPASS));
	m_pipe_dlit		= dynamic_cast<RenderPipe_deferredLit_opengl *> (GetRenderPipe(PIPE_DEFERREDlIT));
	m_pipe_bloom	= dynamic_cast<RenderPipe_bloom_opengl *>(GetRenderPipe(PIPE_BLOOM));
#if _DEBUG
	m_pipe_img.init(device);
#endif
	return true;
}

void RenderPipe_opengl::render( void )
{	
	render(0);
}

void RenderPipe_opengl::render( GLuint target )
{
	m_pipe_zpass->setArgs(m_shadowmap_size);
	m_pipe_zpass->render(m_depthBufferID);

//	GLuint frame, buffer;
	m_pipe_dlit->setArgs(m_pipe_zpass->getLightViewMatrix(), m_pipe_zpass->getLightProjMatrix(), m_pipe_zpass->getShadowMapSize(), m_depthTextureID);

	m_pipe_dlit->render(m_canvasBufferID);

	GLuint bloomTex, bloomBuffer;
	m_pipe_dlit->getBloomTexAndBuffer(bloomTex, bloomBuffer);
	m_pipe_bloom->setArgs(m_canvasTextureID, m_canvasBufferID, bloomTex, bloomBuffer,  m_pipe_dlit->getDepthTexture());
	m_pipe_bloom->render();

}

void RenderPipe_opengl::release( void )
{
	glDeleteFramebuffersEXT(1, &m_depthBufferID);
	m_depthBufferID = NULL;
	glDeleteTextures(1, &m_depthTextureID);
	m_depthTextureID = NULL;

	glDeleteFramebuffersEXT(1, &m_canvasBufferID);
	m_canvasBufferID = NULL;
	glDeleteTextures(1, &m_canvasTextureID);
	m_canvasBufferID = NULL;
}