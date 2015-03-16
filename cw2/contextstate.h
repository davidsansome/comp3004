#ifndef CONTEXTSTATE_H
#define CONTEXTSTATE_H

#include <svl/SVL.h>

class Texture;

class ContextState
{
public:
	ContextState();
	~ContextState();
	
	static ContextState* active() { return m_activeContext; }
	void makeActive() { m_activeContext = this; }
	
	enum MatrixType
	{
		ModelViewMatrix,
		ProjectionMatrix
	};
	
	enum Axis
	{
		XAxis,
		YAxis,
		ZAxis
	};
	
	enum PolygonMode
	{
		Fill,
		Line
	};
	
	void setMatrixMode(MatrixType type);
	void setPolygonMode(PolygonMode mode) { m_polygonMode = mode; }
	void bindTexture(Texture* tex) { m_texture = tex; }
	
	void loadIdentity();
	void frustum(float l, float r, float b, float t, float n, float f);
	void perspective(float fov, float aspect, float zNear, float zFar);
	void ortho(float l, float r, float b, float t, float n, float f);
	void translate(Vec3 d);
	void scale(Vec3 d);
	void rotate(float radians, Axis axis);
	
	const Mat4& modelViewMatrix() const { return m_modelView; }
	const Mat4& projectionMatrix() const { return m_projection; }
	const Texture* texture() const { return m_texture; }
	PolygonMode polygonMode() const { return m_polygonMode; }
	
private:
	Mat4 m_modelView;
	Mat4 m_projection;
	Mat4* m_activeMatrix;
	PolygonMode m_polygonMode;
	
	Texture* m_texture;

	static ContextState* m_activeContext;
};

#endif
