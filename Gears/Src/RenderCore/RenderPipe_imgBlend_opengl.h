/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPE_IMGBLEND_OPENGL
#define RENDERPIPE_IMGBLEND_OPENGL

#include "IRenderpipe.h"

class RenderPipe_imgBlend_opengl :
	public IRenderPipe
{
public:
	RenderPipe_imgBlend_opengl(void);
	virtual ~RenderPipe_imgBlend_opengl(void);

	bool init(Device_opengl *device);
	void release(void);
	void render(void);
	void render(GLuint target);

	void setArgs( GLuint texture1, GLuint texture2, float intensityScale1, float intensityScale2);
	void setArgs( GLuint texture1, GLuint texture2);

private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;

	GLuint  m_texture[2];
	float	m_intensityScale1;
	float	m_intensityScale2;
};

#endif

