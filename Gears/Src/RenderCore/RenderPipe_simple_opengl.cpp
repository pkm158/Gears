/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_simple_opengl.h"
#include "UtilityManager.h"

RenderPipe_simple_opengl::RenderPipe_simple_opengl(void) : m_isDepthBufferAltered(false)
{
}

RenderPipe_simple_opengl::~RenderPipe_simple_opengl(void)
{
	release();
}

bool RenderPipe_simple_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".\\Src\\Shader\\SimpleDiffuse.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".\\Src\\Shader\\SimpleDiffuse.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);

	return true;
}

void RenderPipe_simple_opengl::release( void )
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

void RenderPipe_simple_opengl::render( void )
{
	render(0);
}

void RenderPipe_simple_opengl::render( GLuint target )
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);
	
	if (m_isDepthBufferAltered)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depthBuffer, 0);
		m_isDepthBufferAltered = false;
	}
	glUseProgram(m_pipeProgramID);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &(GetProjectionMatrix()));
	CMatrix view_matrix = GetViewMatrix();

	GLuint reg = glGetUniformLocation(m_pipeProgramID, "myTexture");

	switch (m_type)
	{
	case TYPE_REGULAR:
		{
			glUniform1i(reg, DIFF0_MAP_CHANNEL);
		} break;
	case TYPE_EMISSION:
		{
			glUniform1i(reg, EMISSION_MAP_CHANNEL);
		} break;
	default:
		HR(false);
	}
	

	for (RenderEntityList::const_iterator it=m_listToRender.begin(); it!=m_listToRender.end(); ++it)
	{
		CMatrix worldMat = (*it)->getWorldMat();

		glMatrixMode(GL_MODELVIEW);
		CMatrix worldlMatView = worldMat * view_matrix;
		glLoadMatrixf( (float *) &worldlMatView);
		(*it)->render();
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glUseProgram(0);
}

void RenderPipe_simple_opengl::setArgs( GLuint depthBuffer, int type )
{
	if (depthBuffer != m_depthBuffer)
	{
		m_depthBuffer = depthBuffer;
		m_isDepthBufferAltered = true;
	}

	m_type = type;
	
}