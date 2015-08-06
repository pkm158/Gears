/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "IRenderPipe.h"
#include "RenderEntityManager.h"
#include "UtilityManager.h"

IRenderPipe::IRenderPipe( void ) :
							m_pDevice( NULL )
{

}

IRenderPipe::~IRenderPipe( void )
{

}


#ifdef OPENGL_CORE
GLuint IRenderPipe::loadVertexShaderOpenGL_GLSL( const char *filename )
{
	return loadGLSLShader(filename, GL_VERTEX_SHADER);
}

GLuint IRenderPipe::loadFragmentShaderOpenGL_GLSL( const char *filename )
{
	return loadGLSLShader(filename, GL_FRAGMENT_SHADER);
}

GLuint IRenderPipe::loadGLSLShader( const char *filename, GLenum type )
{
	GLuint shader = glCreateShader(type);

	unsigned int len = 0;
	const GLchar *code = (const GLchar *)LoadBinaryStream(filename, &len);
	GLint size = len;
	glShaderSource(shader, 1, &code, &size);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if ( GL_TRUE!=result )
	{
		GLchar pMessage[2048];
		GLsizei msgLen = 1023;

		glGetShaderInfoLog(shader, size, &msgLen, pMessage);

		DEBUG_PRINT("%s error!!!\n", filename);
		DEBUG_PRINT("%s\n", pMessage);
		ALERT("%s compile error\n", filename);

		glDeleteShader(shader);
		shader = 0;
	}

	SAFE_DELETE_ARRAY(code);

	return shader;
}

void IRenderPipe::clear( GLuint target )
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void IRenderPipe::clearColorBuffer( GLuint target )
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void IRenderPipe::clearDepthBuffer( GLuint target )
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearStencil(0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void IRenderPipe::renderScreenQuad(void)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f); 

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();
}
#endif