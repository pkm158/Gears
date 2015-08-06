/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPE_DLFIN_OPENGL
#define RENDERPIPE_DLFIN_OPENGL

#include "IRenderpipe.h"
#include "GutModel_OpenGL.h"
#include "RenderPipe_img_opengl.h"

class RenderPipe_DLFin_opengl :
	public IRenderPipe
{
public:
	RenderPipe_DLFin_opengl(void);
	virtual ~RenderPipe_DLFin_opengl(void);

	bool init(Device_opengl *device);
	void release(void);

	void render(void);
	void render(GLuint target);
	void setArgs(GLuint *texture);

private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;

	GLuint *m_texture;

	CGutModel_OpenGL m_sphere_OpenGL;

	RenderPipe_img_opengl *m_pipe_img;
};

#endif