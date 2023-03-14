#pragma once
#include "VariantSolution.h"

class WinAPI
{
public:
    enum editBox //�������̽� Ŀ���� ����Ʈ�ڽ�
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


    WinAPI(); //������
    ~WinAPI(); //�Ҹ���
    bool Init(HINSTANCE hInstance, int nCmdShow); //�ʱ�ȭ
    int Run(); //����
    HWND GetHwnd(); //window �ڵ� ���

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //������ ���ν���
    LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //������ ���ν���
    unsigned __stdcall ThreadsFunction(LPVOID arg);

    static WinAPI* pWinAPI;

private:
    static HWND WinAPI_hwnd; //������ �ڵ�
    static HWND Child_hwnd;
    HINSTANCE hinst;

    using Shapes = std::vector< ShapeVariant >;
    Shapes shapes;

    RECT rect;
    void Render(); //������ �׷��ֱ�
    void FixedUpdate(); //�ݶ��̴��� ���� ���

    //������
    HANDLE chartThreadHandle;
    unsigned int dwTreadID;
    bool InitChartWindow(threadArg* _arg, RECT parerentPoistion);
    int RunChartWindow();

    std::queue<MemoryStruct> *MemoryQueue;//�޸� ���´��� ť
    wchar_t editBoxCaption[6][20] = {};

    std::list<int> lineChart;//������Ʈ

    //�ʱ�ȭ
    void InitShape();
    void line(HDC hdc, int x, int y, COLORREF color, int thk);

};