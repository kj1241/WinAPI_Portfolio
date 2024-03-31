#include "stdafx.h"
#include "WinAPI.h"

HWND WinAPI::WinAPI_hwnd = nullptr; //�ڵ� �� �ʱ�ȭ
HINSTANCE WinAPI::hInst = nullptr;

WinAPI* WinAPI::pWinAPI = nullptr;

static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return WinAPI::pWinAPI->WindowProc(hWnd, msg, wParam, lParam);
}

WinAPI::WinAPI(): rect({ 250,0,1150,750 })//������
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
    windowClass.lpszClassName = L"Particle_ToyProjcet";
    windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 1200,800 };//������ â����
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

    TextOut(hdc, 30, 30, L"X", static_cast<int>(strlen("X")));
    TextOut(hdc, 30, 70, L"Y", static_cast<int>(strlen("Y")));

    //���ۿ� ����
    wchar_t editBoxCaption[2][8];
    swprintf(editBoxCaption[0], 8, L"%d", (int)particleSystem.GetPosition().x);
    swprintf(editBoxCaption[1], 8, L"%d", (int)particleSystem.GetPosition().y);

    HWND hEidt[2];
    for (int i = 0; i < 2; ++i)
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

    mapObject.InitObjects(); //map object �ʱ�ȭ
    particleSystem.CreateParticle();

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
            Render();
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

            wchar_t editBoxCaption[2][8];

            for (int i = 0; i < 2; ++i)
            {
                GetDlgItemText(WinAPI_hwnd, ICE_X + i, editBoxCaption[i], 8);
                //particleSystem.SetX(atoi(str)); �����ڵ尡 �ƴ�
            }
            particleSystem.SetPosition(Vector(_wtoi(editBoxCaption[0]), _wtoi(editBoxCaption[1]), 0));
           
            swprintf(editBoxCaption[0], 8, L"%d", (int)particleSystem.GetPosition().x);
            swprintf(editBoxCaption[1], 8, L"%d", (int)particleSystem.GetPosition().y);

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

    //particleSystem.DrawParticle(WinAPI_hwnd, hdc , hBufferDC);
    mapObject.DrawUpdate(hBufferDC);
    particleSystem.DrawParticle(hBufferDC);

    BitBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, hBufferDC, 0, 0, SRCCOPY);

    DeleteObject(hBufferBitmap);

    ReleaseDC(WinAPI_hwnd, hBufferDC);
    DeleteObject(hBufferDC);
    DeleteDC(hdc);
}

void WinAPI::FixedUpdate()
{
    for (int i = 0; i < particleSystem.GetParticleCollider().size(); ++i)
    {
        float time = 0.1f;
        CircleCollider *pCollider = particleSystem.GetParticleCollider()[i];
        //�ʱⰪ����
        pCollider->isCollision = false;
        pCollider->impactForces = Vector(0, 0, 0); //��ݷ� �ʱ�ȭ
        pCollider->countCollider = 0;
        colliderPlanetoCircle(pCollider, time);

        for (int j = 0; j < mapObject.GetCircleCollider().size(); ++j)
        {
            CircleCollider* pCircleObjectColldier = mapObject.GetCircleCollider()[j];
            colliderCircletoCircle(pCollider, pCircleObjectColldier,time);
        }

       
        //for (int j = 0; j < particleSystem.GetParticleCollider().size(); ++j)
        //{
        //    CircleCollider* pEnmyCollider = particleSystem.GetParticleCollider()[j];

        //    if (i == j)
        //        continue;
        //    //if(colliderCircletoCircle(pCollider, pEnmyCollider, time))
        //    //    pCollider->countCollider += 1;
        //}
    

        pCollider->CalcResistance(); //���׷� ���
        pCollider->UdpateRigidBody(time); //��ü����
    }

}

void WinAPI::colliderPlanetoCircle(CircleCollider* col, float time)
{
    //�ٴ� �浹
    if (col->position.y >= (700 - col->radius)) //�ݶ��̴��� ��ǥ�� �ٴ� + ���ݶ��̴� ���� ũ�ٸ�
    {
        Vector normal = Vector(0, 1, 0); //���� ����
        Vector velocityRelative = col->velocity; //���ӵ�
        float velocityRelativeNormal = velocityRelative * normal;
        if (velocityRelativeNormal > 0.0) {
            float j = -(velocityRelative * normal) * (0.2f + 1) * col->weight; //(joule ���� )
            Vector fi = normal; // ��ݷ� ����
            fi *= j / time; 
            col->impactForces += fi;
            col->position.y = 700 + col->radius;
            col->position.x = (700 + col->radius - col->prePosition.y) / (col->position.y - col->prePosition.y) * (col->position.x - col->prePosition.x) + col->prePosition.x;
            col->isCollision= true;
        }
    }
}

bool WinAPI::colliderCircletoCircle(CircleCollider* aCol, CircleCollider* bCol, float time)
{
    //obb �浹�� ���� 
    float roundDelta = aCol->radius + bCol->radius;  // r + r'
    Vector distance = aCol->position - bCol->position; //���� ���� �߽� �Ÿ� 
    float s = distance.Magnitude() - roundDelta; //

    if (s <= 1)
    {
        distance.Normalize();
        Vector normal = distance;
        Vector velocityRelative = aCol->velocity - bCol->velocity; //���ӵ�
        float velocityRelativeNormal = velocityRelative * normal; //����� ����

        if (velocityRelativeNormal < 0.0)
        {
            float j = -(velocityRelative * normal) * (0.2 + 1) / (1 / aCol->weight + 1 / bCol->weight); //0.2->��ݰ��
            Vector fi = normal;
            fi *= j / time;
            aCol->impactForces += fi;
            aCol->position -= normal * s;
            aCol->isCollision = true;
            return true;
        }
    }
    return false;
}
