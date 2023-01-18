#pragma once

//Ŭ������ �����ؼ� ������ �ַ��������� �׷� �ʿ䰡 ����°� ���Ƽ� �׳� ����ü�� ����
struct Particle
{
	float x;			// x��ǥ 
	float y;			// y��ǥ
	float vi;			// �ʱ�ӵ�
	float angle;		// �ʱ� ���� ����
	float gravity;    // �߷� ���
	int	life;		// �����ֱ�
	int r;          // ������ �������(0~255)
	int	g;          // �ʷϻ� �������(0~255)
	int	b;          // �Ķ��� �������(0~255)
	int	time;       // �ð�
	bool active;	    // ����ƼŬ�� ����ִ���
};


//��ƼŬ �ý��� WinAPI�� �������� ���´�
class ParticleSystem
{
	float v0; //�ʱ� �ӵ�
	float gravity;  // �߷� ���ӵ�
	float angle; //������ ����
	int	duration;  //����
	//static const int maxParticle= 50; //�� �̷��� �������� ���ͷ� ��������
	int maxParticle = 50;
	int	x;  //�ʱ� x ��ġ
	int y;  //�ʱ� y ��ġ

	RECT rect;// { 0, 0, 400, 400 };
	bool active; //�� ��ƼŬ �ý����� �۵��ϴ°�?

	std::vector<Particle> particle;

	//�ʹ� �x��������� �ڵ� ¥�� �ֽ����� �ۼ��غ���
	//���ø��� ������Ͽ� �����ϴ� ����: �̰� ����� ������ ����� �ҽ����� ������ ������ �𸣴� ����
	//�ҽ������� �ִ� ������ ����Ʈ �ڵ�� ������ �ϴ� ���ε� 
	//���ø��� ������Ʈ ���� ������ �Լ��� �������� ����� ������ ��ü�ڵ�� �������ϱ����� �ϼ����Ѿ� ��
	//�׷��� �����ε� Ŭ���� �ۿ��� �����Ϸ��� ������Ͽ��� inline�Լ��� ���ؼ� �����ؾߵ�
	template <class T1, class T2> float Random(T1 min, T2 max) //���ø��� �̿��� �����Լ�
	{ //�ζ��� �Լ� ���� ��������
		//random �Լ��� �õ�� �õ���� ���밪 ������ �̷��� ���ָ� �Ҽ��� ù���ڸ����� �����Լ��� �Ҹ��� �ִµ� 
		//�̷��� �ۼ��ϸ� ���� �Լ� ������ִ� �õ�� ������ ���� ���� ���ڰ��� �������� ��迡���ؼ� ��� ������
		float result = min + ((abs(rand()) % static_cast<int>(max - min + 1) * 10) / 10);

		//����ó��
		if (result > max) return max;
		if (result < min) return min;

		return result;
	}

	void DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr); //�ڵ�, ũ��,������ ,��
	bool UpdateParticleState(Particle* particle, int dtime);

public:
	ParticleSystem();
	ParticleSystem(RECT rect);
	~ParticleSystem();

	void CreateParticle(); //��ƼŬ ����
	void DrawParticle(HWND hwnd, HDC hdc,HDC hBufferDC); // ��ƼŬ �׸���
	void DrawRectangle(HDC hdc, RECT* r, int thk, COLORREF clr); // ��ƼŬ �׸���


	float GetV0();
	void SetV0(int v0);
	float GetGravity();
	void SetGravity(float gravity);
	float GetAngle();
	void SetAngle(float angle);
	int GetLife();
	void SetLife(int duration);

	int GetMaxParticle();
	int GetX();
	void SetX(int x);
	int GetY();
	void SetY(int y);
	bool GetActive();
	void SetActive(bool active);
};

