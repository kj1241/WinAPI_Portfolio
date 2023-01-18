#pragma once
#include "resource.h"
#include "ParticleSystem.h"

static class ParticleSystem particleSystem; //설계를 잘못했네... API를 싱글톤으로 사용할껄

//싱글톤으로 짜서 정적 함수 지워야하나 고민중
//의존함수 짜는게 맞나... 음
class WinAPI
{
public:
    enum editBox //인터페이스 커스텀 에디트박스
    {
        ICE_X=1001,
        ICE_Y,
        ICE_V0,
        ICE_LIFE,
        ICE_GRAVITY,
        ICE_ANGLE,
    };

    enum buttoncControl //인터페이스 커스템 컨트롤버튼
    {
        ICC_EXCUTION,
    };

    WinAPI(); //생성자
    ~WinAPI(); //소멸자
    static bool Init(HINSTANCE hInstance, int nCmdShow); //초기화
    static int Run(); //실행
    static HWND GetHwnd(); //window 핸들 얻기

protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //윈도우 프로시져
private:
    static HWND WinAPI_hwnd; //윈도우 핸들
    static HINSTANCE hInst;
};