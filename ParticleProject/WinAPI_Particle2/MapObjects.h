#pragma once

//오브젝트 안에 있는 함수
struct Objects
{
	Vector position; //위치
	COLORREF color;
};

struct CircleObjects :public Objects
{
	float r; // 반지름
	float mass;
	CircleCollider* pCollider; //콜라이더의 주소값

	void DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr);
};

struct PlaneObjects :public Objects
{
	Vector startPoint;
	Vector endPoint;
	int thk; //두깨

	void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, int thk, COLORREF clr);
};


//맵에 생성 시키는 오브젝트들
class MapObjects
{
public:
	MapObjects();
	~MapObjects();
	std::vector< CircleCollider*> GetCircleCollider();
	
	void InitObjects(); //초기화 함수
	void DrawUpdate(HDC hBufferDC); //드로우 함수

private:
	const int numberCircleObjects;
	std::vector<CircleObjects> circleObjects;
	std::vector<CircleCollider*> circleCollider; //오브젝트 패턴

	const int numberPlaneObjects;
	std::vector<PlaneObjects> planeObjects;

};