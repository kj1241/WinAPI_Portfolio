#include "stdafx.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : numberParticle(1000) , nubmerWidthParticle(50), position(Vector(475, 300, 0)), active(true)
{
	particle.reserve(numberParticle); //백터 최대값
	Particle p;
	for (int i = 0; i < numberParticle; ++i)
	{
		p.pCollider = new CircleCollider(); //콜라이더 동적할당
		particle.push_back(p);
	}
}

ParticleSystem::~ParticleSystem()
{
	//동적할당 해제
	for (int i = 0; i < numberParticle; ++i)
	{
		if (particle[i].pCollider != nullptr)
		{
			delete particle[i].pCollider;
			particle[i].pCollider = nullptr;
		}
	}
}

std::vector<CircleCollider*> ParticleSystem::GetParticleCollider()
{
	return collider;
}

void ParticleSystem::CreateParticle() //파티클을 만든다
{
	int numberhightParticle = static_cast<int>(numberParticle / nubmerWidthParticle);
	for (int i = 0; i < nubmerWidthParticle; ++i)
		for (int j = 0; j < numberhightParticle; ++j)
		{
			particle[(i * numberhightParticle) + j].position = Vector(i * 4, j, 0);
		}
	for (int i = 0; i < nubmerWidthParticle* numberhightParticle; ++i)
	{
		particle[i].color = RGB(0, 0, 255);
		particle[i].radius = 2;

		particle[i].pCollider->position = particle[i].position + position;
		particle[i].pCollider->radius = particle[i].radius;
		particle[i].pCollider->weight = 5;

		particle[i].pCollider->velocity = Vector(0, 0, 0);
		particle[i].pCollider->speed = 0;;
		particle[i].pCollider->forces = Vector(0, 0, 0);
		particle[i].pCollider->gravity = Vector(0, particle[i].pCollider->weight *9.8f,0);
		particle[i].pCollider->prePosition = particle[i].pCollider->position;
		particle[i].pCollider->countCollider = 0;
		particle[i].pCollider->isCollision = false;

		collider.push_back(particle[i].pCollider);
	}
}

void ParticleSystem::DrawParticle(HDC hBufferDC)
{
	for (int i = 0; i < numberParticle; ++i)
	{
		particle[i].position = particle[i].pCollider->position - position; //원래 위치로 계산
		particle[i].DrawCircle(hBufferDC, this->position.x+particle[i].position.x, this->position.y + particle[i].position.y, particle[i].radius, particle[i].color);
	}
}

Vector ParticleSystem::GetPosition()
{
	return position;
}
 
void ParticleSystem::SetPosition(Vector position)
{
	this->position=position;
}



//bool ParticleSystem::UpdateParticleState(Particle* particle, int dtime)
//{
//	const double PI = 3.1415926; //파이 값
//
//	particle->time += dtime; //시간변화랑만큼 시간을 더해줌
//
//	float t = static_cast<float>(particle->time / 1000.0f);//지금 시간은 밀리초로 바꿔줌
//	particle->x = particle->vi * cos(particle->angle * PI / 180.0f) * t;//x 방향은 각도/호도를 포물선의 방정식으로 .
//	particle->y = particle->vi * sin(particle->angle * PI / 180.0f) * t + (particle->gravity * t * t) / 2.0f;//y 방향은 중력값을 계산해서 v =v0t+ 1/2at^2
//
//	bool result;
//	if (particle->time >= particle->life) //파티클이 시간인 생명시간보다 크면 
//		result = FALSE;
//	else
//		result = TRUE;
//
//	return result;
//}



void Particle::DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr)
{
	HBRUSH		hBrush, OldBrush; //브러쉬
	HPEN		hPen, OldPen;  //팬 선의 상태 굵기 점선 등
	COLORREF	FColor = clr;
	COLORREF	BColor = clr;

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //팬 핸들 만들기
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //화면에 등록하고 등록했던핸들을 반환

	Ellipse(hdc, cX - r, cY - r, cX + r, cY + r); //원형 그리기

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //저장했던 팬을 다시 등록
	DeleteObject(hBrush);
	DeleteObject(hPen); //새로 만든 팬의 핸들을 지워버린다
}
