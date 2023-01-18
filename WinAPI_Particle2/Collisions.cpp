#include "stdafx.h"
#include "Collisions.h"

CircleCollider::CircleCollider() :
	position(0, 0, 0),
	radius(2),
	weight(5),
	velocity(0, 0, 0),
	speed(0),
	forces(0, 0, 0),
	gravity(0, weight * 9.8f, 0),
	prePosition(position),
	countCollider(0),
	isCollision(false)
{

}

CircleCollider::~CircleCollider()
{
}

void CircleCollider::CalcResistance()
{
	forces = Vector(0.0f, 0.0f, 0.0f); //�ۿ��� ���� ��� �ʱ�ȭ
	if (isCollision) //�浹������
		forces += impactForces; //��ݶ��� �� ������ ���������
	else
	{
		forces += gravity; //���� �߷¿� ������ �ް�

		//�������׷� ���
		Vector Drag; //���׷�
		float coef_Drag; //���׷� ���

		Drag -= velocity; //�ӵ��� �ݴ�������� ���׷��� �̱��
		Drag.Normalize(); //����ȭ ���ְ�
		//1/2v^2 * ����е� * r^2*pi * ���׷°�� (feat. ��ü���� å)
		coef_Drag = 0.5 * speed * speed * 1.23f * (3.14159 * radius * radius) * 0.6; //1.23f ->���� �е� 0.6-> ���׷°��
		Drag *= coef_Drag; //����� ���Ͽ� �� ���׷��� ������

		//�ٶ�
		Vector	wind;
		//1/2*�ٶ��ӵ�^2*����е� *r^2*pi*���װ��
		wind.x = 0.5 * 1.23f * 0.1f * 0.1f * (3.14159 * radius * radius) * 0.6;
	}
}

void CircleCollider::UdpateRigidBody(float dt)
{
	Vector a; //���ӵ�

	Vector ds; //�Ÿ� ��ȭ��

	prePosition = position; //�ϴ� ����ġ�� ����
	a = forces / weight; //������ f=ma
	velocity += (a * dt); //v=at �ӵ� ��ȭ�� �־��ְ�
	position += (velocity * dt); //s =vt �Ÿ� ��ȭ�� �־��ְ�
	speed = velocity.Magnitude(); //������ ��Į��ȭ(�Ÿ�)
}
