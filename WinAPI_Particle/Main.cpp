#include "stdafx.h" 
#include "WinAPI.h" 


_Use_decl_annotations_ //error c28213 �ذ� ��� ����: �����м� ���� ���� �ּ��� ���������� ���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    if (!WinAPI::Init( hInstance, nCmdShow))  //�ʱ�ȭ �����ϸ� 
        return 0;   // 0���� ����
    return WinAPI::Run(); //������ run �ڵ� ����
}
