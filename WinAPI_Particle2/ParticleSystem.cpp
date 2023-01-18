#include "stdafx.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : numberParticle(1000) , nubmerWidthParticle(50), position(Vector(475, 300, 0)), active(true)
{
	particle.reserve(numberParticle); //���� �ִ밪
	Particle p;
	for (int i = 0; i < numberParticle; ++i)
	{
		p.pCollider = new CircleCollider(); //�ݶ��̴� �����Ҵ�
		particle.push_back(p);
	}
}

ParticleSystem::~ParticleSystem()
{
	//�����Ҵ� ����
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

void ParticleSystem::CreateParticle() //��ƼŬ�� �����
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
		particle[i].position = particle[i].pCollider->position - position; //���� ��ġ�� ���
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
//	const double PI = 3.1415926; //���� ��
//
//	particle->time += dtime; //�ð���ȭ����ŭ �ð��� ������
//
//	float t = static_cast<float>(particle->time / 1000.0f);//���� �ð��� �и��ʷ� �ٲ���
//	particle->x = particle->vi * cos(particle->angle * PI / 180.0f) * t;//x ������ ����/ȣ���� �������� ���������� .
//	particle->y = particle->vi * sin(particle->angle * PI / 180.0f) * t + (particle->gravity * t * t) / 2.0f;//y ������ �߷°��� ����ؼ� v =v0t+ 1/2at^2
//
//	bool result;
//	if (particle->time >= particle->life) //��ƼŬ�� �ð��� ����ð����� ũ�� 
//		result = FALSE;
//	else
//		result = TRUE;
//
//	return result;
//}



void Particle::DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr)
{
	HBRUSH		hBrush, OldBrush; //�귯��
	HPEN		hPen, OldPen;  //�� ���� ���� ���� ���� ��
	COLORREF	FColor = clr;
	COLORREF	BColor = clr;

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //�� �ڵ� �����
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //ȭ�鿡 ����ϰ� ����ߴ��ڵ��� ��ȯ

	Ellipse(hdc, cX - r, cY - r, cX + r, cY + r); //���� �׸���

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //�����ߴ� ���� �ٽ� ���
	DeleteObject(hBrush);
	DeleteObject(hPen); //���� ���� ���� �ڵ��� ����������
}
