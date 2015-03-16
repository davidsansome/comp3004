#ifndef GLWIN_H
#define GLWIN_H

void setupWinGLFunctions();

#ifdef _WIN32

#include <windows.h>
#include <gl/gl.h>
#include <gl/glext.h>

extern PFNGLACTIVETEXTUREPROC pglActiveTexture;
extern PFNGLMULTITEXCOORD2FPROC pglMultiTexCoord2f;
extern PFNGLBINDBUFFERPROC pglBindBuffer;
extern PFNGLDELETEBUFFERSPROC pglDeleteBuffers;
extern PFNGLGENBUFFERSPROC pglGenBuffers;
extern PFNGLBUFFERDATAPROC pglBufferData;
extern PFNGLUNIFORM1FPROC pglUniform1f;
extern PFNGLUNIFORM2FPROC pglUniform2f;
extern PFNGLUNIFORM3FPROC pglUniform3f;
extern PFNGLUNIFORM1IPROC pglUniform1i;
extern PFNGLCREATEPROGRAMPROC pglCreateProgram;
extern PFNGLLINKPROGRAMPROC pglLinkProgram;
extern PFNGLISSHADERPROC pglIsShader;
extern PFNGLISPROGRAMPROC pglIsProgram;
extern PFNGLDELETESHADERPROC pglDeleteShader;
extern PFNGLDELETEPROGRAMPROC pglDeleteProgram;
extern PFNGLCREATESHADERPROC pglCreateShader;
extern PFNGLCOMPILESHADERPROC pglCompileShader;
extern PFNGLGETSHADERIVPROC pglGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog;
extern PFNGLSHADERSOURCEPROC pglShaderSource;
extern PFNGLATTACHSHADERPROC pglAttachShader;
extern PFNGLUSEPROGRAMPROC pglUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv;
extern PFNGLSTENCILOPSEPARATEPROC pglStencilOpSeparate;

#define glActiveTexture pglActiveTexture
#define glMultiTexCoord2f pglMultiTexCoord2f
#define glBindBuffer pglBindBuffer
#define glDeleteBuffers pglDeleteBuffers
#define glGenBuffers pglGenBuffers
#define glBufferData pglBufferData
#define glUniform1f pglUniform1f
#define glUniform2f pglUniform2f
#define glUniform3f pglUniform3f
#define glUniform1i pglUniform1i
#define glCreateProgram pglCreateProgram
#define glLinkProgram pglLinkProgram
#define glIsShader pglIsShader
#define glIsProgram pglIsProgram
#define glDeleteShader pglDeleteShader
#define glDeleteProgram pglDeleteProgram
#define glCreateShader pglCreateShader
#define glCompileShader pglCompileShader
#define glGetShaderiv pglGetShaderiv
#define glGetShaderInfoLog pglGetShaderInfoLog
#define glShaderSource pglShaderSource
#define glAttachShader pglAttachShader
#define glUseProgram pglUseProgram
#define glGetUniformLocation pglGetUniformLocation
#define glUniformMatrix4fv pglUniformMatrix4fv
#define glStencilOpSeparate pglStencilOpSeparate

#endif
#endif
