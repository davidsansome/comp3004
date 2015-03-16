#include "bezier.h"

Bezier::Bezier()
{
	reset();
}

Bezier::~Bezier()
{
}

void Bezier::reset()
{
	m_totalProgress = 0.0;
	m_sectionProgress = 0.0;
	m_currentSection = 0;
}

void Bezier::addPoint(const Vec3& point, const Vec3& control, float speed)
{
	m_points.push_back(point);
	m_controlVectors.push_back(control);
	m_speeds.push_back(speed);
}

void Bezier::finalize()
{
	m_sections = m_points.size();
	
	for (int i=0 ; i<m_sections ; ++i)
	{
		int n = (i+1) % m_sections;
		Vec3 controlA = m_points[i] + m_controlVectors[i];
		Vec3 controlB = m_points[n] - m_controlVectors[n];
		
		m_controls.push_back(pair<Vec3, Vec3>(controlA, controlB));
	}
	
	for (int i=0 ; i<m_sections ; ++i)
		m_lengths.push_back(float(calculateLengths(i, 0.0, 1.0)) / (30.0 * m_speeds[i]));
}

int Bezier::calculateLengths(int i, float start, float end)
{
	Vec3 startPos = calcBezier(i, start);
	Vec3 endPos = calcBezier(i, end);
	if (len(startPos - endPos) < 0.1)
		return 1;
	
	return calculateLengths(i, start, start + (end-start)/2.0) +
	       calculateLengths(i, start + (end-start)/2.0, end);
}

Vec3 Bezier::calc() const
{
	return calcBezier(m_currentSection, m_sectionProgress);
}

Vec3 Bezier::calc(float progress) const
{
	return calcBezier(m_currentSection, progress);
}

Vec3 Bezier::calcBezier(int i, float pos) const
{
	int n = (i+1) % m_sections;
	return m_points[i]          * ::pow(1.0-pos, 3.0)                         +
	       m_controls[i].first  * ::pow(1.0-pos, 2.0) * 3.0 * pos             +
	       m_controls[i].second *      (1.0-pos)      * 3.0 * ::pow(pos, 2.0) +
	       m_points[n]          *                             ::pow(pos, 3.0);
}

bool Bezier::updateProgress(float progress)
{
	m_totalProgress += progress;
	bool reset = false;
	
	// Which section are we in?
	m_currentSection = -1;
	while (true)
	{
		float sum = 0.0;
		for (int i=0 ; i<m_sections ; ++i)
		{
			sum += m_lengths[i];
			if (m_totalProgress < sum)
			{
				m_currentSection = i;
				m_sectionProgress = (m_totalProgress - sum + m_lengths[i]) / m_lengths[i];
				break;
			}
		}
		if (m_currentSection != -1)
			break;
		m_totalProgress -= sum;
		reset = true;
	}
	
	return reset;
}

Vec3 Bezier::pointA() const
{
	return m_points[m_currentSection];
}

Vec3 Bezier::pointB() const
{
	return m_points[(m_currentSection + 1) % m_sections];
}

Vec3 Bezier::controlA() const
{
	return m_controls[m_currentSection].first;
}

Vec3 Bezier::controlB() const
{
	return m_controls[m_currentSection].second;
}
