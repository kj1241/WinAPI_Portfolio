#include "stdafx.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : maxParticle(50), x(100), y(100), v0(75.0f), active(false), duration(4000), gravity(30), angle(0), rect({ 250,0,650,400 })
{
	particle.reserve(maxParticle); //백터 최대값
	Particle p;
	for (int i = 0; i < maxParticle; ++i)
		particle.push_back(p);
}

ParticleSystem::ParticleSystem(RECT rect) : maxParticle(50), x(100), y(100), v0(75.0f), active(false), duration(4000), gravity(30), angle(999), rect(rect)
{
	particle.reserve(maxParticle); //백터 최대값
	Particle p;
	for (int i = 0; i < maxParticle; ++i)
		particle.push_back(p);
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::CreateParticle() //파티클을 만든다
{
	active = true; //파티클 생성됨
	for (int i = 0; i < particle.size(); ++i)
	{
		// 파티클 초기값
		particle[i].x = 0;
		particle[i].y = 0;
		particle[i].vi = Random(v0 / 2, v0);

		if (angle > 360 || angle < 0) // 파티클 한방향으로 던지기
		{
			int	coe;
			if (Random(0, 1) < 0.5f)
				coe = -1;
			else
				coe = 1;
			particle[i].angle = angle + coe * sinf(Random(0, 360));
		}
		else
			particle[i].angle = Random(0, 360); //파티클 랜덤방향으로 던지기 

		float ranLife = static_cast<float>(Random(80, 100) / 100.0f);
		particle[i].life = round(duration) * ranLife;
		particle[i].r = 255;
		particle[i].g = 255;
		particle[i].b = 255;
		particle[i].time = 0;
		particle[i].active = TRUE;
		particle[i].gravity = gravity;
	}
}

void ParticleSystem::DrawParticle(HWND hwnd, HDC hdc, HDC hBufferDC)
{

	HBITMAP hBufferBitmap = CreateCompatibleBitmap(hdc,rect.right -rect.left, rect.bottom -rect.top);

	//PatBlt(hBufferDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, BLACKNESS);
	SelectObject(hBufferDC, hBufferBitmap);

	//DrawRectangle(hBufferDC, &rect, 1, RGB(0, 0, 0));
	//if (active)
	for (int i = 0; i < particle.size(); ++i)
	{
		if (particle[i].active)
		{
			float r = (static_cast<float>(particle[i].life - particle[i].time) / static_cast<float>(particle[i].life));
			COLORREF clr = RGB(round((r * particle[i].r)), round((r * particle[i].g)), (round(r * particle[i].b)));
			particle[i].active = UpdateParticleState(&particle[i], 10);

			//이중버퍼를 쓰면서 0,0으로 돌아감
			float cal_x = x + round(particle[i].x); //+ rect.left;
			float cal_y = y + round(particle[i].y); //+ rect.top;
			//if (cal_x > rect.left + 10 && cal_x < rect.right - 10)
			//	if (cal_y > rect.top + 10 && cal_y < rect.bottom - 10)
					DrawCircle(hBufferDC, cal_x, cal_y, 2, clr);
		}
	}

	BitBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, hBufferDC, 0, 0, SRCCOPY);
	//SelectObject(hBufferDC, hBufferBitmap);
	//StretchBlt(hdc, r.left, r.top, r.right - r.left, r.bottom - r.top, hBufferDC, r.left, r.top, r.right , r.bottom , SRCCOPY);
	DeleteObject(hBufferBitmap);

	active = false;
}

void ParticleSystem::DrawRectangle(HDC hdc, RECT* r, int thk, COLORREF clr)
{
	HBRUSH		CurrentBrush, OldBrush;
	HPEN		CurrentPen, OldPen;
	COLORREF	FColor = clr;
	COLORREF	BColor = clr;

	CurrentBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, CurrentBrush));
	CurrentPen = CreatePen(PS_SOLID, thk, FColor);
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, CurrentPen));

	Rectangle(hdc, r->left, r->top, r->right, r->bottom); //사각형그리기

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen);
	DeleteObject(CurrentBrush);
	DeleteObject(CurrentPen);
}

void ParticleSystem::DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr)
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

bool ParticleSystem::UpdateParticleState(Particle* particle, int dtime)
{
	const double PI = 3.1415926; //파이 값

	particle->time += dtime; //시간변화랑만큼 시간을 더해줌

	float t = static_cast<float>(particle->time / 1000.0f);//지금 시간은 밀리초로 바꿔줌
	particle->x = particle->vi * cos(particle->angle * PI / 180.0f) * t;//x 방향은 각도/호도를 포물선의 방정식으로 .
	particle->y = particle->vi * sin(particle->angle * PI / 180.0f) * t + (particle->gravity * t * t) / 2.0f;//y 방향은 중력값을 계산해서 v =v0t+ 1/2at^2

	bool result;
	if (particle->time >= particle->life) //파티클이 시간인 생명시간보다 크면 
		result = FALSE;
	else
		result = TRUE;

	return result;
}

int ParticleSystem::GetMaxParticle()
{
	return maxParticle;
}

int ParticleSystem::GetX()
{
	return x;
}

void ParticleSystem::SetX(int x)
{
	this->x = x;
}

int ParticleSystem::GetY()
{
	return y;
}

void ParticleSystem::SetY(int y)
{
	this->y = y;
}

float ParticleSystem::GetV0()
{
	return v0;
}

void ParticleSystem::SetV0(int v0)
{
	this->v0 = v0;
}

float ParticleSystem::GetGravity()
{
	return gravity;
}

void ParticleSystem::SetGravity(float gravity)
{
	this->gravity = gravity;
}

float ParticleSystem::GetAngle()
{
	return angle;
}

void ParticleSystem::SetAngle(float angle)
{
	this->angle = angle;
}

int ParticleSystem::GetLife()
{
	return duration;
}

void ParticleSystem::SetLife(int duration)
{
	this->duration = duration;
}

bool ParticleSystem::GetActive()
{
	return active;
}

void ParticleSystem::SetActive(bool active)
{
	this->active = active;
}