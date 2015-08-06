/************************************************************************/
/* Author: unknown, found it online, modified by Kemi Peng				
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef _GUTTEXTURE_OPENGL_
#define _GUTTEXTURE_OPENGL_

#include "GutImage.h"

GLuint GutLoadTexture_OpenGL(const char *filename, sImageInfo *pInfo=NULL, bool mipmap_enabled = true);
GLuint GutLoadCubemapTexture_OpenGL(const char **filename_array, bool mipmap_enabled = true);
GLuint GutLoadCubemapTexture_OpenGL(const char *filename);
GLuint GutLoadVolumeTexture_OpenGL(const char *filename);

#endif // _GUTTEXTURE_OPENGL_