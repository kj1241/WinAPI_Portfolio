#pragma once

//������Ʈ �ȿ� �ִ� �Լ�
struct Objects
{
	Vector position; //��ġ
	COLORREF color;
};

struct CircleObjects :public Objects
{
	float r; // ������
	float mass;
	CircleCollider* pCollider; //�ݶ��̴��� �ּҰ�

	void DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr);
};

struct PlaneObjects :public Objects
{
	Vector startPoint;
	Vector endPoint;
	int thk; //�α�

	void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, int thk, COLORREF clr);
};


//�ʿ� ���� ��Ű�� ������Ʈ��
class MapObjects
{
public:
	MapObjects();
	~MapObjects();
	std::vector< CircleCollider*> GetCircleCollider();
	
	void InitObjects(); //�ʱ�ȭ �Լ�
	void DrawUpdate(HDC hBufferDC); //��ο� �Լ�

private:
	const int numberCircleObjects;
	std::vector<CircleObjects> circleObjects;
	std::vector<CircleCollider*> circleCollider; //������Ʈ ����

	const int numberPlaneObjects;
	std::vector<PlaneObjects> planeObjects;

};