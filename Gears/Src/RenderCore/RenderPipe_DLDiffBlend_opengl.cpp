/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_DLDiffBlend_opengl.h"
#include "UtilityManager.h"

RenderPipe_DLDiffBlend_opengl::RenderPipe_DLDiffBlend_opengl(void) :
															m_isDepthBufferAltered(false)
{
}

RenderPipe_DLDiffBlend_opengl::~RenderPipe_DLDiffBlend_opengl(void)
{
	release();
}

bool RenderPipe_DLDiffBlend_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".\\Src\\Shader\\SimpleDiffuse.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".\\Src\\Shader\\DL_pre_diffBlend.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);

	return true;
}

void RenderPipe_DLDiffBlend_opengl::release( void )
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

void RenderPipe_DLDiffBlend_opengl::render( void )
{
	render(0);
}

void RenderPipe_DLDiffBlend_opengl::render( GLuint target )
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);
	glDepthMask(GL_FALSE);

	if (m_isDepthBufferAltered)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depthBuffer, 0);
		m_isDepthBufferAltered = false;
	}
	glUseProgram(m_pipeProgramID);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &GetProjectionMatrix());
	CMatrix view_matrix = GetViewMatrix();
	GLuint reg = glGetUniformLocation(m_pipeProgramID, "u_ambDiffSample");

	if (reg >= 0)
	{
		glActiveTexture(GL_TEXTURE0_ARB + NOTSPECIALFIED_CHANNEL);
		glUniform1i(reg, NOTSPECIALFIED_CHANNEL);
		glBindTexture(GL_TEXTURE_2D, m_ambDiffTex);
		glActiveTexture(GL_TEXTURE0_ARB);
	}

	GLuint regDiffScale = glGetUniformLocation(m_pipeProgramID, "u_diffScale");

	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_ONE, GL_ONE);

	renderBlend(1, regDiffScale, view_matrix);
	renderBlend(2, regDiffScale, view_matrix);

	glDisable(GL_BLEND);

	glDepthMask(GL_TRUE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glUseProgram(0);
}

void RenderPipe_DLDiffBlend_opengl::setArgs( GLuint depthBuffer, GLuint ambDiffTex )
{
	if (depthBuffer != m_depthBuffer)
	{
		m_depthBuffer = depthBuffer;
		m_isDepthBufferAltered = true;
	}

	m_ambDiffTex = ambDiffTex;

}

void RenderPipe_DLDiffBlend_opengl::renderBlend( int textureLevel, GLuint regScale, CMatrix & viewMatrix )
{
	GLuint regTexture = glGetUniformLocation(m_pipeProgramID, "u_texture");
	if (regTexture >= 0)
		glUniform1i(regTexture, textureLevel);

	switch (textureLevel)
	{
	case 1:
		{
			m_listToRender = GetRenderEntityList(ENTITIES_OPAQUE_2ND_DIFFUSE);
		} break;
	case 2:
		{
			m_listToRender = GetRenderEntityList(ENTITIES_OPAQUE_3RD_DIFFUSE);
		} break;
	default:
		HR(false);
		break;
	}
	
	for (RenderEntityList::const_iterator it=m_listToRender.begin(); it!=m_listToRender.end(); ++it)
	{
		CMatrix worldMat = (*it)->getWorldMat();

		glMatrixMode(GL_MODELVIEW);
		CMatrix worldlMatView = worldMat * viewMatrix;
		glLoadMatrixf( (float *) &worldlMatView);

		if (regScale >= 0)
			glUniform1f(regScale, (*it)->getDiffuseMapScale(textureLevel));

		(*it)->render();
	}

}