#pragma once
#include "resource.h"
#include "ParticleSystem.h"

static class ParticleSystem particleSystem; //���踦 �߸��߳�... API�� �̱������� ����Ҳ�

//�̱������� ¥�� ���� �Լ� �������ϳ� �����
//�����Լ� ¥�°� �³�... ��
class WinAPI
{
public:
    enum editBox //�������̽� Ŀ���� ����Ʈ�ڽ�
    {
        ICE_X=1001,
        ICE_Y,
        ICE_V0,
        ICE_LIFE,
        ICE_GRAVITY,
        ICE_ANGLE,
    };

    enum buttoncControl //�������̽� Ŀ���� ��Ʈ�ѹ�ư
    {
        ICC_EXCUTION,
    };

    WinAPI(); //������
    ~WinAPI(); //�Ҹ���
    static bool Init(HINSTANCE hInstance, int nCmdShow); //�ʱ�ȭ
    static int Run(); //����
    static HWND GetHwnd(); //window �ڵ� ���

protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //������ ���ν���
private:
    static HWND WinAPI_hwnd; //������ �ڵ�
    static HINSTANCE hInst;
};