#include "pch.h"
#include "Core/Vector3D.h"

#include <cmath>

Vector3D::Vector3D(float x, float y, float z)
	:m_x(x), m_y(y), m_z(z)
{
}

float Vector3D::length()
{
	return static_cast<float>(sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z)));
}

Vector3D Vector3D::operator+(const Vector3D& v2) const
{
	return Vector3D(m_x + v2.m_x, m_y + v2.m_y, m_z + v2.m_z);
}

Vector3D& Vector3D::operator*=(float scalar)
{
	m_x *= scalar;
	m_y *= scalar;
	m_z *= scalar;

	return *this;
}

Vector3D Vector3D::operator*(float scalar)
{
	return Vector3D(m_x * scalar, m_y * scalar, m_z * scalar);
}

Vector3D Vector3D::operator-(const Vector3D& v2) const
{
	return Vector3D(m_x - v2.m_x, m_y - v2.m_y, m_z - v2.m_z);
}

Vector3D& Vector3D::operator/=(float scalar)
{
	m_x /= scalar;
	m_y /= scalar;
	m_z /= scalar;

	return *this;
}

Vector3D Vector3D::operator/(float scalar)
{
	return Vector3D(m_x / scalar, m_y / scalar, m_z / scalar);
}

void Vector3D::normalize()
{
	float L = length();
	if (L > 0) // Stops division by 0
	{
		(*this) *= 1 / L; // Normalizes this vector
	}
}

