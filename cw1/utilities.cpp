#include "utilities.h"

#include <math.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// Returns a normalized vector (length = 1)
float3 Utilities::normalize(float3 v)
{
	float len = 0.0f;
	for (int i=0 ; i<3 ; ++i)
		len += v[i]*v[i];
	len = sqrt(len);
	
	float3 ret;
	for (int i=0 ; i<3 ; ++i)
		ret[i] = v[i] / len;
	return ret;
}

// Returns the dot product between a and b
float Utilities::dot(float3 a, float3 b)
{
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

// Clamps value to the range min-max
float Utilities::clamp(float min, float value, float max)
{
	return fminf(fmaxf(min, value), max);
}

// Transforms the vector v by the transpose of the current
// modelview matrix.  This is useful for converting from world space
// to object space
float3 Utilities::multMatrixTrans(float3 v)
{
	// Store the current matrix as we're about to mess with it
	glPushMatrix();
	
	// Get the current matrix
	float origMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, origMatrix);
	
	// Transpose it
	transpose(origMatrix);
	
	// Load this transpose back into the GL modelview matrix
	glLoadMatrixf(origMatrix);
	
	// Loads our vector into the final column of a matrix.
	// This is column-major so it looks funny in C
	float matrix[16] =
	{
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		v[0], v[1], v[2], 1.0
	};
	
	// Multiply the vector matrix by the modelview projection matrix
	// (which is now the transpose)
	glMultMatrixf(matrix);
	
	// Get the result out
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	
	// Get the vector out of the matrix - column-major again
	float3 ret = {{ matrix[12], matrix[13], matrix[14] }};
	
	// Restore the original modelview matrix
	glPopMatrix();
	
	return ret;
}

// Transforms the vector v by the inverse transpose of the current
// modelview matrix.  This is useful for converting from object space
// to world space
float3 Utilities::multMatrixInvTrans(float3 v)
{
	// Store the current matrix as we're about to mess with it
	glPushMatrix();
	
	// Get the current matrix
	float origMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, origMatrix);
	
	// Transpose it
	transpose(origMatrix);
	
	// Calculate its inverse
	float inverse[16];
	invert2(origMatrix, inverse);
	
	// Load this inverse-transpose back into the GL modelview matrix
	glLoadMatrixf(inverse);
	
	// Loads our vector into the final column of a matrix.
	// This is column-major so it looks funny in C
	float matrix[16] =
	{
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		v[0], v[1], v[2], 1.0
	};
	
	// Multiply the vector matrix by the modelview projection matrix
	// (which is now the inverse-transpose)
	glMultMatrixf(matrix);
	
	// Get the result out
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	
	// Get the vector out of the matrix - column-major again
	float3 ret = {{ matrix[12], matrix[13], matrix[14] }};
	
	// Restore the original modelview matrix
	glPopMatrix();
	
	return ret;
}

// Inverts a matrix.
// This was taken from a paper by Intel:
//   "Streaming SIMD Extensions - Inverse of 4x4 Matrix"
//   http://download.intel.com/design/PentiumIII/sml/24504301.pdf
void Utilities::invert2(float* mat, float* dst) {
        float    tmp[12]; /* temp array for pairs                      */ 
    float    src[16]; /* array of transpose source matrix */ 
    float    det;     /* determinant                                  */ 
    /* transpose matrix */ 
    for (int i = 0; i < 4; i++) { 
        src[i]        = mat[i*4]; 
        src[i + 4]    = mat[i*4 + 1]; 
        src[i + 8]    = mat[i*4 + 2]; 
        src[i + 12]   = mat[i*4 + 3]; 
    } 
    /* calculate pairs for first 8 elements (cofactors) */ 
    tmp[0]  = src[10] * src[15]; 
    tmp[1]  = src[11] * src[14]; 
    tmp[2]  = src[9]  * src[15]; 
    tmp[3]  = src[11] * src[13]; 
    tmp[4]  = src[9]  * src[14]; 
    tmp[5]  = src[10] * src[13]; 
    tmp[6]  = src[8]  * src[15]; 
    tmp[7]  = src[11] * src[12]; 
    tmp[8]  = src[8]  * src[14]; 
    tmp[9]  = src[10] * src[12]; 
    tmp[10] = src[8]  * src[13]; 
    tmp[11] = src[9]  * src[12]; 
    /* calculate first 8 elements (cofactors) */ 
    dst[0]  = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7]; 
    dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7]; 
    dst[1]  = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7]; 
    dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7]; 
    dst[2]  = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7]; 
    dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7]; 
    dst[3]  = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6]; 
    dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6]; 
    dst[4]  = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3]; 
    dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3]; 
    dst[5]  = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3]; 
    dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3]; 
    dst[6]  = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3]; 
    dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3]; 
    dst[7]  = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2]; 
    dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2]; 
    /* calculate pairs for second 8 elements (cofactors) */ 
    tmp[0]  = src[2]*src[7]; 
    tmp[1]  = src[3]*src[6]; 
    tmp[2]  = src[1]*src[7]; 
    tmp[3]  = src[3]*src[5]; 
    tmp[4]  = src[1]*src[6]; 
    tmp[5]  = src[2]*src[5]; 
    tmp[6]  = src[0]*src[7]; 
    tmp[7]  = src[3]*src[4]; 
    tmp[8]  = src[0]*src[6]; 
    tmp[9]  = src[2]*src[4]; 
    tmp[10] = src[0]*src[5]; 
    tmp[11] = src[1]*src[4]; 
    /* calculate second 8 elements (cofactors) */ 
    dst[8]  = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15]; 
    dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15]; 
    dst[9]  = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15]; 
    dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15]; 
    dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15]; 
    dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15]; 
    dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14]; 
    dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14]; 
    dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9]; 
    dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10]; 
    dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10]; 
    dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8]; 
    dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8]; 
    dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9]; 
    dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9]; 
    dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8]; 
    /* calculate determinant */ 
    det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3]; 
    /* calculate matrix inverse */ 
    det = 1/det; 
    for (int j = 0; j < 16; j++) 
        dst[j] *= det; 
}

// Transposes the matrix m
void Utilities::transpose(float* m)
{
	float n[16] =
	{
		m[0], m[4], m[8], m[12],
		m[1], m[5], m[9], m[13],
		m[2], m[6], m[10], m[14],
		m[3], m[7], m[11], m[15]
	};
	
	memcpy(m, n, sizeof(float)*16);
}
