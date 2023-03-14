#pragma once
#include "VariantSolution.h"

class WinAPI
{
public:
    enum editBox //인터페이스 커스텀 에디트박스
    {
        EditBox0 = 1001,
        EditBox1,
        EditBox2,
        EditBox3,
        EditBox4,
        EditBox5
    };

    struct threadArg{
        HINSTANCE hInstance; 
        int nCmdShow;
    };

    struct MemoryStruct{
        int logicTime;
        int vitrualMemory;
        int physicsMemory;
    };


    WinAPI(); //생성자
    ~WinAPI(); //소멸자
    bool Init(HINSTANCE hInstance, int nCmdShow); //초기화
    int Run(); //실행
    HWND GetHwnd(); //window 핸들 얻기

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //윈도우 프로시져
    LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //윈도우 프로시져
    unsigned __stdcall ThreadsFunction(LPVOID arg);

    static WinAPI* pWinAPI;

private:
    static HWND WinAPI_hwnd; //윈도우 핸들
    static HWND Child_hwnd;
    HINSTANCE hinst;

    using Shapes = std::vector< ShapeVariant >;
    Shapes shapes;

    RECT rect;
    void Render(); //랜더링 그려주기
    void FixedUpdate(); //콜라이더등 물리 계산

    //쓰레드
    HANDLE chartThreadHandle;
    unsigned int dwTreadID;
    bool InitChartWindow(threadArg* _arg, RECT parerentPoistion);
    int RunChartWindow();

    std::queue<MemoryStruct> *MemoryQueue;//메모리 상태담을 큐
    wchar_t editBoxCaption[6][20] = {};

    std::list<int> lineChart;//라인차트

    //초기화
    void InitShape();
    void line(HDC hdc, int x, int y, COLORREF color, int thk);

};