#pragma once

//원형 콜라이더 정의
class CircleCollider
{
public:
	CircleCollider();
	~CircleCollider();

	void CalcResistance(); //저항력 계산(유체역학 정교한건 논문을 다시 읽어봐야할듯)
	void UdpateRigidBody(float dt); //강체 계산

	//기본
	Vector position;  //콜라이더 위치
	float radius; //원 반지름
	float weight; //질량

	//속도 관련 함수들
	Vector velocity; //속도
	float speed; //속력

	//힘관련 함수들
	Vector forces; //힘
	Vector gravity; //중력
	Vector prePosition; //이전 위치
	Vector impactForces; //충격력

	int countCollider;

	bool isCollision; //충돌했는가?


};