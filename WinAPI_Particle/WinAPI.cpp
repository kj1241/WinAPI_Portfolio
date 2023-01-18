#include "stdafx.h"
#include "WinAPI.h"

HWND WinAPI::WinAPI_hwnd = nullptr; //�ڵ� �� �ʱ�ȭ
HINSTANCE WinAPI::hInst = nullptr;

WinAPI::WinAPI() //������
{
   

}

WinAPI::~WinAPI() //�Ҹ���
{

}

bool WinAPI::Init(HINSTANCE hInstance, int nCmdShow)
{
    //����� �ŰԺ��� ���� 
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    LocalFree(argv);
    hInst = hInstance;

    // ������ Ŭ�� �ʱ�ȭ
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"Particle_ToyProjcet";
    windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 700,450 };//������ â����
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // â�� �ڵ� ����
    WinAPI_hwnd = CreateWindow(
        windowClass.lpszClassName,
        L"Particle_ToyProjcet",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,        // �θ�â ����
        nullptr,        // �޴� ������� ����
        hInstance,
        nullptr); //���ν����� DirectXBase Ŭ���� �ּ� �ѱ��

    ShowWindow(WinAPI_hwnd, nCmdShow); //������ �����ֱ�

    HDC hdc = GetDC(WinAPI_hwnd);
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, false, false, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial"); //��Ʈ �����
    HFONT OldFont = (HFONT)SelectObject(hdc, hFont);

    //���� �ؽ�Ʈ
    TextOut(hdc, 30, 30, L"X", static_cast<int>(strlen("X")));
    TextOut(hdc, 30, 70, L"Y", static_cast<int>(strlen("Y")));
    TextOut(hdc, 30, 110, L"v0", static_cast<int>(strlen("v0")));
    TextOut(hdc, 30, 150, L"Duration", static_cast<int>(strlen("Duration")));
    TextOut(hdc, 30, 190, L"Gravity", static_cast<int>(strlen("Gravity")));
    TextOut(hdc, 30, 230, L"Angle", static_cast<int>(strlen("Angle")));

    //���ۿ� ����
    wchar_t editBoxCaption[6][8];
    swprintf(editBoxCaption[0], 8, L"%d", particleSystem.GetX());
    swprintf(editBoxCaption[1], 8, L"%d", particleSystem.GetY());
    swprintf(editBoxCaption[2], 8, L"%f", particleSystem.GetV0());
    swprintf(editBoxCaption[3], 8, L"%d", particleSystem.GetLife());
    swprintf(editBoxCaption[4], 8, L"%f", particleSystem.GetGravity());
    swprintf(editBoxCaption[5], 8, L"%f", particleSystem.GetAngle());

    HWND hEidt[6];
    for (int i = 0; i < 6; ++i)
    {
        //����Ʈ �ڽ�
        hEidt[i] = CreateWindow(
            L"edit",
            editBoxCaption[i],
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            100, 30 + (40 * i), 100, 25,
            WinAPI_hwnd,
            reinterpret_cast<HMENU>(ICE_X + i),
            hInst,
            nullptr);
    }

    HWND hButton = CreateWindow(
        L"Button",
        L"����",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | ES_CENTER,
        30, 270, 100, 30,
        WinAPI_hwnd,
        reinterpret_cast<HMENU>(ICC_EXCUTION),
        hInst,
        nullptr);

    SelectObject(hdc, OldFont);
    DeleteObject(hFont);
    ReleaseDC(WinAPI_hwnd, hdc);

    //HDC hdc = GetDC(WinAPI_hwnd);
    //HDC hBufferDC = CreateCompatibleDC(hdc);
    //HBITMAP hBufferBitmap = CreateCompatibleBitmap(hdc, 400, 400);
    //ReleaseDC(WinAPI_hwnd, hdc);
    //SelectObject(hBufferDC, hBufferBitmap);

    return true;
}

//WinAPI����
int WinAPI::Run()
{
    PAINTSTRUCT ps;
    // ���� ����
    MSG msg = { 0 };

    while (msg.message != WM_QUIT) //�޽����� winAPI���ᰡ �ƴ϶��
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // �޽����� ������ ó��
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else // �Ǽ� directx �� �ϴٺ��� �̰��� ����� ȭ�� �����϶� ����Ʈ�� �ٽ� �������� ������ ������ �ֳ�
        {
            HDC hdc = GetDC(WinAPI_hwnd); // HDC��������
            HDC hBufferDC = CreateCompatibleDC(hdc); //�׷��� ����̹� ������ ���θ���

            particleSystem.DrawParticle(WinAPI_hwnd, hdc , hBufferDC);
            ReleaseDC(WinAPI_hwnd, hBufferDC);
            DeleteObject(hBufferDC);
        }
    }

    return static_cast<char>(msg.wParam);    // WM_QUIT �޽����� ��ȯ
}

//�ڵ鰪�� ��� ���ؼ�
HWND WinAPI::GetHwnd()
{
    return WinAPI_hwnd;
}

//�� ���ν���
LRESULT WinAPI::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;

    //�޽���
    switch (message)
    {
    case WM_CREATE: //â ����� ������ ���� ����
    {
        // ������ ����� DirectX12Base �� ����
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }
    return 0;

    case WM_COMMAND: //Ŀ��� �Լ��� ��������
        switch (LOWORD(wParam)) //�޼����� wParam�� ó���Ǵ�
        {
            //�����ư�� ������
        case ICC_EXCUTION:
            //��Ʈ�ѹڽ� ���ڿ� �б�

            wchar_t editBoxCaption[6][8];

            for (int i = 0; i < 6; ++i)
            {
                GetDlgItemText(WinAPI_hwnd, ICE_X + i, editBoxCaption[i], 8);
                //particleSystem.SetX(atoi(str)); �����ڵ尡 �ƴ�
            }
            //�̷��� �Ⱦ����� ���� ���۷����� ������.. �����ؾߵǴµ� ��������̶�
            particleSystem.SetX(_wtoi(editBoxCaption[0]));
            particleSystem.SetY(_wtoi(editBoxCaption[1]));
            particleSystem.SetV0(_wtof(editBoxCaption[2]));
            particleSystem.SetLife(_wtoi(editBoxCaption[3]));
            particleSystem.SetGravity(_wtof(editBoxCaption[4]));
            particleSystem.SetAngle(_wtof(editBoxCaption[5]));

            swprintf(editBoxCaption[0], 8, L"%d", particleSystem.GetX());
            swprintf(editBoxCaption[1], 8, L"%d", particleSystem.GetY());
            swprintf(editBoxCaption[2], 8, L"%f", particleSystem.GetV0());
            swprintf(editBoxCaption[3], 8, L"%d", particleSystem.GetLife());
            swprintf(editBoxCaption[4], 8, L"%f", particleSystem.GetGravity());
            swprintf(editBoxCaption[5], 8, L"%f", particleSystem.GetAngle());

            SetDlgItemText(WinAPI_hwnd, ICE_X, editBoxCaption[0]);
            particleSystem.CreateParticle();
            break;

        case ID_EXID://Ż��
            PostQuitMessage(0);
            break;
        }

    case WM_KEYDOWN: //Ű��ư�� ��������
        return 0;

    case WM_KEYUP:  //Ű��ư��  �������
        return 0;

    //case WM_PAINT: //������� �ʴ� ����: ���Լ��� �ٽ� �׸��¿��ε� run()�Լ����� ó���ϰ� �ֱ� ����
    //    return 0;

    case WM_DESTROY: //�ı��Ǿ�����
        PostQuitMessage(0);
        return 0;
    }

    // ����Ʈ�� ��� ��� �޽��� ó��
    return DefWindowProc(hWnd, message, wParam, lParam);
}


