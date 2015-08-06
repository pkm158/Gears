/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef RENDERPIPE_DEPTH_OPENGL
#define RENDERPIPE_DEPTH_OPENGL

#include "IRenderPipe.h"

class RenderPipe_depth_opengl : public IRenderPipe
{
public:
	RenderPipe_depth_opengl(void);
	virtual ~RenderPipe_depth_opengl(void);

	bool init(Device_opengl *device);
	void release(void);
	void render(void);
	void render(GLuint target);

	void setArgs( GLuint image, GLuint blurredImage, GLuint depthTexture);


private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;

	GLuint m_image;
	GLuint m_blurredImage;
	GLuint m_depthTexture;

	CVector m_zInv;
	CVector m_depthOfField;
};

#endif