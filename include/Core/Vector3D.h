#pragma once

#pragma once

/// <summary>
/// Represents a vector in 3D space
/// X, Y and Zaxis
/// </summary>
class Vector3D
{
public:
	Vector3D(float x, float y, float z);

	float		GetX() const { return m_x; }
	float		GetY() const { return m_y; }
	float		GetZ() const { return m_z; }

	void		SetX(float x) { m_x = x; }
	void		SetY(float y) { m_y = y; }
	void		SetZ(float z) { m_z = z; }

	// Pythagoras Theorem
	float		Length();

	// Add together two vectors to return a new one
	Vector3D	operator+ (const Vector3D& v2) const;

	// Using friend means you can access .m_x and .m_y for both objects directly
	friend Vector3D& operator+= (Vector3D& v1, const Vector3D& v2)
	{
		v1.m_x += v2.m_x;
		v1.m_y += v2.m_y;
		v1.m_z += v2.m_z;

		return v1;
	}

	/*
		With these two functions you can add two vectors together using standard addition operators:
		Vector2D v1(10, 11);
		Vector2D v2(35, 25);
		v1 += v2;
		Vector2D v3 = v1 + v2;
	*/

	// Multiply vector by a scalar number
	Vector3D	operator* (float scalar);

	Vector3D& operator*= (float scalar);

	// Subtraction of two vectors
	Vector3D	operator- (const Vector3D& v2) const;

	friend Vector3D& operator-= (Vector3D& v1, const Vector3D& v2)
	{
		v1.m_x -= v2.m_x;
		v1.m_y -= v2.m_y;
		v1.m_z -= v2.m_z;

		return v1;
	}

	// Divide by a scaler number
	Vector3D	operator/ (float scalar);

	Vector3D& operator /= (float scalar);

	// Normalize a vector
	void		Normalize();

private:
	float m_x;
	float m_y;
	float m_z;
};

