#pragma once

struct Vector2 {
	double x;
	double y;	

	Vector2(double _x, double _y) :x(_x), y(_y) { };
};

class Shape
{
public:
	Shape(HDC hdc, double x, double y, COLORREF cir):_hdc(hdc), _position(x, y), _cir(cir)
	{
	}

	virtual ~Shape() = default;
	virtual void Draw() const = 0;
	virtual void Translate0() = 0;
	virtual void Translate1() = 0;
	virtual void Translate2() = 0;
	virtual void Translate3() = 0;
	virtual void Translate4() = 0;
	virtual void Translate5() = 0;
	virtual void Translate6() = 0;
	virtual void Translate7() = 0;

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
	explicit Circle(HDC hdc, double x, double y, COLORREF cir, double r) :Shape( hdc, x, y, cir), radius(r)
	{
	}

	double GetRadius() const noexcept
	{
		return radius;
	}
	void Draw() const override;
	void Translate0() override;
	void Translate1() override;
	void Translate2() override;
	void Translate3() override;
	void Translate4() override;
	void Translate5() override;
	void Translate6() override;
	void Translate7() override;

private:
	double radius;
};


class Square : public Shape
{
public:
	explicit Square(HDC hdc, double x, double y, COLORREF cir, double s) :Shape( hdc, x, y, cir), side(s)
	{
	}

	double GetSide() const noexcept
	{
		return side;
	}
	void Draw() const override;
	void Translate0() override;
	void Translate1() override;
	void Translate2() override;
	void Translate3() override;
	void Translate4() override;
	void Translate5() override;
	void Translate6() override;
	void Translate7() override;

private:
	double side;
};


class _Ellipse : public Shape  //Ellipse로 사용하면 충돌나서 _ 붙여줌
{
public:
	explicit _Ellipse(HDC hdc, double x, double y, COLORREF cir, double r1, double r2) :Shape( hdc, x, y, cir), R(r1, r2)
	{
	}

	Vector2 GetRadius() const noexcept
	{
		return R;
	}
	void Draw() const override;
	void Translate0() override;
	void Translate1() override;
	void Translate2() override;
	void Translate3() override;
	void Translate4() override;
	void Translate5() override;
	void Translate6() override;
	void Translate7() override;

private:
	Vector2 R;
};





class _Rectangle : public Shape //Rectangle로 사용하면 충돌나서 _ 붙여줌
{
public:
	explicit _Rectangle(HDC hdc, double x, double y, COLORREF cir, double a1, double b2) :Shape( hdc, x, y, cir), S(a1, b2)
	{
	}

	Vector2 GetSide() const noexcept
	{
		return S;
	}
	void Draw() const override;
	void Translate0() override;
	void Translate1() override;
	void Translate2() override;
	void Translate3() override;
	void Translate4() override;
	void Translate5() override;
	void Translate6() override;
	void Translate7() override;

private:
	Vector2 S;
};




void Translate(std::vector<std::unique_ptr<Shape>> const& shape);
void DrawAllShape(std::vector<std::unique_ptr<Shape>> const& shape);