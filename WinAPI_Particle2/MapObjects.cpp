#include "stdafx.h"
#include "MapObjects.h"

MapObjects::MapObjects():numberCircleObjects(5), numberPlaneObjects(1)
{
	circleObjects.reserve(numberCircleObjects); //���� �ִ밪
	CircleObjects c;
	for (int i = 0; i < numberCircleObjects; ++i)
	{
		c.pCollider = new CircleCollider(); //�ݶ��̴� �����Ҵ�
		circleObjects.push_back(c);
	}

	planeObjects.reserve(numberPlaneObjects); //���� �ִ밪
	PlaneObjects p;
	for (int i = 0; i < numberPlaneObjects; ++i)
		planeObjects.push_back(p);
}

MapObjects::~MapObjects()
{
	//�����Ҵ� ����
	for (int i = 0; i < numberCircleObjects; ++i)
	{
		if (circleObjects[i].pCollider != nullptr)
		{
			delete circleObjects[i].pCollider;
			circleObjects[i].pCollider = nullptr;
		}
	}
}

std::vector<CircleCollider*> MapObjects::GetCircleCollider()
{
	return circleCollider;
}

void MapObjects::InitObjects()
{
	circleObjects[0].position = Vector(210, 520, 0);
	circleObjects[1].position = Vector(390, 500, 0);
	circleObjects[2].position = Vector(560, 500, 0);
	circleObjects[3].position = Vector(590, 480, 0);
	circleObjects[4].position = Vector(740, 520, 0);

	for (int i = 0; i < circleObjects.size(); ++i)
	{
		circleObjects[i].r = 30;
		circleObjects[i].color = RGB(255,0,0);

		circleObjects[i].pCollider->position = circleObjects[i].position;
		circleObjects[i].pCollider->radius = circleObjects[i].r;
		circleObjects[i].pCollider->weight = 100;

		circleObjects[i].pCollider->velocity = Vector(0, 0, 0);
		circleObjects[i].pCollider->speed = 0;;
		circleObjects[i].pCollider->forces = Vector(0, 0, 0);
		circleObjects[i].pCollider->gravity = Vector(0, circleObjects[i].pCollider->weight * 9.8f, 0);
		circleObjects[i].pCollider->prePosition = circleObjects[i].pCollider->position;
		circleObjects[i].pCollider->countCollider = 0;
		circleObjects[i].pCollider->isCollision = false;

		circleCollider.push_back(circleObjects[i].pCollider);
	}

	planeObjects[0].position = Vector(0, 700, 0);
	planeObjects[0].color = RGB(255, 255, 255);
	
	planeObjects[0].startPoint = Vector(0, 0, 0);
	planeObjects[0].endPoint = Vector(1000, 0, 0);
	planeObjects[0].thk = 3;
}

void MapObjects::DrawUpdate(HDC hBufferDC)
{
	for (int i = 0; i < circleObjects.size(); ++i)
	{
		circleObjects[i].DrawCircle(hBufferDC, circleObjects[i].position.x, circleObjects[i].position.y, circleObjects[i].r, circleObjects[i].color);
	}
	for (int i = 0; i < planeObjects.size(); ++i)
	{
		planeObjects[i].DrawLine(hBufferDC,
			planeObjects[i].position.x + planeObjects[0].startPoint.x, 
			planeObjects[i].position.y + planeObjects[0].startPoint.y,
			planeObjects[i].position.x + planeObjects[0].endPoint.x,
			planeObjects[i].position.y + planeObjects[0].endPoint.y,
			planeObjects[i].thk, planeObjects[i].color);
	}
}

void CircleObjects::DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr)
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

void PlaneObjects::DrawLine(HDC hdc, int x1, int y1, int x2, int y2, int thk, COLORREF clr)
{
	HBRUSH		CurrentBrush;
	HBRUSH		OldBrush;
	HPEN		CurrentPen;
	HPEN		OldPen;
	COLORREF	FColor = clr;
	COLORREF	BColor = RGB(0, 0, 0);

	CurrentBrush = CreateSolidBrush(FColor);
	OldBrush = (HBRUSH)SelectObject(hdc, CurrentBrush);
	CurrentPen = CreatePen(PS_SOLID, thk, FColor);
	OldPen = (HPEN)SelectObject(hdc, CurrentPen);

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen);
	DeleteObject(CurrentBrush);
	DeleteObject(CurrentPen);
}
