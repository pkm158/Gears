/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_DLFin_opengl.h"
#include "UtilityManager.h"
#include "RenderPipeManager.h"

RenderPipe_DLFin_opengl::RenderPipe_DLFin_opengl(void)
{
}

RenderPipe_DLFin_opengl::~RenderPipe_DLFin_opengl(void)
{
	release();
}

bool RenderPipe_DLFin_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	CGutModel sphere;

	sphere.Load_ASCII(".\\Resources\\sphere_simple.gma");
	m_sphere_OpenGL.ConvertToOpenGLModel(&sphere);

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".\\Src\\Shader\\DL_final.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".\\Src\\Shader\\DL_final.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);


	

	m_pipe_img = dynamic_cast<RenderPipe_img_opengl *>(GetRenderPipe(PIPE_IMG));
	return true;
}

void RenderPipe_DLFin_opengl::render( void )
{
	render(0);
}

void RenderPipe_DLFin_opengl::render( GLuint target )
{
	m_pipe_img->setArgs(m_texture[BUFFER_PRE_RENDER]);
	m_pipe_img->render(target);

	glDepthMask(GL_FALSE);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &(GetProjectionMatrix()) );

	CMatrix view_matrix = GetViewMatrix();

	glUseProgram(m_pipeProgramID);

	GLint pos_sampler = glGetUniformLocation(m_pipeProgramID, "u_posSampler");
	GLint normalmap_sampler = glGetUniformLocation(m_pipeProgramID, "u_normalmapSampler");
	GLint diff_sampler = glGetUniformLocation(m_pipeProgramID, "u_diffuseSampler");
	GLint spec_sampler = glGetUniformLocation(m_pipeProgramID, "u_specularSampler");
	GLint shin_sampler = glGetUniformLocation(m_pipeProgramID, "u_shininessSampler");


	GLint lightpos_reg = glGetUniformLocation(m_pipeProgramID, "u_lightPos");
	GLint lightDiffuseColor_reg = glGetUniformLocation(m_pipeProgramID, "u_diffuseColor");
	GLint lightSpecColor_reg = glGetUniformLocation(m_pipeProgramID, "u_specColor");
	GLint camerapos_reg = glGetUniformLocation(m_pipeProgramID, "u_cameraPos");

	CVector camPos = GetCamPos();
	if (camerapos_reg >= 0)
		glUniform4fv(camerapos_reg, 1, (float *)&camPos);

	
	if (pos_sampler >= 0)
	{
		glActiveTexture(GL_TEXTURE0_ARB + BUFFER_POS);
		glUniform1i(pos_sampler, BUFFER_POS);
		glBindTexture(GL_TEXTURE_2D, m_texture[BUFFER_POS]);
	}
	
	if (normalmap_sampler >= 0)
	{
		glActiveTexture(GL_TEXTURE0_ARB + BUFFER_NORMAL);
		glUniform1i(normalmap_sampler, BUFFER_NORMAL);
		glBindTexture(GL_TEXTURE_2D, m_texture[BUFFER_NORMAL]);
	}
	if (diff_sampler >= 0)
	{
		glActiveTexture(GL_TEXTURE0_ARB + BUFFER_DIFFUSE);
		glUniform1i(diff_sampler, BUFFER_DIFFUSE);
		glBindTexture(GL_TEXTURE_2D, m_texture[BUFFER_DIFFUSE]);
	}
	if (spec_sampler >= 0)
	{
		glActiveTexture(GL_TEXTURE0_ARB + BUFFER_SPECULAR);
		glUniform1i(spec_sampler, BUFFER_SPECULAR);
		glBindTexture(GL_TEXTURE_2D, m_texture[BUFFER_SPECULAR]);
	}
	if (shin_sampler >= 0)
	{
		glActiveTexture(GL_TEXTURE0_ARB + BUFFER_SHININESS);
		glUniform1i(shin_sampler, BUFFER_SHININESS);
		glBindTexture(GL_TEXTURE_2D, m_texture[BUFFER_SHININESS]);
	}
	glActiveTexture(GL_TEXTURE0_ARB);

	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_ONE, GL_ONE);

	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_CULL_FACE);

	LightList lightList = GetLightManager().getLights();

	for (LightList::iterator it=lightList.begin(); it!=lightList.end(); ++it)
	{
		Light *pLight = *it;

		if (lightpos_reg >= 0)
			glUniform4fv(lightpos_reg, 1, (float *)&pLight->position);
		if (lightDiffuseColor_reg >= 0)
			glUniform4fv(lightDiffuseColor_reg, 1, (float *)&pLight->diffuse);
		if (lightSpecColor_reg >= 0)
			glUniform4fv(lightSpecColor_reg, 1, (float *)&pLight->specular);


		CMatrix world_matrix;
		world_matrix.Identity();

		world_matrix[0][0] = world_matrix[1][1] = world_matrix[2][2] = pLight->position.w;
		world_matrix[3][0] = pLight->position.x;
		world_matrix[3][1] = pLight->position.y;
		world_matrix[3][2] = pLight->position.z;
		world_matrix[3][3] = 1.0f;

		CMatrix wv_matrix = world_matrix * view_matrix;

		glLoadMatrixf( (float*) &wv_matrix);

		m_sphere_OpenGL.Render();
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	glUseProgram(0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void RenderPipe_DLFin_opengl::release( void )
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

void RenderPipe_DLFin_opengl::setArgs( GLuint *texture )
{
	m_texture = texture;
}