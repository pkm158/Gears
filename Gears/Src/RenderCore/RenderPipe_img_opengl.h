/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPE_DLAMB_OPENGL
#define RENDERPIPE_DLAMB_OPENGL

#include "IRenderPipe.h"

class RenderPipe_img_opengl :
	public IRenderPipe
{
public:
	RenderPipe_img_opengl(void);
	virtual ~RenderPipe_img_opengl(void);

	bool init(Device_opengl *device);
	void release(void);
	void render(void);
	void render(GLuint target);

	void setArgs( GLuint texture);

private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;

	GLuint   m_texture;
};

#endif