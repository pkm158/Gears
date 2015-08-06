/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_img_opengl.h"

RenderPipe_img_opengl::RenderPipe_img_opengl(void)
{
}

RenderPipe_img_opengl::~RenderPipe_img_opengl(void)
{
	release();
}

bool RenderPipe_img_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".\\Src\\Shader\\Image.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".\\Src\\Shader\\Image.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);

	return true;

}

void RenderPipe_img_opengl::release( void )
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

void RenderPipe_img_opengl::render( void )
{
	render(0);
}

void RenderPipe_img_opengl::render( GLuint target )
{
	glDepthMask(GL_FALSE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glUseProgram(m_pipeProgramID);

	glActiveTexture(GL_TEXTURE0_ARB);
	glClientActiveTexture(GL_TEXTURE0_ARB);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	renderScreenQuad();
	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDepthMask(GL_TRUE);
}

void RenderPipe_img_opengl::setArgs( GLuint texture )
{
	m_texture = texture;
}