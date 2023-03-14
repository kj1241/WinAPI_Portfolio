#include "stdafx.h"
#include "VariantSolution.h"



void Draw::operator()(Circle const& c) const
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


void Draw::operator()(Square const& s) const
{
	HBRUSH		hBrush, OldBrush; //브러쉬
	HPEN		hPen, OldPen;  //팬 선의 상태 굵기 점선 등
	COLORREF	FColor = s.GetColor();
	COLORREF	BColor = s.GetColor();

	HDC hdc = s.GetHDC();
	Vector2 position = s.GetPosition();
	double side = s.GetSide();

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

void Draw::operator()(_Ellipse const& c) const
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

void Draw::operator()(_Rectangle const& s) const
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

void Translate(std::vector<ShapeVariant> shapes)
{
	for (int i = 0; i < 25; ++i)
	{
		for (auto& s : shapes)
		{
			switch (rand() % 8)
			{
			case 0:
				std::visit(Translate0{}, s);
				break;
			case 1:
				std::visit(Translate1{}, s);
				break;
			case 2:
				std::visit(Translate2{}, s);
				break;
			case 3:
				std::visit(Translate3{}, s);
				break;
			case 4:
				std::visit(Translate4{}, s);
				break;
			case 5:
				std::visit(Translate5{}, s);
				break;
			case 6:
				std::visit(Translate6{}, s);
				break;
			case 7:
				std::visit(Translate7{}, s);
				break;
			}

		}
	}
}



void DrawAllShape(std::vector<ShapeVariant> const& Shapes)
{
	for (auto const& s : Shapes)
	{
		std::visit(Draw{}, s);
	}
}


void Translate0::operator()(Circle& c) const
{
	c.SetPosition(1, 0);
}

void Translate1::operator()(Circle& c) const
{
	c.SetPosition(1, 1);
}

void Translate2::operator()(Circle& c) const
{
	c.SetPosition(0, 1);
}

void Translate3::operator()(Circle& c) const
{
	c.SetPosition(-1, 1);
}

void Translate4::operator()(Circle& c) const
{
	c.SetPosition(-1, 0);
}

void Translate5::operator()(Circle& c) const
{
	c.SetPosition(-1, -1);
}

void Translate6::operator()(Circle& c) const
{
	c.SetPosition(0, -1);
}

void Translate7::operator()(Circle& c) const
{
	c.SetPosition(1, -1);
}


//2
void Translate0::operator()(Square& c) const
{
	c.SetPosition(1, 0);
}

void Translate1::operator()(Square& c) const
{
	c.SetPosition(1, 1);
}

void Translate2::operator()(Square& c) const
{
	c.SetPosition(0, 1);
}

void Translate3::operator()(Square& c) const
{
	c.SetPosition(-1, 1);
}

void Translate4::operator()(Square& c) const
{
	c.SetPosition(-1, 0);
}

void Translate5::operator()(Square& c) const
{
	c.SetPosition(-1, -1);
}

void Translate6::operator()(Square& c) const
{
	c.SetPosition(0, -1);
}

void Translate7::operator()(Square& c) const
{
	c.SetPosition(1, -1);
}

//3
void Translate0::operator()(_Ellipse& c) const
{
	c.SetPosition(1, 0);
}

void Translate1::operator()(_Ellipse& c) const
{
	c.SetPosition(1, 1);
}

void Translate2::operator()(_Ellipse& c) const
{
	c.SetPosition(0, 1);
}

void Translate3::operator()(_Ellipse& c) const
{
	c.SetPosition(-1, 1);
}

void Translate4::operator()(_Ellipse& c) const
{
	c.SetPosition(-1, 0);
}

void Translate5::operator()(_Ellipse& c) const
{
	c.SetPosition(-1, -1);
}

void Translate6::operator()(_Ellipse& c) const
{
	c.SetPosition(0, -1);
}

void Translate7::operator()(_Ellipse& c) const
{
	c.SetPosition(1, -1);
}

//4
void Translate0::operator()(_Rectangle& c) const
{
	c.SetPosition(1, 0);
}

void Translate1::operator()(_Rectangle& c) const
{
	c.SetPosition(1, 1);
}

void Translate2::operator()(_Rectangle& c) const
{
	c.SetPosition(0, 1);
}

void Translate3::operator()(_Rectangle& c) const
{
	c.SetPosition(-1, 1);
}

void Translate4::operator()(_Rectangle& c) const
{
	c.SetPosition(-1, 0);
}

void Translate5::operator()(_Rectangle& c) const
{
	c.SetPosition(-1, -1);
}

void Translate6::operator()(_Rectangle& c) const
{
	c.SetPosition(0, -1);
}

void Translate7::operator()(_Rectangle& c) const
{
	c.SetPosition(1, -1);
}


