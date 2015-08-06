/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef IRENDERPIPE
#define IRENDERPIPE

#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include "Device_opengl.h"
#include "RenderEntityManager.h"

class IRenderPipe
{
public:
	enum {
		BUFFER_BLOOM = 0,
		BUFFER_PRE_RENDER,
		BUFFER_POS,
		BUFFER_NORMAL,
		BUFFER_DIFFUSE,
		BUFFER_SPECULAR,
		BUFFER_SHININESS,
		BUFFER_AMB_DIFF,
		BUFFER_NUM
	};

	enum BLUR_LEVEL{
		BLUR_L1 = 0,
		BLUR_L2,
		BLUR_NUM
	};

	enum BLUR_INTENSE_LEVEL{
		BLUR_INTENSE_L1 = 4,
		BLUR_INTENSE_L2 = 6
	};

	IRenderPipe(void);
	virtual ~IRenderPipe(void);
	virtual void render(void) = NULL;

#ifdef OPENGL_CORE
	virtual bool init(Device_opengl *device) = NULL;
	virtual void render(GLuint target) = NULL; // render to a texture, target is the buffer associated with the texture
	virtual void release() = NULL;		// render to screen
#endif

	virtual void setEntitiesToRender(const RenderEntityList& list ){ m_listToRender = list; };	// if a render pipe get entities from RenderEntityManager, this function won't work

protected:
	Device_opengl *m_pDevice;
	RenderEntityList m_listToRender;

#ifdef OPENGL_CORE
	GLuint loadVertexShaderOpenGL_GLSL(const char *filename);
	GLuint loadFragmentShaderOpenGL_GLSL(const char *filename);

	GLuint loadGLSLShader(const char *filename, GLenum type);

	virtual void clear(GLuint target);
	virtual void clearColorBuffer(GLuint target);
	virtual void clearDepthBuffer(GLuint target);
	virtual void renderScreenQuad(void);
#endif
};
#endif