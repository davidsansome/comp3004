/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *                                                                         *
 * This file was taken from Baarbarity, (c) David Sansome 2006, and        *
 * modified for COMP3004.                                                  *
 * http://fluffy.ecs.soton.ac.uk/~ds505/svn/baarbarity/                    *
 *                                                                         *
 ***************************************************************************/

#include "timedependentvariable.h"

#include <math.h>
#include <algorithm>

TimeDependentVariable::TimeDependentVariable()
{
	m_targetValue = 0.0f;
	m_currentValue = 0.0f;
	m_needsUpdating = false;

	// Speed is taken from Bequnge
	// At 30fps, expected time delta is 33ms
	// Speed in bequnge was 0.2 per frame, so 0.006 * 33 = 0.2
	
	m_speed = 0.006f;
	m_limit = 0.001f;
	m_modulo = 0.0f;
}


void TimeDependentVariable::setTargetValue(float value)
{
	m_targetValue = value;
	m_needsUpdating = true;
}

TimeDependentVariable& TimeDependentVariable::operator =(const float value)
{
	setTargetValue(value);
	return *this;
}

void TimeDependentVariable::updateImmediately()
{
	m_currentValue = m_targetValue;
	m_needsUpdating = false;
}

void TimeDependentVariable::update(int timeDelta)
{
	if (!m_needsUpdating)
		return;
	
	float diff = m_targetValue - m_currentValue;
	if (diff > m_modulo/2.0f)
		diff -= m_modulo;
	else if (diff < - m_modulo/2.0f)
		diff += m_modulo;
	
	if (fabs(diff) < m_limit)
	{
		m_currentValue = m_targetValue;
		m_needsUpdating = false;
	}
	else
	{
		m_currentValue += diff * std::min(1.0f, m_speed * timeDelta);

		if (m_currentValue > m_modulo)
			m_currentValue -= m_modulo;
		else if (m_currentValue < 0.0f)
			m_currentValue += m_modulo;
	}
}

float TimeDependentVariable::targetValue() const
{
	return m_targetValue;
}

float TimeDependentVariable::currentValue() const
{
	return m_currentValue;
}

TimeDependentVariable::operator float &()
{
	return m_currentValue;
}

void TimeDependentVariable::setSpeed(float speed)
{
	m_speed = speed;
}

void TimeDependentVariable::setLimit(float limit)
{
	m_limit = limit;
}

void TimeDependentVariable::setModulo(float modulo)
{
	m_modulo = modulo;
}

bool TimeDependentVariable::needsUpdating() const
{
	return m_needsUpdating;
}

void TimeDependentVariable::finishedEditing()
{
	if (m_modulo != 0.0f)
	{
		if (m_targetValue > m_modulo)
		{
			m_targetValue -= m_modulo;
			m_currentValue -= m_modulo;
		}
		else if (m_targetValue < 0.0f)
		{
			m_targetValue += m_modulo;
			m_currentValue += m_modulo;
		}
	}
}
