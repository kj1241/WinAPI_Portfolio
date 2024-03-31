#include "stdafx.h"
#include "Mathf.h"

Vector::Vector(): x(0),y(0),z (0)
{
}

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vector::~Vector()
{
}

Vector& Vector::operator+=(Vector u)
{
	x += u.x;
	y += u.y;
	z += u.z;
	return *this;
}

Vector& Vector::operator-=(Vector u)
{
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

Vector& Vector::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector& Vector::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector Vector::operator-(void)
{
	return Vector(-x, -y, -z);
}

Vector& Vector::operator=(Vector u)
{
	x = u.x;
	y = u.y;
	z = u.z;
	return *this;
}


float Vector::Magnitude(void)
{
	return (float)sqrt(x * x + y * y + z * z);
}

void Vector::Normalize(void)
{
	float m = (float)sqrt(x * x + y * y + z * z);
	if (m <= tol) m = 1;
	x /= m;
	y /= m;
	z /= m;

	if (fabs(x) < tol) x = 0.0f;
	if (fabs(y) < tol) y = 0.0f;
	if (fabs(z) < tol) z = 0.0f;
}

void Vector::Reverse(void)
{
	x = -x;
	y = -y;
	z = -z;
}


VecterF::VecterF():
	n(0),
	vec(0,0,0)
{
}

VecterF::VecterF(float n, float e1, float e2, float e3):
	n(n),
	vec(e1,e2,e3)
{
}

VecterF::~VecterF()
{
}

float VecterF::Magnitude()
{
	return static_cast<float>(sqrt(n * n + vec.x * vec.x + vec.y * vec.y + vec.z * vec.z)); 
}

Vector VecterF::GetVector()
{
	return Vector(vec.x, vec.y, vec.z);
}

float VecterF::GetScalar()
{
	return n;
}

VecterF VecterF::operator+=(VecterF q)
{
	n += q.n;
	vec.x += q.vec.x;
	vec.y += q.vec.y;
	vec.z += q.vec.z;
	return *this;
}

VecterF VecterF::operator-=(VecterF q)
{
	n -= q.n;
	vec.x -= q.vec.x;
	vec.y -= q.vec.y;
	vec.z -= q.vec.z;
	return *this;
}

VecterF VecterF::operator*=(float s)
{
	n *= s;
	vec.x *= s;
	vec.y *= s;
	vec.z *= s;
	return *this;
}

VecterF VecterF::operator/=(float s)
{
	n /= s;
	vec.x /= s;
	vec.y /= s;
	vec.z /= s;
	return *this;
}

VecterF VecterF::operator~() const
{
	return VecterF(n, -vec.x, -vec.y, -vec.z);
}

Matrix3x3::Matrix3x3():
	e{(0.0f,0.0f,0.0f),(0.0f,0.0f,0.0f),(0.0f,0.0f,0.0f)}
{
}

Matrix3x3::Matrix3x3(float r1c1, float r1c2, float r1c3, float r2c1, float r2c2, float r2c3, float r3c1, float r3c2, float r3c3):
	e{(r1c1,r1c2,r1c3),(r2c1,r2c2,r2c3),(r3c1,r3c2,r3c3)}
{
}

Matrix3x3::~Matrix3x3()
{
}

float Matrix3x3::det()
{
	return	e[0][0] * e[1][1] * e[2][2] -
		e[0][0] * e[2][1] * e[1][2] +
		e[1][0] * e[2][1] * e[0][2] -
		e[1][0] * e[0][1] * e[2][2] +
		e[2][0] * e[0][1] * e[1][2] -
		e[2][0] * e[1][1] * e[0][2];
}

Matrix3x3 Matrix3x3::Transpose()
{
	return Matrix3x3(e[0][0], e[1][0], e[2][0], e[0][1], e[1][1], e[2][1], e[0][2], e[1][2], e[2][2]);
}

Matrix3x3 Matrix3x3::Inverse()
{
	//ad-bc »Æ¿Â
	float	d = e[0][0] * e[1][1] * e[2][2] -  
		e[0][0] * e[2][1] * e[1][2] +
		e[1][0] * e[2][1] * e[0][2] -
		e[1][0] * e[0][1] * e[2][2] +
		e[2][0] * e[0][1] * e[1][2] -
		e[2][0] * e[1][1] * e[0][2];

	if (d == 0) 
		d = 1;

	return	Matrix3x3((e[1][1] * e[2][2] - e[1][2] * e[2][1]) / d,
		-(e[0][1] * e[2][2] - e[0][2] * e[2][1]) / d,
		(e[0][1] * e[1][2] - e[0][2] * e[1][1]) / d,
		-(e[1][0] * e[2][2] - e[1][2] * e[2][0]) / d,
		(e[0][0] * e[2][2] - e[0][2] * e[2][0]) / d,
		-(e[0][0] * e[1][2] - e[0][2] * e[1][0]) / d,
		(e[1][0] * e[2][1] - e[1][1] * e[2][0]) / d,
		-(e[0][0] * e[2][1] - e[0][1] * e[2][0]) / d,
		(e[0][0] * e[1][1] - e[0][1] * e[1][0]) / d);
}

Matrix3x3& Matrix3x3::operator+=(Matrix3x3 m)
{
	e[0][0] += m.e[0][0];
	e[0][1] += m.e[0][1];
	e[0][2] += m.e[0][2];
	e[1][0] += m.e[1][0];
	e[1][1] += m.e[1][1];
	e[1][2] += m.e[1][2];
	e[2][0] += m.e[2][0];
	e[2][1] += m.e[2][1];
	e[2][2] += m.e[2][2];
	return *this;
}

Matrix3x3& Matrix3x3::operator-=(Matrix3x3 m)
{
	e[0][0] -= m.e[0][0];
	e[0][1] -= m.e[0][1];
	e[0][2] -= m.e[0][2];
	e[1][0] -= m.e[1][0];
	e[1][1] -= m.e[1][1];
	e[1][2] -= m.e[1][2];
	e[2][0] -= m.e[2][0];
	e[2][1] -= m.e[2][1];
	e[2][2] -= m.e[2][2];
	return *this;
}

Matrix3x3& Matrix3x3::operator*=(float s)
{
	e[0][0] *= s;
	e[0][1] *= s;
	e[0][2] *= s;
	e[1][0] *= s;
	e[1][1] *= s;
	e[1][2] *= s;
	e[2][0] *= s;
	e[2][1] *= s;
	e[2][2] *= s;
	return *this;
}

Matrix3x3& Matrix3x3::operator/=(float s)
{
	e[0][0] /= s;
	e[0][1] /= s;
	e[0][2] /= s;
	e[1][0] /= s;
	e[1][1] /= s;
	e[1][2] /= s;
	e[2][0] /= s;
	e[2][1] /= s;
	e[2][2] /= s;
	return *this;
}

Vector operator*(Vector u, float s)
{
	return Vector(u.x * s, u.y * s, u.z * s);
}

Vector operator/(Vector u, float s)
{
	return Vector(u.x / s, u.y / s, u.z / s);
}

Vector operator+(Vector u, Vector v)
{
	return Vector(u.x + v.x, u.y + v.y, u.z + v.z);
}

Vector operator-(Vector u, Vector v)
{
	return Vector(u.x - v.x, u.y - v.y, u.z - v.z);
}

float operator*(Vector u, Vector v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}
