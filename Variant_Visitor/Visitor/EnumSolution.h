#pragma once

enum ShapeType {
	circle = 0,
	square = 1,
	ellipse = 2,
	rectangle = 3
};

struct Vector2 {
	double x;
	double y;

	Vector2(double _x, double _y) :x(_x), y(_y) { };
};

class Shape
{
public:
	//explicit묵시적 형병환 금지 (원하지 않는 형변환 금지)
	explicit Shape(ShapeType t, HDC hdc, double x, double y, COLORREF cir) :_type(t), _position(x,y), _hdc(hdc), _cir(cir)
	{
	}

	virtual ~Shape() = default;
	ShapeType GetType() const noexcept //noexcept 에외 상황 던지기
	{
		return _type;
	}

	HDC GetHDC()const noexcept
	{
		return _hdc;
	}
	Vector2 GetPosition() const noexcept
	{
		return _position;
	}
	COLORREF GetColor() const noexcept
	{
		return _cir;
	}
	void SetPosition(double x, double y) noexcept
	{
		_position.x += x;
		_position.y += y;
	}


private:
	Vector2 _position;
	HDC _hdc;
	ShapeType _type;
	COLORREF _cir;
};

class Circle : public Shape
{
public:
	explicit Circle(HDC hdc, double x, double y, COLORREF cir, double r) :Shape(circle, hdc, x, y, cir), radius(r)
	{
	}

	double GetRadius() const noexcept
	{
		return radius;
	}

private:
	double radius;
};

//void Translate(Circle&, Vector2D const&);
void Translate0(Circle&c);
void Translate1(Circle&c);
void Translate2(Circle&c);
void Translate3(Circle&c);
void Translate4(Circle&c);
void Translate5(Circle&c);
void Translate6(Circle&c);
void Translate7(Circle&c);
void Draw(Circle const& c);

class Square : public Shape
{
public:
	explicit Square(HDC hdc, double x, double y, COLORREF cir, double s) :Shape(square, hdc, x, y, cir), side(s)
	{
	}

	double GetSide() const noexcept
	{
		return side;
	}

private:
	double side;
};

void Translate0(Square&);
void Translate1(Square&);
void Translate2(Square&);
void Translate3(Square&);
void Translate4(Square&);
void Translate5(Square&);
void Translate6(Square&);
void Translate7(Square&);
void Draw(Square const& s);


class _Ellipse : public Shape  //Ellipse로 사용하면 충돌나서 _ 붙여줌
{
public:
	explicit _Ellipse(HDC hdc, double x, double y, COLORREF cir, double r1, double r2) :Shape(ellipse, hdc, x, y, cir), R(r1, r2)
	{
	}

	Vector2 GetRadius() const noexcept
	{
		return R;
	}

private:
	Vector2 R;
};


void Translate0(_Ellipse&);
void Translate1(_Ellipse&);
void Translate2(_Ellipse&);
void Translate3(_Ellipse&);
void Translate4(_Ellipse&);
void Translate5(_Ellipse&);
void Translate6(_Ellipse&);
void Translate7(_Ellipse&);
void Draw(_Ellipse const& c);


class _Rectangle : public Shape //Rectangle로 사용하면 충돌나서 _ 붙여줌
{
public:
	explicit _Rectangle(HDC hdc, double x, double y, COLORREF cir, double a1, double b2) :Shape(rectangle, hdc, x, y, cir), S(a1, b2)
	{
	}

	Vector2 GetSide() const noexcept
	{
		return S;
	}

private:
	Vector2 S;
};

void Translate0(_Rectangle&);
void Translate1(_Rectangle&);
void Translate2(_Rectangle&);
void Translate3(_Rectangle&);
void Translate4(_Rectangle&);
void Translate5(_Rectangle&);
void Translate6(_Rectangle&);
void Translate7(_Rectangle&);
void Draw(_Rectangle const& s);


void Translate(std::vector<std::unique_ptr<Shape>> const& shape);
void DrawAllShape(std::vector<std::unique_ptr<Shape>> const& shape);