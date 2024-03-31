#pragma once
#include "resource.h"
#include "ParticleSystem.h"
#include "MapObjects.h"

//class ParticleSystem particleSystem; //설계를 잘못했네... API를 싱글톤으로 사용할껄

//늦엇을때가 가장 빠를때.
//프로시저는 this 포인트를 사용해야 하는데 프로시저를 쓰기 위해 싱글톤 기법을 살짝 변형하여 
//static 함수 안에서 사용하기 위해 this 포인트를 대신하여 자기 자신 포인트를 만들어준다.
class WinAPI
{
public:
    enum editBox //인터페이스 커스텀 에디트박스
    {
        ICE_X = 1001,
        ICE_Y
    };
    enum buttoncControl //인터페이스 커스템 컨트롤버튼
    {
        ICC_EXCUTION,
    };

    WinAPI(); //생성자
    ~WinAPI(); //소멸자
    bool Init(HINSTANCE hInstance, int nCmdShow); //초기화
    int Run(); //실행
    HWND GetHwnd(); //window 핸들 얻기

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //윈도우 프로시져

    static WinAPI* pWinAPI;

private:
    static HWND WinAPI_hwnd; //윈도우 핸들
    static HINSTANCE hInst;
   
    RECT rect;
    MapObjects mapObject; //맵에관련해서
    ParticleSystem particleSystem;

    void DrawRectangle(HDC hdc, RECT* r, int thk, COLORREF clr); //사각형 그려주기

    void Render(); //랜더링 그려주기
    void FixedUpdate(); //콜라이더등 물리 계산

    void colliderPlanetoCircle(CircleCollider* col, float time);
    bool colliderCircletoCircle(CircleCollider* aCol, CircleCollider* bCol, float time);
};