#include "stdafx.h"
#include "VisitorSolution.h"

void Draw::Visit(Circle & c) const 
{
	HBRUSH		hBrush, OldBrush; //브러쉬
	HPEN		hPen, OldPen;  //팬 선의 상태 굵기 점선 등
	COLORREF	FColor = c.GetColor();
	COLORREF	BColor = c.GetColor();

	HDC hdc = c.GetHDC();
	Vector2 position = c.GetPosition();
	double r = c.GetRadius();

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //팬 핸들 만들기
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //화면에 등록하고 등록했던핸들을 반환

	Ellipse(hdc, position.x - r, position.y - r, position.x + r, position.y + r); //원형 그리기

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //저장했던 팬을 다시 등록
	DeleteObject(hBrush);
	DeleteObject(hPen); //새로 만든 팬의 핸들을 지워버린다
}


void Draw::Visit(Square & c)const
{
	HBRUSH		hBrush, OldBrush; //브러쉬
	HPEN		hPen, OldPen;  //팬 선의 상태 굵기 점선 등
	COLORREF	FColor = c.GetColor();
	COLORREF	BColor = c.GetColor();

	HDC hdc = c.GetHDC();
	Vector2 position = c.GetPosition();
	double side = c.GetSide();

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //팬 핸들 만들기
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //화면에 등록하고 등록했던핸들을 반환

	Rectangle(hdc, position.x - side, position.y - side, position.x + side, position.y + side); //원형 그리기

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //저장했던 팬을 다시 등록
	DeleteObject(hBrush);
	DeleteObject(hPen); //새로 만든 팬의 핸들을 지워버린다
}

void Draw::Visit(_Ellipse & c) const
{
	HBRUSH		hBrush, OldBrush; //브러쉬
	HPEN		hPen, OldPen;  //팬 선의 상태 굵기 점선 등
	COLORREF	FColor = c.GetColor();
	COLORREF	BColor = c.GetColor();

	HDC hdc = c.GetHDC();
	Vector2 position = c.GetPosition();
	Vector2 r = c.GetRadius();

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //팬 핸들 만들기
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //화면에 등록하고 등록했던핸들을 반환

	Ellipse(hdc, position.x - r.x, position.y - r.y, position.x + r.x, position.y + r.y); //원형 그리기

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //저장했던 팬을 다시 등록
	DeleteObject(hBrush);
	DeleteObject(hPen); //새로 만든 팬의 핸들을 지워버린다
}
void Draw::Visit(_Rectangle& s) const
{
	HBRUSH		hBrush, OldBrush; //브러쉬
	HPEN		hPen, OldPen;  //팬 선의 상태 굵기 점선 등
	COLORREF	FColor = s.GetColor();
	COLORREF	BColor = s.GetColor();

	HDC hdc = s.GetHDC();
	Vector2 position = s.GetPosition();
	Vector2 side = s.GetSide();

	hBrush = CreateSolidBrush(BColor);
	OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
	//hPen = CreatePen(PS_SOLID, thk, FColor); 
	hPen = CreatePen(PS_SOLID, 1, FColor); //팬 핸들 만들기
	OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //화면에 등록하고 등록했던핸들을 반환

	Rectangle(hdc, position.x - side.x, position.y - side.y, position.x + side.x, position.y + side.y); //원형 그리기

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen); //저장했던 팬을 다시 등록
	DeleteObject(hBrush);
	DeleteObject(hPen); //새로 만든 팬의 핸들을 지워버린다
}

void Translate(std::vector<std::unique_ptr<Shape>> const& shape)
{
	for (int i = 0; i < 25; ++i)
	{
		for (auto const& s : shape)
		{
			switch (rand() % 8)
			{
			case 0:
				s->Accept(Translate0{});
				break;
			case 1:
				s->Accept(Translate1{});
				break;
			case 2:
				s->Accept(Translate2{});
				break;
			case 3:
				s->Accept(Translate3{});
				break;
			case 4:
				s->Accept(Translate4{});
				break;
			case 5:
				s->Accept(Translate5{});
				break;
			case 6:
				s->Accept(Translate6{});
				break;
			case 7:
				s->Accept(Translate7{});
				break;
			}

		}
	}
}



void DrawAllShape(std::vector<std::unique_ptr<Shape>> const& shape)
{
	for (auto const& s : shape)
	{
		s->Accept(Draw{});
	}
}



void Translate0::Visit(Circle& c) const
{
	c.SetPosition(1, 0);
}

void Translate1::Visit(Circle& c) const
{
	c.SetPosition(1, 1);
}

void Translate2::Visit(Circle& c) const
{
	c.SetPosition(0, 1);
}

void Translate3::Visit(Circle& c) const
{
	c.SetPosition(-1, 1);
}

void Translate4::Visit(Circle& c) const
{
	c.SetPosition(-1, 0);
}

void Translate5::Visit(Circle& c) const
{
	c.SetPosition(-1, -1);
}

void Translate6::Visit(Circle& c) const
{
	c.SetPosition(0, -1);
}

void Translate7::Visit(Circle& c) const
{
	c.SetPosition(1, -1);
}


//2
void Translate0::Visit(Square& c) const
{
	c.SetPosition(1, 0);
}

void Translate1::Visit(Square& c) const
{
	c.SetPosition(1, 1);
}

void Translate2::Visit(Square& c) const
{
	c.SetPosition(0, 1);
}

void Translate3::Visit(Square& c) const
{
	c.SetPosition(-1, 1);
}

void Translate4::Visit(Square& c) const
{
	c.SetPosition(-1, 0);
}

void Translate5::Visit(Square& c) const
{
	c.SetPosition(-1, -1);
}

void Translate6::Visit(Square& c) const
{
	c.SetPosition(0, -1);
}

void Translate7::Visit(Square& c) const
{
	c.SetPosition(1, -1);
}

//3
void Translate0::Visit(_Ellipse& c) const
{
	c.SetPosition(1, 0);
}

void Translate1::Visit(_Ellipse& c) const
{
	c.SetPosition(1, 1);
}

void Translate2::Visit(_Ellipse& c) const
{
	c.SetPosition(0, 1);
}

void Translate3::Visit(_Ellipse& c) const
{
	c.SetPosition(-1, 1);
}

void Translate4::Visit(_Ellipse& c) const
{
	c.SetPosition(-1, 0);
}

void Translate5::Visit(_Ellipse& c) const
{
	c.SetPosition(-1, -1);
}

void Translate6::Visit(_Ellipse& c) const
{
	c.SetPosition(0, -1);
}

void Translate7::Visit(_Ellipse& c) const
{
	c.SetPosition(1, -1);
}

//4
void Translate0::Visit(_Rectangle& c) const
{
	c.SetPosition(1, 0);
}

void Translate1::Visit(_Rectangle& c) const
{
	c.SetPosition(1, 1);
}

void Translate2::Visit(_Rectangle& c) const
{
	c.SetPosition(0, 1);
}

void Translate3::Visit(_Rectangle& c) const
{
	c.SetPosition(-1, 1);
}

void Translate4::Visit(_Rectangle& c) const
{
	c.SetPosition(-1, 0);
}

void Translate5::Visit(_Rectangle& c) const
{
	c.SetPosition(-1, -1);
}

void Translate6::Visit(_Rectangle& c) const
{
	c.SetPosition(0, -1);
}

void Translate7::Visit(_Rectangle& c) const
{
	c.SetPosition(1, -1);
}

