/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPE_BLOOM_OPENGL
#define RENDERPIPE_BLOOM_OPENGL

#include "IRenderpipe.h"
#include "RenderPipe_brightness_opengl.h"
#include "RenderPipe_blur_opengl.h"
#include "RenderPipe_imgBlend_opengl.h"
#include "RenderPipe_img_opengl.h"
#include "RenderPipe_simple_opengl.h"
#include "RenderPipe_depth_opengl.h"

class RenderPipe_bloom_opengl :
	public IRenderPipe
{
public:
	RenderPipe_bloom_opengl(void);
	virtual ~RenderPipe_bloom_opengl(void);

	bool init(Device_opengl *device);
	void release(void);
	void render(void);
	void render(GLuint target);

	/************************************************************************/
	/* canvasTexture, the texture you have drawn main scene on it
	   canvasBuffer, to write into canvasTexture
	   shininessTexture, the texture you want it to be bloom, for example, a sun
	   shininessBuffer, to write into shininessTexture
	   depthTexture, recorded depth buffer, for depth test
	   scale and offset, for brightness pipe, new_color = (old_color + offset) * scale
	  */
	/************************************************************************/
	void setArgs(GLuint canvasTexture, GLuint canvasBuffer, GLuint shininessTexture, GLuint shininessBuffer,
					GLuint depthTexture, const CVector &scale, const CVector &offset);
	void setArgs(GLuint canvasTexture, GLuint canvasBuffer, GLuint bloomTexture, GLuint bloomBuffer, GLuint depthTexture);


private:
	RenderPipe_brightness_opengl *m_pipe_brightness;
	RenderPipe_blur_opengl		 *m_pipe_blur;
	RenderPipe_imgBlend_opengl	 *m_pipe_imgBlend;
	RenderPipe_img_opengl		 *m_pipe_img;
	RenderPipe_simple_opengl	 *m_pipe_simpleDiff;
	RenderPipe_depth_opengl		 *m_pipe_depthOfField;

	int m_width;
	int m_height;
	GLuint m_brightBufferID[2];
	GLuint m_brightTextureID[2];

	GLuint m_canvasTexture;
	GLuint m_canvasBuffer;
	GLuint m_shininessTexture;
	GLuint m_shininessBuffer;
	GLuint m_depthTexure;

};

#endif
