#pragma once

//���� ���õ� �Լ�
//���͸���� �ƴ� �� Math �Լ����� �����Ѵٰ� ���� ����
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);
	~Vector();

	Vector& operator+=(Vector u); //���� ����
	Vector& operator-=(Vector u); //���� ����
	Vector& operator*=(float s);  //��Į�� ����
	Vector& operator/=(float s);  //��Į�� ������
	Vector operator-();       //����
	Vector& operator=(Vector u);  //���ٸ�



	float Magnitude(void);
	void  Normalize(void);
	void  Reverse(void);

	//������Ƽ�� �Ϸ����ߴµ� �̷��� �ϴ°� �´µ�
	float x;
	float y;
	float z;
private:

	const float tol = 0.000000000000001f;		//�ε��Ҽ��� ������
};
Vector operator*(Vector u, float s);
Vector operator/(Vector u, float s);
Vector operator+(Vector u, Vector v);
Vector operator-(Vector u, Vector v);
float operator*(Vector u, Vector v);


//�̰͵� ��ȸ�Ǹ� 4*4 ������� �ٲ����
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

	// eij i->row�� ,j->column ��
	float e[3][3];
};

class VecterF {
public:
	VecterF();
	VecterF(float n, float e1, float e2, float e3);
	~VecterF();

	float	Magnitude(); //��Ʈ(x^2 + y^2+ z^2) �Ÿ�
	Vector	GetVector();
	float	GetScalar();
	VecterF	operator+=(VecterF q);
	VecterF	operator-=(VecterF q);
	VecterF operator*=(float s);
	VecterF operator/=(float s);
	VecterF	operator~() const;

	//
	float n; //��Į��κ�
	Vector vec;	// ���ͺκ�
};


