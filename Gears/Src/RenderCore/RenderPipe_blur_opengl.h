/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef RENDERPIPE_BLUR_OPENGL
#define RENDERPIPE_BLUR_OPENGL

#include "IRenderpipe.h"

#define NUM_SAMPLE 9

class RenderPipe_blur_opengl :
	public IRenderPipe
{
public:
	RenderPipe_blur_opengl(void);
	virtual ~RenderPipe_blur_opengl(void);


	bool init(Device_opengl *device);
	void release(void);
	void render(void);
	void render(GLuint target);

	/************************************************************************/
	/* blurLevel, BLUR_INTENSE_L1 means slightly blur, BLUR_INTENSE_L2 means heavily blur
	   texture, the texture is going to be blurred.
	*/
	/************************************************************************/
	void setArgs( BLUR_LEVEL blurLevel, GLuint texture);

private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;
	GLuint   m_texture;

	GLuint m_interTextureID[2];
	GLuint m_interBufferID[2];

	GLuint m_blurLevel;

	int m_bufferWidth;
	int m_bufferHeight;

	float m_weight[NUM_SAMPLE];
	float m_uvOffset[NUM_SAMPLE];
};


#endif
