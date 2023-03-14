#include "stdafx.h"
#include "EnumSolution.h"

void Draw(Circle const& c)
{
	HBRUSH		hBrush, OldBrush; //�귯��
	HPEN		hPen, OldPen;  //�� ���� ���� ���� ���� ��
	COLORREF	FColor = c.GetColor();
	COLORREF	BColor = c.GetColor();

	HDC hdc = c.GetHDC();
	Vector2 position = c.GetPosition();
	double r = c.GetRadius();

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

void Draw(Square const& s)
{
	HBRUSH		hBrush, OldBrush; //�귯��
	HPEN		hPen, OldPen;  //�� ���� ���� ���� ���� ��
	COLORREF	FColor = s.GetColor();
	COLORREF	BColor = s.GetColor();

	HDC hdc = s.GetHDC();
	Vector2 position = s.GetPosition();
	double side = s.GetSide();

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

void Draw(_Ellipse const& c)
{
	HBRUSH		hBrush, OldBrush; //�귯��
	HPEN		hPen, OldPen;  //�� ���� ���� ���� ���� ��
	COLORREF	FColor = c.GetColor();
	COLORREF	BColor = c.GetColor();

	HDC hdc = c.GetHDC();
	Vector2 position = c.GetPosition();
	Vector2 r = c.GetRadius();

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

void Draw(_Rectangle const& s)
{
	HBRUSH		hBrush, OldBrush; //�귯��
	HPEN		hPen, OldPen;  //�� ���� ���� ���� ���� ��
	COLORREF	FColor = s.GetColor();
	COLORREF	BColor = s.GetColor();

	HDC hdc = s.GetHDC();
	Vector2 position = s.GetPosition();
	Vector2 side = s.GetSide();

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
	for(int i=0;i<25;++i)
		for (auto const& s : shape)
		{
			switch (s->GetType())
			{
			case circle:
				switch (rand() % 8)
				{
				case 0:
					Translate0(*static_cast<Circle*>(s.get()));
					break;
				case 1:
					Translate1(*static_cast<Circle*>(s.get()));
					break;
				case 2:
					Translate2(*static_cast<Circle*>(s.get()));
					break;
				case 3:
					Translate3(*static_cast<Circle*>(s.get()));
					break;
				case 4:
					Translate4(*static_cast<Circle*>(s.get()));
					break;
				case 5:
					Translate5(*static_cast<Circle*>(s.get()));
					break;
				case 6:
					Translate6(*static_cast<Circle*>(s.get()));
					break;
				case 7:
					Translate7(*static_cast<Circle*>(s.get()));
					break;
				}
				break;
			case square:
				switch (rand() % 8)
				{
				case 0:
					Translate0(*static_cast<Square*>(s.get()));
					break;
				case 1:
					Translate1(*static_cast<Square*>(s.get()));
					break;
				case 2:
					Translate2(*static_cast<Square*>(s.get()));
					break;
				case 3:
					Translate3(*static_cast<Square*>(s.get()));
					break;
				case 4:
					Translate4(*static_cast<Square*>(s.get()));
					break;
				case 5:
					Translate5(*static_cast<Square*>(s.get()));
					break;
				case 6:
					Translate6(*static_cast<Square*>(s.get()));
					break;
				case 7:
					Translate7(*static_cast<Square*>(s.get()));
					break;
				}
			case ellipse:
				switch (rand() % 8)
				{
				case 0:
					Translate0(*static_cast<_Ellipse*>(s.get()));
					break;
				case 1:
					Translate1(*static_cast<_Ellipse*>(s.get()));
					break;
				case 2:
					Translate2(*static_cast<_Ellipse*>(s.get()));
					break;
				case 3:
					Translate3(*static_cast<_Ellipse*>(s.get()));
					break;
				case 4:
					Translate4(*static_cast<_Ellipse*>(s.get()));
					break;
				case 5:
					Translate5(*static_cast<_Ellipse*>(s.get()));
					break;
				case 6:
					Translate6(*static_cast<_Ellipse*>(s.get()));
					break;
				case 7:
					Translate7(*static_cast<_Ellipse*>(s.get()));
					break;
				}
				break;
			case rectangle:
				switch (rand() % 8)
				{
				case 0:
					Translate0(*static_cast<_Rectangle*>(s.get()));
					break;
				case 1:
					Translate1(*static_cast<_Rectangle*>(s.get()));
					break;
				case 2:
					Translate2(*static_cast<_Rectangle*>(s.get()));
					break;
				case 3:
					Translate3(*static_cast<_Rectangle*>(s.get()));
					break;
				case 4:
					Translate4(*static_cast<_Rectangle*>(s.get()));
					break;
				case 5:
					Translate5(*static_cast<_Rectangle*>(s.get()));
					break;
				case 6:
					Translate6(*static_cast<_Rectangle*>(s.get()));
					break;
				case 7:
					Translate7(*static_cast<_Rectangle*>(s.get()));
					break;
				}
				break;

			}
		}
}



void DrawAllShape(std::vector<std::unique_ptr<Shape>> const& shape)
{
	for (auto const& s : shape)
	{
		switch (s->GetType())
		{
		case circle:
			Draw(*static_cast<Circle const*>(s.get()));
			break;
		case square:
			Draw(*static_cast<Square const*>(s.get()));
			break;
		case ellipse:
			Draw(*static_cast<_Ellipse const*>(s.get()));
			break;
		case rectangle:
			Draw(*static_cast<_Rectangle const*>(s.get()));
			break;

		}
	}
}

void Translate0(Circle & c)
{
	c.SetPosition(1, 0);
}

void Translate1(Circle& c)
{
	c.SetPosition(1, 1);
}

void Translate2(Circle& c)
{
	c.SetPosition(0, 1);
}

void Translate3(Circle& c)
{
	c.SetPosition(-1, 1);
}

void Translate4(Circle& c)
{
	c.SetPosition(-1, 0);
}

void Translate5(Circle& c)
{
	c.SetPosition(-1, -1);
}

void Translate6(Circle& c)
{
	c.SetPosition(0, -1);
}

void Translate7(Circle& c)
{
	c.SetPosition(1, -1);
}


//2
void Translate0(Square& c)
{
	c.SetPosition(1, 0);
}

void Translate1(Square& c)
{
	c.SetPosition(1, 1);
}

void Translate2(Square& c)
{
	c.SetPosition(0, 1);
}

void Translate3(Square& c)
{
	c.SetPosition(-1, 1);
}

void Translate4(Square& c)
{
	c.SetPosition(-1, 0);
}

void Translate5(Square& c)
{
	c.SetPosition(-1, -1);
}

void Translate6(Square& c)
{
	c.SetPosition(0, -1);
}

void Translate7(Square& c)
{
	c.SetPosition(1, -1);
}

//3
void Translate0(_Ellipse& c)
{
	c.SetPosition(1, 0);
}

void Translate1(_Ellipse& c)
{
	c.SetPosition(1, 1);
}

void Translate2(_Ellipse& c)
{
	c.SetPosition(0, 1);
}

void Translate3(_Ellipse& c)
{
	c.SetPosition(-1, 1);
}

void Translate4(_Ellipse& c)
{
	c.SetPosition(-1, 0);
}

void Translate5(_Ellipse& c)
{
	c.SetPosition(-1, -1);
}

void Translate6(_Ellipse& c)
{
	c.SetPosition(0, -1);
}

void Translate7(_Ellipse& c)
{
	c.SetPosition(1, -1);
}



void Translate0(_Rectangle& c)
{
	c.SetPosition(1, 0);
}

void Translate1(_Rectangle& c)
{
	c.SetPosition(1, 1);
}

void Translate2(_Rectangle& c)
{
	c.SetPosition(0, 1);
}

void Translate3(_Rectangle& c)
{
	c.SetPosition(-1, 1);
}

void Translate4(_Rectangle& c)
{
	c.SetPosition(-1, 0);
}

void Translate5(_Rectangle& c)
{
	c.SetPosition(-1, -1);
}

void Translate6(_Rectangle& c)
{
	c.SetPosition(0, -1);
}

void Translate7(_Rectangle& c)
{
	c.SetPosition(1, -1);
}
