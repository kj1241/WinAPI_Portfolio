#pragma once


struct Vector2 {
	double x;
	double y;

	Vector2(double _x, double _y) :x(_x), y(_y) { };
};


class Shape
{
public:
	Shape(HDC hdc, double x, double y, COLORREF cir) :_hdc(hdc), _position(x, y), _cir(cir)
	{
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
	COLORREF _cir;
};

class Circle : public Shape
{
public:
	explicit Circle(HDC hdc, double x, double y, COLORREF cir, double r) :Shape(hdc, x, y, cir), radius(r)
	{
	}

	double GetRadius() const noexcept
	{
		return radius;
	}

private:
	double radius;
};


class Square : public Shape
{
public:
	explicit Square(HDC hdc, double x, double y, COLORREF cir, double s) :Shape(hdc, x, y, cir), side(s)
	{
	}

	double GetSide() const noexcept
	{
		return side;
	}

private:
	double side;
};


class _Ellipse : public Shape  //Ellipse로 사용하면 충돌나서 _ 붙여줌
{
public:
	explicit _Ellipse(HDC hdc, double x, double y, COLORREF cir, double r1, double r2) :Shape(hdc, x, y, cir), R(r1, r2)
	{
	}

	Vector2 GetRadius() const noexcept
	{
		return R;
	}

private:
	Vector2 R;
};





class _Rectangle : public Shape //Rectangle로 사용하면 충돌나서 _ 붙여줌
{
public:
	explicit _Rectangle(HDC hdc, double x, double y, COLORREF cir, double a1, double b2) :Shape(hdc, x, y, cir), S(a1, b2)
	{
	}

	Vector2 GetSide() const noexcept
	{
		return S;
	}

private:
	Vector2 S;
};


class Draw 
{
public:
	void operator()(Circle const& c) const;
	void operator()(Square const& s) const;
	void operator()(_Ellipse const& c) const;
	void operator()(_Rectangle const& s) const;
};


class Translate0 
{
public:
	void operator()(Circle & c) const;
	void operator()(Square & s) const;
	void operator()(_Ellipse & c) const;
	void operator()(_Rectangle & s) const;
};

class Translate1
{
public:
	void operator()(Circle& c) const;
	void operator()(Square& s) const;
	void operator()(_Ellipse& c) const;
	void operator()(_Rectangle& s) const;
};
class Translate2 
{
public:
	void operator()(Circle& c) const;
	void operator()(Square& s) const;
	void operator()(_Ellipse& c) const;
	void operator()(_Rectangle& s) const;
};
class Translate3 
{
public:
	void operator()(Circle& c) const;
	void operator()(Square& s) const;
	void operator()(_Ellipse& c) const;
	void operator()(_Rectangle& s) const;
};
class Translate4
{
public:
	void operator()(Circle& c) const;
	void operator()(Square& s) const;
	void operator()(_Ellipse& c) const;
	void operator()(_Rectangle& s) const;
};

class Translate5
{
public:
	void operator()(Circle& c) const;
	void operator()(Square& s) const;
	void operator()(_Ellipse& c) const;
	void operator()(_Rectangle& s) const;
};

class Translate6 
{
public:
	void operator()(Circle& c) const;
	void operator()(Square& s) const;
	void operator()(_Ellipse& c) const;
	void operator()(_Rectangle& s) const;
};

class Translate7
{
public:
	void operator()(Circle& c) const;
	void operator()(Square& s) const;
	void operator()(_Ellipse& c) const;
	void operator()(_Rectangle& s) const;
};

using ShapeVariant = std::variant<Circle, Square, _Ellipse, _Rectangle>;

void Translate(std::vector<ShapeVariant> shape);
void DrawAllShape(std::vector<ShapeVariant> const& Shapes);