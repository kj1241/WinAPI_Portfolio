#include "stdafx.h"
#include "WinAPI.h"

HWND WinAPI::WinAPI_hwnd = nullptr; //�ڵ� �� �ʱ�ȭ
HINSTANCE WinAPI::hInst = nullptr;

WinAPI* WinAPI::pWinAPI = nullptr;

static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return WinAPI::pWinAPI->WindowProc(hWnd, msg, wParam, lParam);
}

WinAPI::WinAPI() : rect({ 0,0,1000,600 })//������
{
    pWinAPI = this;

}

WinAPI::~WinAPI() //�Ҹ���
{
    pWinAPI = nullptr;

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
    windowClass.lpfnWndProc = ::wndProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"NormalStandardDeviation";
    windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 1250,600 };//������ â����
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // â�� �ڵ� ����
    WinAPI_hwnd = CreateWindow(
        windowClass.lpszClassName,
        L"NormalStandardDeviation",
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


    TextOut(hdc, 1020, 70, L"xMin", static_cast<int>(strlen("xMin")));
    TextOut(hdc, 1020, 110, L"xMax", static_cast<int>(strlen("xMax")));
    TextOut(hdc, 1020, 150, L"xStep", static_cast<int>(strlen("xStep")));

    TextOut(hdc, 1020, 190, L"mean", static_cast<int>(strlen("mean")));
    TextOut(hdc, 1020, 230, L"minutes", static_cast<int>(strlen("minutes")));

    TextOut(hdc, 1020, 270, L"step", static_cast<int>(strlen("step")));
    TextOut(hdc, 1020, 310, L"yScaling", static_cast<int>(strlen("yScaling")));
    TextOut(hdc, 1020, 350, L"stddev", static_cast<int>(strlen("stddev")));
   
    ////���ۿ� ����
    wchar_t editBoxCaption[8][11];
    swprintf(editBoxCaption[0], 11, L"%.2f", sd.GetXMin());
    swprintf(editBoxCaption[1], 11, L"%.2f", sd.GetXMax());
    swprintf(editBoxCaption[2], 11, L"%.3f", sd.GetXStep());

    swprintf(editBoxCaption[3], 11, L"%.2f", sd.GetMean());
    swprintf(editBoxCaption[4], 11, L"%.2f", sd.GetMinutes());
   
    swprintf(editBoxCaption[5], 8, L"%d", sd.GetStep());
    swprintf(editBoxCaption[6], 11, L"%f", sd.GetYScaling());
    swprintf(editBoxCaption[7], 11, L"%.2f", sd.GetStddev());

    HWND hEidt[8];
    for (int i = 0; i < 8; ++i)
    {
        //����Ʈ �ڽ�
        hEidt[i] = CreateWindow(
            L"edit",
            editBoxCaption[i],
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            1120, 70 + (40 * i), 100, 25,
            WinAPI_hwnd,
            reinterpret_cast<HMENU>(ICE_XMIN + i),
            hInst,
            nullptr);
    }

    hCheckbox = CreateWindow(
        L"Button",  // ��Ʈ�� Ŭ����: ��ư (üũ�ڽ��� ��ư Ŭ���� ���)
        L"��ü�������� ���",  // üũ�ڽ� �ؽ�Ʈ
        WS_VISIBLE | WS_CHILD | BS_CHECKBOX,  // üũ�ڽ� ��Ÿ��
        1020, 30, 150, 30,  // ��ġ �� ũ��
        WinAPI_hwnd,
        reinterpret_cast<HMENU>(ICC_CHECKBOX),  // üũ�ڽ� ID
        hInst,
        nullptr);

    hButton = CreateWindow(
        L"Button",
        L"����",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | ES_CENTER,
        1020, 390, 100, 30,
        WinAPI_hwnd,
        reinterpret_cast<HMENU>(ICC_EXCUTION),
        hInst,
        nullptr);

    hButton2 = CreateWindow(
        L"Button",
        L"Excle",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | ES_CENTER,
        1130, 390, 100, 30,
        WinAPI_hwnd,
        reinterpret_cast<HMENU>(ICC_EXCLE),
        hInst,
        nullptr);

    ShowWindow(hButton2, SW_HIDE);
    DrawRectangle(hdc, &rect, 1, RGB(255, 255, 255));

    SelectObject(hdc, OldFont);
    DeleteObject(hFont);
    ReleaseDC(WinAPI_hwnd, hdc);

    if (!sd.InitStandarDeviation())
        return false;

    return true;
}

//WinAPI����
int WinAPI::Run()
{
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
            FixedUpdate();
            //Render();
        }
    }
    
    sd.CloseStandarDeviation();

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
    HDC hdc = GetDC(WinAPI_hwnd);
    double value;
    RECT rect2 = { 1130, 390, 1230, 420 };
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
            wchar_t editBoxCaption[8][11];

            for (int i = 0; i < 8; ++i)
            {
                GetDlgItemText(WinAPI_hwnd, ICE_XMIN + i, editBoxCaption[i], 11);
                //particleSystem.SetX(atoi(str)); �����ڵ尡 �ƴ�
            }
  
            sd.SetXMin(_wtof(editBoxCaption[0]));
            sd.SetXMax(_wtof(editBoxCaption[1]));
            sd.SetXStep(_wtof(editBoxCaption[2]));
            
            sd.SetMean(_wtof(editBoxCaption[3]));
            sd.SetMinutes(_wtof(editBoxCaption[4]));

            sd.SetStep(_wtoi(editBoxCaption[5]));
            sd.SetYScaling(_wtof(editBoxCaption[6]));
            sd.SetStddev(_wtof(editBoxCaption[7]));

            swprintf(editBoxCaption[0], 11, L"%.2f", sd.GetXMin());
            swprintf(editBoxCaption[1], 11, L"%.2f", sd.GetXMax());
            swprintf(editBoxCaption[2], 11, L"%.3f", sd.GetXStep());


            swprintf(editBoxCaption[3], 11, L"%.2f", sd.GetMean());
            swprintf(editBoxCaption[4], 11, L"%.2f", sd.GetMinutes());

            swprintf(editBoxCaption[5], 8, L"%d", sd.GetStep());     
            swprintf(editBoxCaption[6], 11, L"%f", sd.GetYScaling());
            swprintf(editBoxCaption[7], 11, L"%.2f", sd.GetStddev());

            DrawRectangle(hdc, &rect, 1, RGB(255, 255, 255));

            sd.Calc(WinAPI_hwnd);

            ShowWindow(hButton2, SW_SHOW);
            break;

        case ICC_EXCLE:
            sd.MakeExcle();
            ShowWindow(hButton2, SW_HIDE);
            DrawRectangle(hdc, &rect2, 1, RGB(255, 255, 255));
            break;

        case ICC_CHECKBOX: {  // üũ�ڽ� ID�� ICC_CHECKBOX�� ��
            HWND hCheckbox = (HWND)lParam;
            int state = SendMessage(hCheckbox, BM_GETCHECK, 0, 0);  // üũ�ڽ� ���� ��������

            // ���¸� ������Ŵ (üũ/��üũ ���)
            if (state == BST_CHECKED) {
                // ���� üũ �����̹Ƿ� üũ ����
                SendMessage(hCheckbox, BM_SETCHECK, BST_UNCHECKED, 0);
                sd.SetIsMaxProbabilityValue(false);
            }
            else {
                // ���� üũ ���� �����̹Ƿ� üũ
                SendMessage(hCheckbox, BM_SETCHECK, BST_CHECKED, 0);
                sd.SetIsMaxProbabilityValue(true);
            }


            break;
        }

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

std::string WinAPI::convertToTime(double time)
{
    // ���� �κ��� �ð�, �Ҽ��� �κ��� ������ ��ȯ
    int hours = static_cast<int>(time);
    int minutes = static_cast<int>(std::round((time - hours) * 60));
    int seconds = 0; // �׻� 00�ʷ� ����

    // minutes�� 60 �̻��̸� �ùٸ��� ����
    if (minutes >= 60) {
        hours += minutes / 60;
        minutes %= 60;
    }

    // HH:MM:SS �������� ��ȯ
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}


void WinAPI::DrawRectangle(HDC hdc, RECT* r, int thk, COLORREF clr)
{
    HBRUSH		CurrentBrush, OldBrush;
    HPEN		CurrentPen, OldPen;
    COLORREF	FColor = clr;
    COLORREF	BColor = clr;

    CurrentBrush = CreateSolidBrush(BColor);
    OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, CurrentBrush));
    CurrentPen = CreatePen(PS_SOLID, thk, FColor);
    OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, CurrentPen));

    Rectangle(hdc, r->left, r->top, r->right, r->bottom); //�簢���׸���

    SelectObject(hdc, OldBrush);
    SelectObject(hdc, OldPen);
    DeleteObject(CurrentBrush);
    DeleteObject(CurrentPen);
}

void WinAPI::Render()
{
    HDC hdc = GetDC(WinAPI_hwnd); // HDC��������
    HDC hBufferDC = CreateCompatibleDC(hdc); //�׷��� ����̹� ������ ���θ���, ���߹��۸�
    BYTE* lpbits = NULL;

    HBITMAP hBufferBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
    //HBITMAP hBufferBitmap = CreateDIBSection(hBufferDC, lpBackBufferBitmapInfo, RGB(0,0,0), (void**)&lpbits, NULL, 0); //��ġ�� ����������

    SelectObject(hBufferDC, hBufferBitmap); //��Ʈ������ ��ü���ֱ�
    //DrawRectangle(hBufferDC, &rect, 1, RGB(0, 0, 0));

    BitBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, hBufferDC, 0, 0, SRCCOPY);

    DeleteObject(hBufferBitmap);

    ReleaseDC(WinAPI_hwnd, hBufferDC);
    DeleteObject(hBufferDC);
    DeleteDC(hdc);
}

void WinAPI::FixedUpdate()
{
    //for (int i = 0; i < particleSystem.GetParticleCollider().size(); ++i)
    //{
    //    float time = 0.1f;
    //    CircleCollider* pCollider = particleSystem.GetParticleCollider()[i];
    //    //�ʱⰪ����
    //    pCollider->isCollision = false;
    //    pCollider->impactForces = Vector(0, 0, 0); //��ݷ� �ʱ�ȭ
    //    pCollider->countCollider = 0;
    //    colliderPlanetoCircle(pCollider, time);

    //    for (int j = 0; j < mapObject.GetCircleCollider().size(); ++j)
    //    {
    //        CircleCollider* pCircleObjectColldier = mapObject.GetCircleCollider()[j];
    //        colliderCircletoCircle(pCollider, pCircleObjectColldier, time);
    //    }


    //    //for (int j = 0; j < particleSystem.GetParticleCollider().size(); ++j)
    //    //{
    //    //    CircleCollider* pEnmyCollider = particleSystem.GetParticleCollider()[j];

    //    //    if (i == j)
    //    //        continue;
    //    //    //if(colliderCircletoCircle(pCollider, pEnmyCollider, time))
    //    //    //    pCollider->countCollider += 1;
    //    //}


    //    pCollider->CalcResistance(); //���׷� ���
    //    pCollider->UdpateRigidBody(time); //��ü����
    //}

}
