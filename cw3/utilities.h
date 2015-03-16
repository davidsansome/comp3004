#ifndef UTILITIES_H
#define UTILITIES_H

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#endif

#include <string>
#include <istream>

#include <svl/SVL.h>

#include "glwin.h"

typedef unsigned char uint8;
typedef unsigned char uchar;
typedef unsigned int uint32;
typedef unsigned int uint;

using namespace std;

namespace Utilities
{
	void fatal(const string& message);
	
	void normalizeVector(float* v);
	float interpolate(float tl, float tr, float bl, float br, float weightX, float weightY, float cWeightX, float cWeightY);
	float clamp(float v, float lower, float upper);
	float sign(float v);
	
	template<class T>
	T read(istream& stream)
	{
		T ret;
		stream.read((char*) &ret, sizeof(T));
		return ret;
	}
	
	template<int N>
	Vec readVec(istream& stream)
	{
		Vec ret(N);
		for (int i=0 ; i<N ; ++i)
			ret[i] = read<float>(stream);
		return ret;
	}
	
	string readString(istream& stream);
};

#endif
