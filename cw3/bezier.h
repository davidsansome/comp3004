#ifndef BEZIER_H
#define BEZIER_H

#include "utilities.h"

#include <vector>
#include <svl/SVL.h>

class Bezier
{
public:
	Bezier();
	~Bezier();
	
	void addPoint(const Vec3& point, const Vec3& control, float speed = 1.0);
	void finalize();
	
	bool updateProgress(float delta);
	void reset();
	
	Vec3 pointA() const;
	Vec3 pointB() const;
	Vec3 controlA() const;
	Vec3 controlB() const;
	float progress() const { return m_sectionProgress; }
	int section() const { return m_currentSection; }
	
	Vec3 calc() const;
	Vec3 calc(float progress) const;

private:
	int calculateLengths(int i, float start, float end);
	Vec3 calcBezier(int i, float pos) const;
	
	int m_sections;
	vector<Vec3> m_points;
	vector<pair<Vec3, Vec3> > m_controls;
	vector<Vec3> m_controlVectors;
	vector<float> m_lengths;
	vector<float> m_speeds;
	
	float m_totalProgress;
	
	int m_currentSection;
	float m_sectionProgress;
};

#endif
