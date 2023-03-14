#include "stdafx.h"
#include "OOSolution.h"

void Circle::Draw() const
{
	HBRUSH		hBrush, OldBrush; //�귯��
	HPEN		hPen, OldPen;  //�� ���� ���� ���� ���� ��
	COLORREF	FColor = GetColor();
	COLORREF	BColor = GetColor();

	HDC hdc = GetHDC();
	Vector2 position = GetPosition();
	double r = GetRadius();

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //�� �ڵ� �����
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //ȭ�鿡 ����ϰ� ����ߴ��ڵ��� ��ȯ

	Ellipse(hdc, position.x - r, position.y - r, position.x + r, position.y + r); //���� �׸���

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //�����ߴ� ���� �ٽ� ���
	DeleteObject(hBrush);
	DeleteObject(hPen); //���� ���� ���� �ڵ��� ����������
}


void Square::Draw() const
{
	HBRUSH		hBrush, OldBrush; //�귯��
	HPEN		hPen, OldPen;  //�� ���� ���� ���� ���� ��
	COLORREF	FColor = GetColor();
	COLORREF	BColor = GetColor();

	HDC hdc = GetHDC();
	Vector2 position = GetPosition();
	double side = GetSide();

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //�� �ڵ� �����
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //ȭ�鿡 ����ϰ� ����ߴ��ڵ��� ��ȯ

	Rectangle(hdc, position.x - side, position.y - side, position.x + side, position.y + side); //���� �׸���

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //�����ߴ� ���� �ٽ� ���
	DeleteObject(hBrush);
	DeleteObject(hPen); //���� ���� ���� �ڵ��� ����������
}

void _Ellipse::Draw() const
{
	HBRUSH		hBrush, OldBrush; //�귯��
	HPEN		hPen, OldPen;  //�� ���� ���� ���� ���� ��
	COLORREF	FColor = GetColor();
	COLORREF	BColor = GetColor();

	HDC hdc = GetHDC();
	Vector2 position = GetPosition();
	Vector2 r = GetRadius();

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //�� �ڵ� �����
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //ȭ�鿡 ����ϰ� ����ߴ��ڵ��� ��ȯ

	Ellipse(hdc, position.x - r.x, position.y - r.y, position.x + r.x, position.y + r.y); //���� �׸���

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //�����ߴ� ���� �ٽ� ���
	DeleteObject(hBrush);
	DeleteObject(hPen); //���� ���� ���� �ڵ��� ����������
}
void _Rectangle::Draw() const
{
	HBRUSH		hBrush, OldBrush; //�귯��
	HPEN		hPen, OldPen;  //�� ���� ���� ���� ���� ��
	COLORREF	FColor = GetColor();
	COLORREF	BColor = GetColor();

	HDC hdc = GetHDC();
	Vector2 position = GetPosition();
	Vector2 side = GetSide();

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //�� �ڵ� �����
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //ȭ�鿡 ����ϰ� ����ߴ��ڵ��� ��ȯ

	Rectangle(hdc, position.x - side.x, position.y - side.y, position.x + side.x, position.y + side.y); //���� �׸���

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //�����ߴ� ���� �ٽ� ���
	DeleteObject(hBrush);
	DeleteObject(hPen); //���� ���� ���� �ڵ��� ����������
}

void Translate(std::vector<std::unique_ptr<Shape>> const& shape)
{
	for(int i=0; i<25; ++i)
		for (auto const& s : shape)
		{
			switch (rand()%8)
			{
			case 0:
				s->Translate0();
				break;
			case 1:
				s->Translate1();
				break;
			case 2:
				s->Translate2();
				break;
			case 3:
				s->Translate3();
				break;
			case 4:
				s->Translate4();
				break;
			case 5:
				s->Translate5();
				break;
			case 6:
				s->Translate6();
				break;
			case 7:
				s->Translate7();
				break;
			}
			
		}
}


void DrawAllShape(std::vector<std::unique_ptr<Shape>> const& shape)
{
	for (auto const& s : shape)
	{
		s->Draw();
	}
}



void Circle::Translate0()
{
	SetPosition(1, 0);
}

void Circle::Translate1()
{
	SetPosition(1, 1);
}

void Circle::Translate2()
{
	SetPosition(0, 1);
}

void Circle::Translate3()
{
	SetPosition(-1, 1);
}

void Circle::Translate4()
{
	SetPosition(-1, 0);
}

void Circle::Translate5()
{
	SetPosition(-1, -1);
}

void Circle::Translate6()
{
	SetPosition(0, -1);
}

void Circle::Translate7()
{
	SetPosition(1, -1);
}


//2
void Square::Translate0()
{
	SetPosition(1, 0);
}

void Square::Translate1()
{
	SetPosition(1, 1);
}

void Square::Translate2()
{
	SetPosition(0, 1);
}

void Square::Translate3()
{
	SetPosition(-1, 1);
}

void Square::Translate4()
{
	SetPosition(-1, 0);
}

void Square::Translate5()
{
	SetPosition(-1, -1);
}

void Square::Translate6()
{
	SetPosition(0, -1);
}
void Square::Translate7()
{
	SetPosition(1, -1);
}

//3

void _Ellipse::Translate0()
{
	SetPosition(1, 0);
}

void _Ellipse::Translate1()
{
	SetPosition(1, 1);
}

void _Ellipse::Translate2()
{
	SetPosition(0, 1);
}

void _Ellipse::Translate3()
{
	SetPosition(-1, 1);
}

void _Ellipse::Translate4()
{
	SetPosition(-1, 0);
}

void _Ellipse::Translate5()
{
	SetPosition(-1, -1);
}

void _Ellipse::Translate6()
{
	SetPosition(0, -1);
}

void _Ellipse::Translate7()
{
	SetPosition(1, -1);
}


void _Rectangle::Translate0()
{
	SetPosition(1, 0);
}

void _Rectangle::Translate1()
{
	SetPosition(1, 1);
}

void _Rectangle::Translate2()
{
	SetPosition(0, 1);
}

void _Rectangle::Translate3()
{
	SetPosition(-1, 1);
}

void _Rectangle::Translate4()
{
	SetPosition(-1, 0);
}

void _Rectangle::Translate5()
{
	SetPosition(-1, -1);
}

void _Rectangle::Translate6()
{
	SetPosition(0, -1);
}

void _Rectangle::Translate7()
{
	SetPosition(1, -1);
}

