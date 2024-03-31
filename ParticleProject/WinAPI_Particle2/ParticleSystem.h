#pragma once

//클래스로 선언해서 의존성 주려고했으나 그럴 필요가 없어는거 같아서 그냥 구조체로 선언 
struct Particle
{
	Vector position; //위치
	COLORREF color; //색
	float radius; //반지름

	//float vi;			// 초기속도
	//float angle;		// 초기 방향 각도
	//float gravity;    // 중력 계수
	//int	life;		// 생명주기

	//int	time;       // 시간
	//bool active;	    // 이파티클이 살아있는지
	bool activeCollider; //이 콜라이더가 적용 받는지
	CircleCollider *pCollider; //콜라이더의 주소값

	void DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr); //핸들, 크기,선종류 ,색
};


//파티클 시스템 WinAPI에 의존성을 갖는다
class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	
	std::vector< CircleCollider*> GetParticleCollider();

	void CreateParticle(); //파티클 생성
	void DrawParticle(HDC hBufferDC); // 파티클 그리기

	Vector GetPosition();
	void SetPosition(Vector position);

	int GetNumberParticle();
	void SetNumberParticle(int numberParticle);

	int GetNubmerWidthParticle();
	void SetnubmerWidthParticle(int nubmerWidthParticle);

private:
	Vector position; //파티클 시스템의 위치 x y z

	const int numberParticle; //파티클 수
	int nubmerWidthParticle;
	std::vector<Particle> particle; //파티클 생성
	
	std::vector<CircleCollider*> collider;//콜라이더 모음

	bool active;//이 시스템이 작동하는가?

	//float v0; //초기 속도
	//float gravity;  // 중력 가속도
	//float angle; //입자의 방향
	//int	duration;  //수명
	//const int maxParticle = 1000;
	//int	x;  //초기 x 위치
	//int y;  //초기 y 위치

	//RECT rect;
	//bool active; //이 파티클 시스템이 작동하는가?

	//std::vector<Particle> particle;

	 //템플릿을 이용한 랜덤함수
	template <class T1, class T2> float Random(T1 min, T2 max)
	{
		float result = min + ((abs(rand()) % static_cast<int>(max - min + 1) * 10) / 10);

		//에러처리
		if (result > max) return max;
		if (result < min) return min;

		return result;
	}
};

