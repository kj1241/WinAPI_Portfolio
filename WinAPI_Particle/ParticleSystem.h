#pragma once

//클래스로 선언해서 의존성 주려고했으나 그럴 필요가 없어는거 같아서 그냥 구조체로 선언
struct Particle
{
	float x;			// x좌표 
	float y;			// y좌표
	float vi;			// 초기속도
	float angle;		// 초기 방향 각도
	float gravity;    // 중력 계수
	int	life;		// 생명주기
	int r;          // 빨간색 구성요소(0~255)
	int	g;          // 초록색 구성요소(0~255)
	int	b;          // 파랑색 구성요소(0~255)
	int	time;       // 시간
	bool active;	    // 이파티클이 살아있는지
};


//파티클 시스템 WinAPI에 의존성을 갖는다
class ParticleSystem
{
	float v0; //초기 속도
	float gravity;  // 중력 가속도
	float angle; //입자의 방향
	int	duration;  //수명
	//static const int maxParticle= 50; //걍 이렇게 쓰지말고 백터로 선언하자
	int maxParticle = 50;
	int	x;  //초기 x 위치
	int y;  //초기 y 위치

	RECT rect;// { 0, 0, 400, 400 };
	bool active; //이 파티클 시스템이 작동하는가?

	std::vector<Particle> particle;

	//너무 엤날방식으로 코드 짜서 최신으로 작성해보자
	//템플릿을 헤더파일에 선언하는 이유: 이거 물어보면 솔직히 헤더랑 소스파일 나누는 이유를 모르는 거임
	//소스파일을 넣는 이유는 바이트 코드로 컴파일 하는 것인데 
	//템플릿은 오브젝트 파일 끝에서 함수를 패턴으로 만들기 때문에 개체코드로 컴파일하기전에 완성시켜야 됨
	//그래서 구현부도 클래스 밖에서 구현하려면 헤더파일에서 inline함수를 통해서 구현해야됨
	template <class T1, class T2> float Random(T1 min, T2 max) //템플릿을 이용한 랜덤함수
	{ //인라인 함수 별로 안좋아함
		//random 함수가 시드라 시드랎에 절대값 씌위고 이렇게 해주면 소수점 첫재자리까지 랜덤함수를 불를수 있는데 
		//이렇게 작성하면 랜덤 함수 편향되있는 시드는 막을수 없음 물론 입자값이 많아지면 통계에의해서 고로 분포됨
		float result = min + ((abs(rand()) % static_cast<int>(max - min + 1) * 10) / 10);

		//에러처리
		if (result > max) return max;
		if (result < min) return min;

		return result;
	}

	void DrawCircle(HDC hdc, int cX, int cY, int r, COLORREF clr); //핸들, 크기,선종류 ,색
	bool UpdateParticleState(Particle* particle, int dtime);

public:
	ParticleSystem();
	ParticleSystem(RECT rect);
	~ParticleSystem();

	void CreateParticle(); //파티클 생성
	void DrawParticle(HWND hwnd, HDC hdc,HDC hBufferDC); // 파티클 그리기
	void DrawRectangle(HDC hdc, RECT* r, int thk, COLORREF clr); // 파티클 그리기


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

