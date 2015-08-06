/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPE_SIMPLE_OPENGL
#define RENDERPIPE_SIMPLE_OPENGL

#include "IRenderPipe.h"

enum 
{
	TYPE_REGULAR = 0,
	TYPE_EMISSION
};

class RenderPipe_simple_opengl : public IRenderPipe
{
public:
	RenderPipe_simple_opengl(void);
	virtual ~RenderPipe_simple_opengl(void);

	bool init(Device_opengl *device);
	void release(void);
	void render(void);
	void render(GLuint target);
	void setArgs(GLuint depthBuffer, int type);

private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;

	GLuint m_depthBuffer;
	bool m_isDepthBufferAltered;
	int m_type;

};

#endif