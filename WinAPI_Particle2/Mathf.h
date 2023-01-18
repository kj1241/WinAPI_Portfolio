#pragma once

//수학 관련된 함수
//백터만들기 아니 걍 Math 함수쓰지 연습한다고 괜히 만들어서
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);
	~Vector();

	Vector& operator+=(Vector u); //백터 덧셈
	Vector& operator-=(Vector u); //백터 뺄셈
	Vector& operator*=(float s);  //스칼라 곱샘
	Vector& operator/=(float s);  //스칼라 나눗셈
	Vector operator-();       //음수
	Vector& operator=(Vector u);  //같다면



	float Magnitude(void);
	void  Normalize(void);
	void  Reverse(void);

	//프로퍼티로 하려고했는데 이렇게 하는게 맞는듯
	float x;
	float y;
	float z;
private:

	const float tol = 0.000000000000001f;		//부동소수점 허용오차
};
Vector operator*(Vector u, float s);
Vector operator/(Vector u, float s);
Vector operator+(Vector u, Vector v);
Vector operator-(Vector u, Vector v);
float operator*(Vector u, Vector v);


//이것도 기회되면 4*4 사원수로 바꿔야함
class Matrix3x3 {
public:
	Matrix3x3();
	Matrix3x3(float r1c1, float r1c2, float r1c3, float r2c1, float r2c2, float r2c3, float r3c1, float r3c2, float r3c3);
	~Matrix3x3();

	float det();
	Matrix3x3 Transpose();
	Matrix3x3 Inverse(); 

	Matrix3x3& operator+=(Matrix3x3 m);
	Matrix3x3& operator-=(Matrix3x3 m);
	Matrix3x3& operator*=(float s);
	Matrix3x3& operator/=(float s);

	// eij i->row행 ,j->column 열
	float e[3][3];
};

class VecterF {
public:
	VecterF();
	VecterF(float n, float e1, float e2, float e3);
	~VecterF();

	float	Magnitude(); //루트(x^2 + y^2+ z^2) 거리
	Vector	GetVector();
	float	GetScalar();
	VecterF	operator+=(VecterF q);
	VecterF	operator-=(VecterF q);
	VecterF operator*=(float s);
	VecterF operator/=(float s);
	VecterF	operator~() const;

	//
	float n; //스칼라부분
	Vector vec;	// 백터부분
};


