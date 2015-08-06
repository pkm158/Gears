/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "RenderPipe_depth_opengl.h"
#include "UtilityManager.h"

RenderPipe_depth_opengl::RenderPipe_depth_opengl(void)
{

}

RenderPipe_depth_opengl::~RenderPipe_depth_opengl(void)
{
	release();
}

bool RenderPipe_depth_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".\\Src\\Shader\\Image.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".\\Src\\Shader\\Depth_of_field.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);

	return true;
}

void RenderPipe_depth_opengl::release( void )
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

void RenderPipe_depth_opengl::render( void )
{
	render(0);
}

void RenderPipe_depth_opengl::render( GLuint target )
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

	GLuint reg = glGetUniformLocation(m_pipeProgramID, "u_image");
	glUniform1i(reg, 0);
	reg = glGetUniformLocation(m_pipeProgramID, "u_blurredImage");
	glUniform1i(reg, 1);
	reg = glGetUniformLocation(m_pipeProgramID, "u_zBuffer");
	glUniform1i(reg, 2);

	CMatrix inverProjMat = GetProjectionMatrix();
	inverProjMat.Invert();

	m_zInv = inverProjMat.GetColumn(3);	// only need to calculate z value, so only 4th column is needed.

	m_depthOfField = GetDepthOfFieldFactors();

	reg = glGetUniformLocation(m_pipeProgramID, "u_depthOfField");
	if (reg >= 0)
		glUniform4fv(reg, 1, (float *)&m_depthOfField);
	if (reg >= 0)
		reg = glGetUniformLocation(m_pipeProgramID, "u_zInv");
	glUniform4fv(reg, 1, (float *)&m_zInv);


	glActiveTexture(GL_TEXTURE0_ARB);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, m_image);

	glActiveTexture(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, m_blurredImage);

	glActiveTexture(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glActiveTexture(GL_TEXTURE0_ARB);

	renderScreenQuad();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDepthMask(GL_TRUE);
}

void RenderPipe_depth_opengl::setArgs( GLuint image, GLuint blurredImage, GLuint depthTexture )
{
	m_image = image;
	m_blurredImage = blurredImage;
	m_depthTexture = depthTexture;
}