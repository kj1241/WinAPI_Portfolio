#include "stdafx.h"
#include "WinAPI.h"

HWND WinAPI::WinAPI_hwnd = nullptr; //핸들 값 초기화
HINSTANCE WinAPI::hInst = nullptr;

WinAPI* WinAPI::pWinAPI = nullptr;

static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return WinAPI::pWinAPI->WindowProc(hWnd, msg, wParam, lParam);
}

WinAPI::WinAPI(): rect({ 250,0,1150,750 })//생성자
{
    pWinAPI = this;

}

WinAPI::~WinAPI() //소멸자
{
    pWinAPI = nullptr;

}

bool WinAPI::Init(HINSTANCE hInstance, int nCmdShow)
{
    //명령줄 매게변수 구분 
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    LocalFree(argv);
    hInst = hInstance;

    // 윈도우 클라스 초기화
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = ::wndProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"Particle_ToyProjcet";
    windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 1200,800 };//윈도우 창범위
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // 창과 핸들 만듦
    WinAPI_hwnd = CreateWindow(
        windowClass.lpszClassName,
        L"Particle_ToyProjcet",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,        // 부모창 없음
        nullptr,        // 메뉴 사용하지 않음
        hInstance,
        nullptr); //프로시저에 DirectXBase 클래스 주소 넘기기

    ShowWindow(WinAPI_hwnd, nCmdShow); //윈도우 보여주기

    HDC hdc = GetDC(WinAPI_hwnd);
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, false, false, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial"); //폰트 만들기
    HFONT OldFont = (HFONT)SelectObject(hdc, hFont);

    TextOut(hdc, 30, 30, L"X", static_cast<int>(strlen("X")));
    TextOut(hdc, 30, 70, L"Y", static_cast<int>(strlen("Y")));

    //버퍼에 쓰기
    wchar_t editBoxCaption[2][8];
    swprintf(editBoxCaption[0], 8, L"%d", (int)particleSystem.GetPosition().x);
    swprintf(editBoxCaption[1], 8, L"%d", (int)particleSystem.GetPosition().y);

    HWND hEidt[2];
    for (int i = 0; i < 2; ++i)
    {
        //에디트 박스
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
        L"실행",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | ES_CENTER,
        30, 270, 100, 30,
        WinAPI_hwnd,
        reinterpret_cast<HMENU>(ICC_EXCUTION),
        hInst,
        nullptr);

    SelectObject(hdc, OldFont);
    DeleteObject(hFont);
    ReleaseDC(WinAPI_hwnd, hdc);

    mapObject.InitObjects(); //map object 초기화
    particleSystem.CreateParticle();

    return true;
}

//WinAPI실행
int WinAPI::Run()
{
    // 메인 루프
    MSG msg = { 0 };

    while (msg.message != WM_QUIT) //메시지가 winAPI종료가 아니라면
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // 메시지가 있으면 처리
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else // 실수 directx 를 하다보니 이곳에 만들면 화면 움직일때 페인트를 다시 안함으로 문제가 있을수 있네
        {
            FixedUpdate();
            Render();
        }
    }

    return static_cast<char>(msg.wParam);    // WM_QUIT 메시지로 반환
}

//핸들값을 얻기 위해서
HWND WinAPI::GetHwnd()
{
    return WinAPI_hwnd;
}

//윈 프로시져
LRESULT WinAPI::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //메시지
    switch (message)
    {
    case WM_CREATE: //창 만들기 전부터 들어옴 빠름
    {
        // 윈도우 만들시 DirectX12Base 를 저장
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }
    return 0;

    case WM_COMMAND: //커멘드 함수가 들어왔을때
        switch (LOWORD(wParam)) //메세지는 wParam로 처리되니
        {
            //실행버튼을 누르면
        case ICC_EXCUTION:

            wchar_t editBoxCaption[2][8];

            for (int i = 0; i < 2; ++i)
            {
                GetDlgItemText(WinAPI_hwnd, ICE_X + i, editBoxCaption[i], 8);
                //particleSystem.SetX(atoi(str)); 유니코드가 아님
            }
            particleSystem.SetPosition(Vector(_wtoi(editBoxCaption[0]), _wtoi(editBoxCaption[1]), 0));
           
            swprintf(editBoxCaption[0], 8, L"%d", (int)particleSystem.GetPosition().x);
            swprintf(editBoxCaption[1], 8, L"%d", (int)particleSystem.GetPosition().y);

            particleSystem.CreateParticle();
            break;

        case ID_EXID://탈출
            PostQuitMessage(0);
            break;
        }

    case WM_KEYDOWN: //키버튼이 눌렸으면
        return 0;

    case WM_KEYUP:  //키버튼이  때어젔으면
        return 0;

    //case WM_PAINT: //사용하지 않는 이유: 이함수는 다시 그리는용인데 run()함수에서 처리하고 있기 때문
    //    return 0;

    case WM_DESTROY: //파괴되었을때
        PostQuitMessage(0);
        return 0;
    }

    // 디폴트값 대신 모든 메시지 처리
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

    Rectangle(hdc, r->left, r->top, r->right, r->bottom); //사각형그리기

    SelectObject(hdc, OldBrush);
    SelectObject(hdc, OldPen);
    DeleteObject(CurrentBrush);
    DeleteObject(CurrentPen);
}

void WinAPI::Render()
{
    HDC hdc = GetDC(WinAPI_hwnd); // HDC가저오기
    HDC hBufferDC = CreateCompatibleDC(hdc); //그래픽 드라이버 정보로 새로만듬, 이중버퍼링
    BYTE* lpbits = NULL;
    
     HBITMAP hBufferBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
    //HBITMAP hBufferBitmap = CreateDIBSection(hBufferDC, lpBackBufferBitmapInfo, RGB(0,0,0), (void**)&lpbits, NULL, 0); //장치와 독립적으로

    SelectObject(hBufferDC, hBufferBitmap); //비트맵으로 교체해주기
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
        //초기값설정
        pCollider->isCollision = false;
        pCollider->impactForces = Vector(0, 0, 0); //충격량 초기화
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
    

        pCollider->CalcResistance(); //저항력 계산
        pCollider->UdpateRigidBody(time); //강체역학
    }

}

void WinAPI::colliderPlanetoCircle(CircleCollider* col, float time)
{
    //바닥 충돌
    if (col->position.y >= (700 - col->radius)) //콜라이더의 좌표는 바닥 + 원콜라이더 보다 크다면
    {
        Vector normal = Vector(0, 1, 0); //수직 백터
        Vector velocityRelative = col->velocity; //상대속도
        float velocityRelativeNormal = velocityRelative * normal;
        if (velocityRelativeNormal > 0.0) {
            float j = -(velocityRelative * normal) * (0.2f + 1) * col->weight; //(joule 총일 )
            Vector fi = normal; // 충격량 백터
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
    //obb 충돌로 쉽게 
    float roundDelta = aCol->radius + bCol->radius;  // r + r'
    Vector distance = aCol->position - bCol->position; //실제 구의 중심 거리 
    float s = distance.Magnitude() - roundDelta; //

    if (s <= 1)
    {
        distance.Normalize();
        Vector normal = distance;
        Vector velocityRelative = aCol->velocity - bCol->velocity; //상대속도
        float velocityRelativeNormal = velocityRelative * normal; //상대노멀 백터

        if (velocityRelativeNormal < 0.0)
        {
            float j = -(velocityRelative * normal) * (0.2 + 1) / (1 / aCol->weight + 1 / bCol->weight); //0.2->충격계수
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
