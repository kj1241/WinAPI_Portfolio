#pragma once

class Circle;
class Square;
class _Ellipse;
class _Rectangle;

struct Vector2 {
	double x;
	double y;

	Vector2(double _x, double _y) :x(_x), y(_y) { };
};

class ShapeVisitor
{
public:
	ShapeVisitor() = default;
	virtual ~ShapeVisitor() = default;

	virtual void Visit(Circle & ) const = 0;
	virtual void Visit(Square & ) const = 0;
	virtual void Visit(_Ellipse & ) const = 0;
	virtual void Visit(_Rectangle &) const = 0;
};

class Shape
{
public:
	Shape(HDC hdc, double x, double y, COLORREF cir) :_hdc(hdc), _position(x, y), _cir(cir)
	{
	}

	virtual void Accept(ShapeVisitor const& v) = 0;

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
	virtual void Accept(ShapeVisitor const& v) override
	{
		v.Visit(*this);
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
	virtual void Accept(ShapeVisitor const& v) override
	{
		v.Visit(*this);
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
	virtual void Accept(ShapeVisitor const& v) override
	{
		v.Visit(*this);
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
	virtual void Accept(ShapeVisitor const& v) override
	{
		v.Visit(*this);
	}

private:
	Vector2 S;
};


class Draw : public ShapeVisitor
{
public:

	void Visit(Circle & c) const override;
	void Visit(Square & c) const override;
	void Visit(_Ellipse & c) const override;
	void Visit(_Rectangle & s) const override;
};


class Translate0 : public ShapeVisitor
{
public:
	void Visit(Circle& c) const override;
	void Visit(Square& c) const override;
	void Visit(_Ellipse& c) const override;
	void Visit(_Rectangle& s) const override;
};

class Translate1 : public ShapeVisitor
{
public:
	void Visit(Circle& c) const override;
	void Visit(Square& c) const override;
	void Visit(_Ellipse& c) const override;
	void Visit(_Rectangle& s) const override;
};
class Translate2 : public ShapeVisitor
{
public:
	void Visit(Circle& c) const override;
	void Visit(Square& c) const override;
	void Visit(_Ellipse& c) const override;
	void Visit(_Rectangle& s) const override;
};
class Translate3 : public ShapeVisitor
{
public:
	void Visit(Circle& c) const override;
	void Visit(Square& c) const override;
	void Visit(_Ellipse& c) const override;
	void Visit(_Rectangle& s) const override;
};
class Translate4 : public ShapeVisitor
{
public:
	void Visit(Circle& c) const override;
	void Visit(Square& c) const override;
	void Visit(_Ellipse& c) const override;
	void Visit(_Rectangle& s) const override;
};

class Translate5 : public ShapeVisitor
{
public:
	void Visit(Circle& c) const override;
	void Visit(Square& c) const override;
	void Visit(_Ellipse& c) const override;
	void Visit(_Rectangle& s) const override;
};

class Translate6 : public ShapeVisitor
{
public:
	void Visit(Circle& c) const override;
	void Visit(Square& c) const override;
	void Visit(_Ellipse& c) const override;
	void Visit(_Rectangle& s) const override;
};

class Translate7 : public ShapeVisitor
{
public:
	void Visit(Circle& c) const override;
	void Visit(Square& c) const override;
	void Visit(_Ellipse& c) const override;
	void Visit(_Rectangle& s) const override;
};


void Translate(std::vector<std::unique_ptr<Shape>> const& shape);
void DrawAllShape(std::vector<std::unique_ptr<Shape>> const& shape);