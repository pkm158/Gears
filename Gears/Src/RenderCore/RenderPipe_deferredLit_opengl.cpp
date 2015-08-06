/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_deferredLit_opengl.h"
#include "UtilityManager.h"
#include "RenderPipeManager.h"

RenderPipe_deferredLit_opengl::RenderPipe_deferredLit_opengl(void)
{
}

RenderPipe_deferredLit_opengl::~RenderPipe_deferredLit_opengl(void)
{
}

bool RenderPipe_deferredLit_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	int w, h;
	GetWindowWH(w, h);

	if(!Device_opengl::CreateRenderTargetOpenGL(w, h, &m_framebuffer, 
		GL_RGBA16F_ARB, m_textures, BUFFER_NUM,
		GL_DEPTH_COMPONENT24, &m_depthTexture))
	{
		return false;
	}

	m_pipe_pre			= dynamic_cast<RenderPipe_DLPre_opengl *>(GetRenderPipe(PIPE_DLPRE));
	m_pipe_fin			= dynamic_cast<RenderPipe_DLFin_opengl *>(GetRenderPipe(PIPE_DLFINAL));
	m_pipe_DiffBlend	= dynamic_cast<RenderPipe_DLDiffBlend_opengl *>(GetRenderPipe(PIPE_DLDIFFBLEND));
	m_pipe_emiss		= dynamic_cast<RenderPipe_DLEmiss_opengl *>(GetRenderPipe(PIPE_DLEMISS));
	return true;
}

void RenderPipe_deferredLit_opengl::render( void )
{
	render(0);
}

void RenderPipe_deferredLit_opengl::render( GLuint target )
{
	clear(m_framebuffer);
	m_pipe_pre->render(m_framebuffer);

	m_pipe_DiffBlend->setArgs( m_depthTexture, m_textures[BUFFER_AMB_DIFF]);
	m_pipe_DiffBlend->render(m_framebuffer);

	m_pipe_emiss->setArgs(m_depthTexture);
	m_pipe_emiss->render(m_framebuffer);

	m_pipe_fin->setArgs(m_textures);
	m_pipe_fin->render(target);
}

void RenderPipe_deferredLit_opengl::setArgs( const CMatrix& light_vmat, const CMatrix& light_pmat, int shadowMapSize, GLuint shadowTexture )
{
	m_pipe_pre->setArgs(light_vmat, light_pmat, shadowMapSize, shadowTexture);
}

void RenderPipe_deferredLit_opengl::release( void )
{
	glDeleteFramebuffersEXT(1, &m_framebuffer);
	for ( int i=0; i<BUFFER_NUM; i++ )
	{
		glDeleteTextures(1, &m_textures[i]);
	}

	if( m_depthTexture) 
	{
		glDeleteTextures(1, &m_depthTexture);
	}
}

GLuint RenderPipe_deferredLit_opengl::getDepthTexture( void )
{
	return m_depthTexture;
}

void RenderPipe_deferredLit_opengl::getBloomTexAndBuffer( GLuint &tex, GLuint &buffer )
{
	tex = m_textures[BUFFER_BLOOM];
	buffer = m_framebuffer;
}