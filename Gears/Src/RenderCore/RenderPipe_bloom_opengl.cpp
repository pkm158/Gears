/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_bloom_opengl.h"
#include "UtilityManager.h"
#include "RenderPipeManager.h"
#include "RenderEntityManager.h"

RenderPipe_bloom_opengl::RenderPipe_bloom_opengl(void)
{
}

RenderPipe_bloom_opengl::~RenderPipe_bloom_opengl(void)
{
	release();
}

bool RenderPipe_bloom_opengl::init( Device_opengl *device )
{
	bool result = true;

	int w, h;
	GetWindowWH(w, h);

	m_width = w;
	m_height = h;

	GLuint depthID;

	int blur_intense_l1 = GetBlurIntenseLevel1();
	int blur_intense_l2 = GetBlurIntenseLevel2();

	if (IsFullscreenBloom())
	{
		if(!Device_opengl::CreateRenderTargetOpenGL(
			m_width/blur_intense_l1, m_height/blur_intense_l1, GL_RGBA8, &m_brightBufferID[0], &m_brightTextureID[0]) )
		{
			return false;
		}
	} else
	{
		m_brightBufferID[0] = NULL;
		m_brightTextureID[0] = NULL;
	}

	if (IsShinessBloom())
	{
		if(!Device_opengl::CreateRenderTargetOpenGL(
			m_width/blur_intense_l2, m_height/blur_intense_l2, GL_RGBA8, &m_brightBufferID[1], &m_brightTextureID[1]) )
		{
			return false;
		}
	} else
	{
		m_brightBufferID[1] = NULL;
		m_brightTextureID[1] = NULL;
	}

	m_pipe_brightness	= dynamic_cast<RenderPipe_brightness_opengl *>(GetRenderPipe(PIPE_BRIGHTNESS));
	m_pipe_blur			= dynamic_cast<RenderPipe_blur_opengl *>(GetRenderPipe(PIPE_BLUR));
	m_pipe_imgBlend		= dynamic_cast<RenderPipe_imgBlend_opengl *>(GetRenderPipe(PIPE_IMGBLEND));
	m_pipe_img			= dynamic_cast<RenderPipe_img_opengl *>(GetRenderPipe(PIPE_IMG));
	m_pipe_simpleDiff	= dynamic_cast<RenderPipe_simple_opengl *>(GetRenderPipe(PIPE_SIMPLEDIFF));
	m_pipe_depthOfField	= dynamic_cast<RenderPipe_depth_opengl *>(GetRenderPipe(PIPE_DEPTHOFFIELD));
	return result;
}

void RenderPipe_bloom_opengl::release( void )
{

	for (int idx=0; idx<2; idx++)
	{
		glDeleteFramebuffersEXT(1, &m_brightBufferID[idx]);
		m_brightBufferID[idx] = NULL;
		glDeleteTextures(1, &m_brightTextureID[idx]);
		m_brightTextureID[idx] = NULL;
	}
}

void RenderPipe_bloom_opengl::render( void )
{
	render(0);
}

void RenderPipe_bloom_opengl::render( GLuint target )
{
	if (!IsDepthOfField() &&
		!IsFullscreenBloom() &&
		!IsShinessBloom())
	{
		m_pipe_img->setArgs(m_canvasTexture);
		m_pipe_img->render();
	} else
	{
		m_pipe_brightness->render(m_brightBufferID[0]);

		m_pipe_blur->setArgs(BLUR_L1, m_brightTextureID[0]);
		m_pipe_blur->render(m_brightBufferID[0]);
	}


	if (IsDepthOfField())
	{
		m_pipe_depthOfField->setArgs(m_canvasTexture, m_brightTextureID[0], m_depthTexure);
		m_pipe_depthOfField->render(m_canvasBuffer);
	}

	if (IsFullscreenBloom())
	{
		m_pipe_brightness->render(m_brightBufferID[0]);

		m_pipe_blur->setArgs(BLUR_L1, m_brightTextureID[0]);
		m_pipe_blur->render(m_brightBufferID[0]);
		
		float scale = GetFullScreenBloomFactor();
		m_pipe_imgBlend->setArgs(m_canvasTexture, m_brightTextureID[0], 1-scale, scale);

		if (IsShinessBloom())
		{
			m_pipe_imgBlend->render(m_canvasBuffer);
		}
		else
		{
			m_pipe_imgBlend->render(target);
		}
	}
	

	if (IsShinessBloom())
	{
		glEnable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_ONE, GL_ONE);
		RenderEntityList simpleDiffList = GetRenderEntityList(ENTITIES_LIGHT);
		m_pipe_simpleDiff->setArgs(m_depthTexure, TYPE_REGULAR);
		m_pipe_simpleDiff->setEntitiesToRender(simpleDiffList);
		m_pipe_simpleDiff->render(m_shininessBuffer);
		glDisable(GL_BLEND);

		int blur_intense_l2 = GetBlurIntenseLevel2();

		m_pipe_brightness->setArgs(m_width/blur_intense_l2, m_height/blur_intense_l2, m_shininessTexture);
		m_pipe_brightness->render(m_brightBufferID[1]);

		m_pipe_blur->setArgs(BLUR_L2, m_brightTextureID[1]);
		m_pipe_blur->render(m_brightBufferID[1]);

		float scale = GetShinessBloomFactor();
		
		m_pipe_imgBlend->setArgs(m_canvasTexture, m_brightTextureID[1], 1.0f, 1.0f * scale);
		m_pipe_imgBlend->render(target);
	
	}

	
}

void RenderPipe_bloom_opengl::setArgs( GLuint canvasTexture, GLuint canvasBuffer, GLuint shininessTexture, GLuint shininessBuffer, GLuint depthTexture,
									  const CVector &scale, const CVector &offset )
{
	m_canvasTexture = canvasTexture;
	m_canvasBuffer  = canvasBuffer;
	m_depthTexure = depthTexture;
	m_shininessTexture = shininessTexture;
	m_shininessBuffer  = shininessBuffer;

	int blur_intense_l1 = GetBlurIntenseLevel1();

	m_pipe_brightness->setArgs( m_width/blur_intense_l1, m_height/blur_intense_l1,canvasTexture, scale, offset);
}

void RenderPipe_bloom_opengl::setArgs( GLuint canvasTexture, GLuint canvasBuffer, GLuint shininessTexture, GLuint shininessBuffer, GLuint depthTexture )
{
	m_canvasTexture = canvasTexture;
	m_canvasBuffer  = canvasBuffer;
	m_shininessTexture = shininessTexture;
	m_shininessBuffer  = shininessBuffer;
	m_depthTexure = depthTexture;

	int blur_intense_l1 = GetBlurIntenseLevel1();
	m_pipe_brightness->setArgs( m_width/blur_intense_l1, m_height/blur_intense_l1,canvasTexture);
}