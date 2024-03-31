#pragma once
#include "resource.h"
#include "ParticleSystem.h"
#include "MapObjects.h"

//class ParticleSystem particleSystem; //���踦 �߸��߳�... API�� �̱������� ����Ҳ�

//�ʾ������� ���� ������.
//���ν����� this ����Ʈ�� ����ؾ� �ϴµ� ���ν����� ���� ���� �̱��� ����� ��¦ �����Ͽ� 
//static �Լ� �ȿ��� ����ϱ� ���� this ����Ʈ�� ����Ͽ� �ڱ� �ڽ� ����Ʈ�� ������ش�.
class WinAPI
{
public:
    enum editBox //�������̽� Ŀ���� ����Ʈ�ڽ�
    {
        ICE_X = 1001,
        ICE_Y
    };
    enum buttoncControl //�������̽� Ŀ���� ��Ʈ�ѹ�ư
    {
        ICC_EXCUTION,
    };

    WinAPI(); //������
    ~WinAPI(); //�Ҹ���
    bool Init(HINSTANCE hInstance, int nCmdShow); //�ʱ�ȭ
    int Run(); //����
    HWND GetHwnd(); //window �ڵ� ���

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //������ ���ν���

    static WinAPI* pWinAPI;

private:
    static HWND WinAPI_hwnd; //������ �ڵ�
    static HINSTANCE hInst;
   
    RECT rect;
    MapObjects mapObject; //�ʿ������ؼ�
    ParticleSystem particleSystem;

    void DrawRectangle(HDC hdc, RECT* r, int thk, COLORREF clr); //�簢�� �׷��ֱ�

    void Render(); //������ �׷��ֱ�
    void FixedUpdate(); //�ݶ��̴��� ���� ���

    void colliderPlanetoCircle(CircleCollider* col, float time);
    bool colliderCircletoCircle(CircleCollider* aCol, CircleCollider* bCol, float time);
};