#pragma once
#include "resource.h"
#include "StandarDeviation.h"


class WinAPI
{
public:
    enum editBox //�������̽� Ŀ���� ����Ʈ�ڽ�
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
    enum buttoncControl //�������̽� Ŀ���� ��Ʈ�ѹ�ư
    {
        ICC_EXCUTION,
        ICC_EXCLE,
        ICC_CHECKBOX,
    };

    WinAPI(); //������
    ~WinAPI(); //�Ҹ���
    bool Init(HINSTANCE hInstance, int nCmdShow); //�ʱ�ȭ
    int Run(); //����
    HWND GetHwnd(); //window �ڵ� ���

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //������ ���ν���

    static WinAPI* pWinAPI;
    std::string convertToTime(double time);

private:
    static HWND WinAPI_hwnd; //������ �ڵ�
    static HINSTANCE hInst;

    RECT rect;

    StandarDeviation sd;
    HWND hButton, hButton2, hCheckbox;

    void DrawRectangle(HDC hdc, RECT* r, int thk, COLORREF clr); //�簢�� �׷��ֱ�

    void Render(); //������ �׷��ֱ�
    void FixedUpdate(); //�ݶ��̴��� ���� ���
};