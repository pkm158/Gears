/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "Device_opengl.h"
#include "GutModel_OpenGL.h"
#ifdef OPENGL_CORE
Device_opengl::Device_opengl( HWND hwnd ) : 
						m_hWnd(hwnd),
						m_hDC(NULL),
						m_hGLRC(NULL)
								
{

}

Device_opengl::~Device_opengl()
{
	wglMakeCurrent(m_hDC, NULL);
	wglDeleteContext(m_hGLRC);
	m_hGLRC = NULL;

	ReleaseDC(m_hWnd, m_hDC);
	m_hDC = NULL;
}

bool Device_opengl::initOpengl( void )
{
	if(!m_hWnd) return false;

	m_hDC = GetDC(m_hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion		= 1;
	pfd.dwFlags			= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
	pfd.iPixelType		= PFD_TYPE_RGBA;
	pfd.cDepthBits		= 32; // 24 bits zbuffer
	pfd.cStencilBits	= 0; // 0 bits stencil buffer, I don't use it, everything go to textures
	pfd.iLayerType		= PFD_MAIN_PLANE; // main layer

	int pixelformat = ChoosePixelFormat(m_hDC, &pfd);

	if ( pixelformat == 0 ) 
	{ 
		return false; 
	}

	if ( SetPixelFormat(m_hDC, pixelformat, &pfd) == FALSE) 
	{ 
		ReleaseDC(m_hWnd, m_hDC);
		return false; 
	}

	m_hGLRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hGLRC);

	glewInit();
	return initOpenglResource();

}

bool Device_opengl::reset( void )
{
	return true;
}

bool Device_opengl::initOpenglResource( void )
{
	// all of thes are default settings
	glDisable(GL_CULL_FACE);
	// 启动2D贴图功能
	glEnable(GL_TEXTURE_2D);
	// 启动zbuffer隐藏面消除
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);

	glCullFace(GL_FRONT);	// in case I turned GL_CULL_FACE on some where
	return true;
}

// No depth buffer
bool  Device_opengl::CreateRenderTargetOpenGL(int w, int h, GLuint color_fmt, GLuint *pFramebuffer, GLuint *pTexture)
{
	GLuint framebuffer, texture;

	*pFramebuffer = 0;
	*pTexture = 0;

	glGenFramebuffersEXT(1, &framebuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

	// create a framebuffer for rendering
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	if ( color_fmt==GL_RGBA32F_ARB || color_fmt==GL_RGBA16F_ARB )
	{
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// size and format
	glTexImage2D(GL_TEXTURE_2D, 0, color_fmt,  w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	// bind framebuffer and frame texture
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);

	// check if success
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if ( status!=GL_FRAMEBUFFER_COMPLETE_EXT )
	{
		return false;
	}

	*pFramebuffer = framebuffer;
	*pTexture = texture;

	return true;
}

// with depth buffer, w is buffer's width, and h is buffer's height. 
// num_mrts is the number of textures which are going to be bind with this framebuffer
bool  Device_opengl::CreateRenderTargetOpenGL(int w, int h, GLuint *pFrameBuffer,
								 GLuint color_fmt, GLuint *pFrameTexture, int num_mrts,
								 GLuint depth_fmt, GLuint *pDepthTexture)
{
	GLuint framebuffer =  0;
	GLuint frametexture = 0;
	GLuint depthtexture = 0;

	glGenFramebuffersEXT(1, &framebuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

	if ( pFrameTexture )
	{
		for ( int i=0; i<num_mrts; i++ )
		{
			// create a framebuffer for rendering
			glGenTextures(1, &frametexture);
			glBindTexture(GL_TEXTURE_2D, frametexture);
			// default filter
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// set filter
			if ( color_fmt==GL_RGBA32F_ARB || color_fmt==GL_RGBA16F_ARB )
			{
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else
			{
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			// size and format
			glTexImage2D(GL_TEXTURE_2D, 0, color_fmt,  w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			// bind framebuffer and frame texture
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, GL_TEXTURE_2D, frametexture, 0);

			pFrameTexture[i] = frametexture;
		}
	}
	else
	{
		glDrawBuffer(FALSE);
		glReadBuffer(FALSE);
	}

	if ( pDepthTexture )
	{
		// almost the same as above
		glGenTextures(1, &depthtexture);
		glBindTexture(GL_TEXTURE_2D, depthtexture);
		
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// it's a depth texture
		glTexImage2D(GL_TEXTURE_2D, 0, depth_fmt,  w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthtexture, 0);

		*pDepthTexture = depthtexture;
	}
	// check if success
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if ( status!=GL_FRAMEBUFFER_COMPLETE_EXT )
	{
		return false;
	}

	*pFrameBuffer = framebuffer;

	return true;
}

bool  Device_opengl::CreateRenderTargetOpenGL(int w, int h, GLuint *pFrameBuffer, 
								 GLuint color_fmt, GLuint *pFrameTexture,
								 GLuint depth_fmt, GLuint *pDepthTexture)
{
	return CreateRenderTargetOpenGL(w, h, pFrameBuffer, color_fmt, pFrameTexture, 1, depth_fmt, pDepthTexture);
}

#endif
