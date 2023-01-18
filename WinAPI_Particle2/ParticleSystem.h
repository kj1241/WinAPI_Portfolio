#pragma once

//Ŭ������ �����ؼ� ������ �ַ��������� �׷� �ʿ䰡 ����°� ���Ƽ� �׳� ����ü�� ���� 
struct Particle
{
	Vector position; //��ġ
	COLORREF color; //��
	float radius; //������

	//float vi;			// �ʱ�ӵ�
	//float angle;		// �ʱ� ���� ����
	//float gravity;    // �߷� ���
	//int	life;		// �����ֱ�

	//int	time;       // �ð�
	//bool active;	    // ����ƼŬ�� ����ִ���
	bool activeCollider; //�� �ݶ��̴��� ���� �޴���
	CircleCollider *pCollider; //�ݶ��̴��� �ּҰ�

	void DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr); //�ڵ�, ũ��,������ ,��
};


//��ƼŬ �ý��� WinAPI�� �������� ���´�
class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	
	std::vector< CircleCollider*> GetParticleCollider();

	void CreateParticle(); //��ƼŬ ����
	void DrawParticle(HDC hBufferDC); // ��ƼŬ �׸���

	Vector GetPosition();
	void SetPosition(Vector position);

	int GetNumberParticle();
	void SetNumberParticle(int numberParticle);

	int GetNubmerWidthParticle();
	void SetnubmerWidthParticle(int nubmerWidthParticle);

private:
	Vector position; //��ƼŬ �ý����� ��ġ x y z

	const int numberParticle; //��ƼŬ ��
	int nubmerWidthParticle;
	std::vector<Particle> particle; //��ƼŬ ����
	
	std::vector<CircleCollider*> collider;//�ݶ��̴� ����

	bool active;//�� �ý����� �۵��ϴ°�?

	//float v0; //�ʱ� �ӵ�
	//float gravity;  // �߷� ���ӵ�
	//float angle; //������ ����
	//int	duration;  //����
	//const int maxParticle = 1000;
	//int	x;  //�ʱ� x ��ġ
	//int y;  //�ʱ� y ��ġ

	//RECT rect;
	//bool active; //�� ��ƼŬ �ý����� �۵��ϴ°�?

	//std::vector<Particle> particle;

	 //���ø��� �̿��� �����Լ�
	template <class T1, class T2> float Random(T1 min, T2 max)
	{
		float result = min + ((abs(rand()) % static_cast<int>(max - min + 1) * 10) / 10);

		//����ó��
		if (result > max) return max;
		if (result < min) return min;

		return result;
	}
};

