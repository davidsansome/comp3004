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

#ifndef TIMEDEPENDENTVARIABLE_H
#define TIMEDEPENDENTVARIABLE_H

class TimeDependentVariable
{
public:
	TimeDependentVariable();

	void setSpeed(float speed);
	void setLimit(float limit);
	void setModulo(float modulo);

	void setTargetValue(float value);
	void updateImmediately();
	void update(int timeDelta);
	void finishedEditing();

	float targetValue() const;
	float currentValue() const;

	float speed() const { return m_speed; }
	float limit() const { return m_limit; }
	float modulo() const { return m_modulo; }

	bool needsUpdating() const;

	operator float &();
	TimeDependentVariable& operator =(const float value);

private:
	float m_targetValue;
	float m_currentValue;

	float m_speed;
	float m_limit;
	float m_modulo;

	bool m_needsUpdating;
};

#endif
