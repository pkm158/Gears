/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPE_DIFFBLEND_OPENGL
#define RENDERPIPE_DIFFBLEND_OPENGL

#include "IRenderpipe.h"

class RenderPipe_DLDiffBlend_opengl :
	public IRenderPipe
{
public:
	RenderPipe_DLDiffBlend_opengl(void);
	virtual ~RenderPipe_DLDiffBlend_opengl(void);

	bool init(Device_opengl *device);
	void release(void);
	void render(void);
	void render(GLuint target);
	void setArgs(GLuint depthBuffer, GLuint ambDiffTex);

private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;

	GLuint m_depthBuffer;
	GLuint m_ambDiffTex;

	bool m_isDepthBufferAltered;

	void renderBlend(int textureLevel, GLuint regScale, CMatrix & viewMatrix);
};

#endif