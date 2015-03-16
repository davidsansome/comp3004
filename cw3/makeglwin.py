functions = [
	"glActiveTexture",
	"glMultiTexCoord2f",
	"glBindBuffer",
	"glDeleteBuffers",
	"glGenBuffers",
	"glBufferData",
	"glUniform1f",
	"glUniform2f",
	"glUniform3f",
	"glUniform1i",
	"glCreateProgram",
	"glLinkProgram",
	"glIsShader",
	"glIsProgram",
	"glDeleteShader",
	"glDeleteProgram",
	"glCreateShader",
	"glCompileShader",
	"glGetShaderiv",
	"glGetShaderInfoLog",
	"glShaderSource",
	"glAttachShader",
	"glUseProgram",
	"glGetUniformLocation",
	"glUniformMatrix4fv",
	"glStencilOpSeparate"
]

h = open("glwin.h", "w")

h.write("""#ifndef GLWIN_H
#define GLWIN_H

void setupWinGLFunctions();

#ifdef _WIN32

#include <windows.h>
#include <gl/gl.h>
#include <gl/glext.h>

""")

for function in functions:
	h.write("extern PFN%sPROC p%s;\n" % (function.upper(), function))

h.write("\n")

for function in functions:
	h.write("#define %s p%s\n" % (function, function))

h.write("""
#endif
#endif
""")

h.close()




cpp = open("glwin.cpp", "w")

cpp.write("""#include "glwin.h"

#ifndef _WIN32
void setupWinGLFunctions()
{
}
#else

""")

for function in functions:
	cpp.write("PFN%sPROC p%s = NULL;\n" % (function.upper(), function))

cpp.write("""
void setupWinGLFunctions()
{
""")

for function in functions:
	cpp.write("\tp%s = (PFN%sPROC) wglGetProcAddress(\"%s\");\n" % (function, function.upper(), function));

cpp.write("""}

#endif
""")

cpp.close()