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
	forces = Vector(0.0f, 0.0f, 0.0f); //작용한 힘들 계산 초기화
	if (isCollision) //충돌했으면
		forces += impactForces; //충격랑은 총 힘에다 더해줘야함
	else
	{
		forces += gravity; //힘은 중력에 영향을 받고

		//공기저항력 계산
		Vector Drag; //저항력
		float coef_Drag; //저항력 계수

		Drag -= velocity; //속도의 반대반향으로 저항력이 싱기니
		Drag.Normalize(); //정규화 해주고
		//1/2v^2 * 공기밀도 * r^2*pi * 저항력계수 (feat. 유체역학 책)
		coef_Drag = 0.5 * speed * speed * 1.23f * (3.14159 * radius * radius) * 0.6; //1.23f ->공기 밀도 0.6-> 저항력계수
		Drag *= coef_Drag; //계수를 곱하여 총 저항력을 구해줌

		//바람
		Vector	wind;
		//1/2*바람속도^2*공기밀도 *r^2*pi*저항계수
		wind.x = 0.5 * 1.23f * 0.1f * 0.1f * (3.14159 * radius * radius) * 0.6;
	}
}

void CircleCollider::UdpateRigidBody(float dt)
{
	Vector a; //가속도

	Vector ds; //거리 변화량

	prePosition = position; //일단 전위치를 저장
	a = forces / weight; //유명한 f=ma
	velocity += (a * dt); //v=at 속도 변화량 넣어주고
	position += (velocity * dt); //s =vt 거리 변화량 넣어주고
	speed = velocity.Magnitude(); //백터의 스칼라화(거리)
}
