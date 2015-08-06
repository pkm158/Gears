/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef DEVICE_OPENGL
#define DEVICE_OPENGL

#ifdef OPENGL_CORE

#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include "IReset.h"

struct DirLight
{
	union{
		struct
		{
			CVector ambient;
			CVector diffuse;
			CVector spec;
			CVector dirW;
		};
		float m[4][4];
	};

};

class Device_opengl : public IReset
{
public:
	Device_opengl( HWND hwnd );
	virtual ~Device_opengl( void );

	bool initOpengl( void );
	virtual bool reset( void );

	HDC	getHDC( void ) { return m_hDC; };

	static bool CreateRenderTargetOpenGL(int w, int h,  GLuint color_fmt, GLuint *framebuffer, GLuint *texture);
	static bool CreateRenderTargetOpenGL(int w, int h, GLuint *framebuffer, 
		GLuint color_fmt, GLuint *frametexture,
		GLuint depth_fmt, GLuint *depthtexture);
	static bool CreateRenderTargetOpenGL(int w, int h, GLuint *pFrameBuffer,
		GLuint color_fmt, GLuint *pFrameTexture, int num_mrts,
		GLuint depth_fmt, GLuint *pDepthTexture);
private:
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hGLRC;

	bool initOpenglResource(void);
};

#endif

#endif
