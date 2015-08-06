/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_DLEmiss_opengl.h"
#include "UtilityManager.h"

RenderPipe_DLEmiss_opengl::RenderPipe_DLEmiss_opengl(void) :
													m_isDepthBufferAltered(false)
{
}

RenderPipe_DLEmiss_opengl::~RenderPipe_DLEmiss_opengl(void)
{
	release();
}

bool RenderPipe_DLEmiss_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".\\Src\\Shader\\SimpleDiffuse.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".\\Src\\Shader\\DL_emission.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);

	return true;
}

void RenderPipe_DLEmiss_opengl::release( void )
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

void RenderPipe_DLEmiss_opengl::render( void )
{
	render(0);
}

void RenderPipe_DLEmiss_opengl::render( GLuint target )
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
	glLoadMatrixf( (float *) &(GetProjectionMatrix()));
	CMatrix view_matrix = GetViewMatrix();

	GLuint regTexture = glGetUniformLocation(m_pipeProgramID, "u_texture");
	glUniform1i(regTexture, EMISSION_MAP_CHANNEL);


	m_listToRender = GetRenderEntityList(ENTITIES_OPAQUE_EMISSION);

	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_ONE, GL_ONE);

	for (RenderEntityList::const_iterator it=m_listToRender.begin(); it!=m_listToRender.end(); ++it)
	{
		CMatrix worldMat = (*it)->getWorldMat();

		glMatrixMode(GL_MODELVIEW);
		CMatrix worldlMatView = worldMat * view_matrix;
		glLoadMatrixf( (float *) &worldlMatView);

		(*it)->render();
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glUseProgram(0);
}

void RenderPipe_DLEmiss_opengl::setArgs( GLuint depthBuffer)
{

}