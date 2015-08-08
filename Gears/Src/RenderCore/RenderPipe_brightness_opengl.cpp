/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "RenderPipe_brightness_opengl.h"
#include "UtilityManager.h"

RenderPipe_brightness_opengl::RenderPipe_brightness_opengl(void):
															m_brightnessOffset(CVector(-0.0f, 0.0f, 0.0f, 0.0f)),
															m_brightnessScale(CVector(3.0f, 3.0f, 3.0f, 3.0f))
{
}

RenderPipe_brightness_opengl::~RenderPipe_brightness_opengl(void)
{
	release();
}

bool RenderPipe_brightness_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".\\Src\\Shader\\bloom.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".\\Src\\Shader\\brightness.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);

	return true;
}

void RenderPipe_brightness_opengl::release( void )
{
	if ( m_vertexProgramID )
	{
		glDeleteShader(m_vertexProgramID);
		m_vertexProgramID = NULL;
	}

	if ( m_fragmentProgramID )
	{
		glDeleteShader(m_fragmentProgramID);
		m_fragmentProgramID = NULL;
	}

	if ( m_pipeProgramID )
	{
		glDeleteProgram(m_pipeProgramID);
		m_pipeProgramID = NULL;
	}
}

void RenderPipe_brightness_opengl::render( void )
{
	render(0);
}

void RenderPipe_brightness_opengl::render( GLuint target )
{
	glDepthMask(GL_FALSE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, m_width, m_height);

	glUseProgram(m_pipeProgramID);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	GLint regOffset = glGetUniformLocation(m_pipeProgramID, "u_intensityOffset");
	GLint regScale = glGetUniformLocation(m_pipeProgramID, "u_intensityScale");

	if (regOffset >= 0)
		glUniform4fv(regOffset, 1, (float *)&m_brightnessOffset);
	
	if (regScale >= 0)
		glUniform4fv(regScale, 1, (float *)&m_brightnessScale);

	renderScreenQuad();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glPopAttrib();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glDepthMask(GL_TRUE);

}

void RenderPipe_brightness_opengl::setArgs( int w, int h, GLuint texture, const CVector &scale, const CVector &offset )
{
	m_texture = texture;
	m_brightnessScale = scale;
	m_brightnessOffset = offset;
	m_width = w;
	m_height = h;
	
}

void RenderPipe_brightness_opengl::setArgs( int w, int h, GLuint texture )
{
	m_texture = texture;
	m_width = w;
	m_height = h;
}
