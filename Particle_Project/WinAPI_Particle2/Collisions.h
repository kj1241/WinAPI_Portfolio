#pragma once

//���� �ݶ��̴� ����
class CircleCollider
{
public:
	CircleCollider();
	~CircleCollider();

	void CalcResistance(); //���׷� ���(��ü���� �����Ѱ� ���� �ٽ� �о�����ҵ�)
	void UdpateRigidBody(float dt); //��ü ���

	//�⺻
	Vector position;  //�ݶ��̴� ��ġ
	float radius; //�� ������
	float weight; //����

	//�ӵ� ���� �Լ���
	Vector velocity; //�ӵ�
	float speed; //�ӷ�

	//������ �Լ���
	Vector forces; //��
	Vector gravity; //�߷�
	Vector prePosition; //���� ��ġ
	Vector impactForces; //��ݷ�

	int countCollider;

	bool isCollision; //�浹�ߴ°�?


};