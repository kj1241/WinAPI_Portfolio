#include "stdafx.h" 
#include "WinAPI.h" 


_Use_decl_annotations_ //error c28213 �ذ� ��� ����: �����м� ���� ���� �ּ��� ���������� ���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WinAPI windowAPI;
    if (!windowAPI.Init(hInstance, nCmdShow))  //�ʱ�ȭ �����ϸ� 
        return 0;   // 0���� ����
    return windowAPI.Run(); //������ run �ڵ� ����
}
