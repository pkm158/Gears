/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPE_DLEMISS_OPENGL
#define RENDERPIPE_DLEMISS_OPENGL

#include "IRenderpipe.h"

class RenderPipe_DLEmiss_opengl :
	public IRenderPipe
{
public:
	RenderPipe_DLEmiss_opengl(void);
	virtual ~RenderPipe_DLEmiss_opengl(void);

	bool init(Device_opengl *device);
	void release(void);
	void render(void);
	void render(GLuint target);
	void setArgs(GLuint depthBuffer);

private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;

	GLuint m_depthBuffer;

	bool m_isDepthBufferAltered;
};

#endif