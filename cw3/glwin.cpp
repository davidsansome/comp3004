#include "glwin.h"

#ifndef _WIN32
void setupWinGLFunctions()
{
}
#else

PFNGLACTIVETEXTUREPROC pglActiveTexture = NULL;
PFNGLMULTITEXCOORD2FPROC pglMultiTexCoord2f = NULL;
PFNGLBINDBUFFERPROC pglBindBuffer = NULL;
PFNGLDELETEBUFFERSPROC pglDeleteBuffers = NULL;
PFNGLGENBUFFERSPROC pglGenBuffers = NULL;
PFNGLBUFFERDATAPROC pglBufferData = NULL;
PFNGLUNIFORM1FPROC pglUniform1f = NULL;
PFNGLUNIFORM2FPROC pglUniform2f = NULL;
PFNGLUNIFORM3FPROC pglUniform3f = NULL;
PFNGLUNIFORM1IPROC pglUniform1i = NULL;
PFNGLCREATEPROGRAMPROC pglCreateProgram = NULL;
PFNGLLINKPROGRAMPROC pglLinkProgram = NULL;
PFNGLISSHADERPROC pglIsShader = NULL;
PFNGLISPROGRAMPROC pglIsProgram = NULL;
PFNGLDELETESHADERPROC pglDeleteShader = NULL;
PFNGLDELETEPROGRAMPROC pglDeleteProgram = NULL;
PFNGLCREATESHADERPROC pglCreateShader = NULL;
PFNGLCOMPILESHADERPROC pglCompileShader = NULL;
PFNGLGETSHADERIVPROC pglGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog = NULL;
PFNGLSHADERSOURCEPROC pglShaderSource = NULL;
PFNGLATTACHSHADERPROC pglAttachShader = NULL;
PFNGLUSEPROGRAMPROC pglUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation = NULL;
PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv = NULL;
PFNGLSTENCILOPSEPARATEPROC pglStencilOpSeparate = NULL;

void setupWinGLFunctions()
{
	pglActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress("glActiveTexture");
	pglMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC) wglGetProcAddress("glMultiTexCoord2f");
	pglBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer");
	pglDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress("glDeleteBuffers");
	pglGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
	pglBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");
	pglUniform1f = (PFNGLUNIFORM1FPROC) wglGetProcAddress("glUniform1f");
	pglUniform2f = (PFNGLUNIFORM2FPROC) wglGetProcAddress("glUniform2f");
	pglUniform3f = (PFNGLUNIFORM3FPROC) wglGetProcAddress("glUniform3f");
	pglUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
	pglCreateProgram = (PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram");
	pglLinkProgram = (PFNGLLINKPROGRAMPROC) wglGetProcAddress("glLinkProgram");
	pglIsShader = (PFNGLISSHADERPROC) wglGetProcAddress("glIsShader");
	pglIsProgram = (PFNGLISPROGRAMPROC) wglGetProcAddress("glIsProgram");
	pglDeleteShader = (PFNGLDELETESHADERPROC) wglGetProcAddress("glDeleteShader");
	pglDeleteProgram = (PFNGLDELETEPROGRAMPROC) wglGetProcAddress("glDeleteProgram");
	pglCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader");
	pglCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader");
	pglGetShaderiv = (PFNGLGETSHADERIVPROC) wglGetProcAddress("glGetShaderiv");
	pglGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress("glGetShaderInfoLog");
	pglShaderSource = (PFNGLSHADERSOURCEPROC) wglGetProcAddress("glShaderSource");
	pglAttachShader = (PFNGLATTACHSHADERPROC) wglGetProcAddress("glAttachShader");
	pglUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
	pglGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");
	pglUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");
	pglStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC) wglGetProcAddress("glStencilOpSeparate");
}

#endif
