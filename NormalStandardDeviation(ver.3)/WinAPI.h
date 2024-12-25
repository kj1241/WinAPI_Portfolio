#pragma once
#include "resource.h"
#include "StandarDeviation.h"


class WinAPI
{
public:
    enum editBox //인터페이스 커스텀 에디트박스
    {
        ICE_XMIN = 1001,
        ICE_XMAX,
        ICE_XSTEP,
        ICE_MEAN,
        ICE_INPUTY,
        ICE_STEP,
        ICE_YSCALING,
        ICE_STDDEV,
    };
    enum buttoncControl //인터페이스 커스템 컨트롤버튼
    {
        ICC_EXCUTION,
        ICC_EXCLE,
        ICC_CHECKBOX,
    };

    WinAPI(); //생성자
    ~WinAPI(); //소멸자
    bool Init(HINSTANCE hInstance, int nCmdShow); //초기화
    int Run(); //실행
    HWND GetHwnd(); //window 핸들 얻기

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //윈도우 프로시져

    static WinAPI* pWinAPI;
    std::string convertToTime(double time);

private:
    static HWND WinAPI_hwnd; //윈도우 핸들
    static HINSTANCE hInst;

    RECT rect;

    StandarDeviation sd;
    HWND hButton, hButton2, hCheckbox;

    void DrawRectangle(HDC hdc, RECT* r, int thk, COLORREF clr); //사각형 그려주기

    void Render(); //랜더링 그려주기
    void FixedUpdate(); //콜라이더등 물리 계산
};