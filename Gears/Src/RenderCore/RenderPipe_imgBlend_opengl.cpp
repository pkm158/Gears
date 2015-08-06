/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_imgBlend_opengl.h"

RenderPipe_imgBlend_opengl::RenderPipe_imgBlend_opengl(void) : 
													m_intensityScale1(1),
													m_intensityScale2(1)
{
}

RenderPipe_imgBlend_opengl::~RenderPipe_imgBlend_opengl(void)
{
	release();
}

bool RenderPipe_imgBlend_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".\\Src\\Shader\\Image.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".\\Src\\Shader\\Image_blend.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);

	return true;
}

void RenderPipe_imgBlend_opengl::release( void )
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

void RenderPipe_imgBlend_opengl::render( void )
{
	render(0);
}

void RenderPipe_imgBlend_opengl::render( GLuint target )
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

	GLuint reg = glGetUniformLocation(m_pipeProgramID, "u_texture1");
	if (reg >= 0);
		glUniform1i(reg, 0);

	reg = glGetUniformLocation(m_pipeProgramID, "u_texture2");
	if (reg >= 0);
		glUniform1i(reg, 1);

	reg = glGetUniformLocation(m_pipeProgramID, "u_intensityScale1");
	if (reg >= 0);
		glUniform1f(reg, m_intensityScale1);

	reg = glGetUniformLocation(m_pipeProgramID, "u_intensityScale2");
	if (reg >= 0);
		glUniform1f(reg, m_intensityScale2);


	glActiveTexture(GL_TEXTURE0_ARB);
	glClientActiveTexture(GL_TEXTURE0_ARB);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);

	glActiveTexture(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	glActiveTexture(GL_TEXTURE0_ARB);

	renderScreenQuad();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDepthMask(GL_TRUE);
}

void RenderPipe_imgBlend_opengl::setArgs( GLuint texture1, GLuint texture2)
{
	m_texture[0] = texture1;
	m_texture[1] = texture2;

}

void RenderPipe_imgBlend_opengl::setArgs( GLuint texture1, GLuint texture2, float intensityScale1, float intensityScale2 )
{
	m_texture[0] = texture1;
	m_texture[1] = texture2;

	m_intensityScale1 = intensityScale1;
	m_intensityScale2 = intensityScale2;
}