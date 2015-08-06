/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef RENDERPIPE_BRIGHTNESS_OPENGL
#define RENDERPIPE_BRIGHTNESS_OPENGL

#include "IRenderpipe.h"

class RenderPipe_brightness_opengl :
	public IRenderPipe
{
public:
	RenderPipe_brightness_opengl(void);
	virtual ~RenderPipe_brightness_opengl(void);

	bool init(Device_opengl *device);
	void release(void);
	void render(void);
	void render(GLuint target);
	
	//  new_color = (old_color + offset) * scale
	void setArgs( int w, int h, GLuint texture, const CVector &scale, const CVector &offset);
	void setArgs( int w, int h, GLuint texture );

private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;

	GLuint   m_texture;
	CVector m_brightnessScale;
	CVector m_brightnessOffset;

	int m_width;
	int m_height;
};

#endif