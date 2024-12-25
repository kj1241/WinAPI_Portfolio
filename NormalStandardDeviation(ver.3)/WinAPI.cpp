#include "stdafx.h"
#include "WinAPI.h"

HWND WinAPI::WinAPI_hwnd = nullptr; //핸들 값 초기화
HINSTANCE WinAPI::hInst = nullptr;

WinAPI* WinAPI::pWinAPI = nullptr;

static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return WinAPI::pWinAPI->WindowProc(hWnd, msg, wParam, lParam);
}

WinAPI::WinAPI() : rect({ 0,0,1000,600 })//생성자
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
    windowClass.lpszClassName = L"NormalStandardDeviation";
    windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 1250,600 };//윈도우 창범위
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // 창과 핸들 만듦
    WinAPI_hwnd = CreateWindow(
        windowClass.lpszClassName,
        L"NormalStandardDeviation",
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


    TextOut(hdc, 1020, 70, L"xMin", static_cast<int>(strlen("xMin")));
    TextOut(hdc, 1020, 110, L"xMax", static_cast<int>(strlen("xMax")));
    TextOut(hdc, 1020, 150, L"xStep", static_cast<int>(strlen("xStep")));

    TextOut(hdc, 1020, 190, L"mean", static_cast<int>(strlen("mean")));
    TextOut(hdc, 1020, 230, L"minutes", static_cast<int>(strlen("minutes")));

    TextOut(hdc, 1020, 270, L"step", static_cast<int>(strlen("step")));
    TextOut(hdc, 1020, 310, L"yScaling", static_cast<int>(strlen("yScaling")));
    TextOut(hdc, 1020, 350, L"stddev", static_cast<int>(strlen("stddev")));
   
    ////버퍼에 쓰기
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
        //에디트 박스
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
        L"Button",  // 컨트롤 클래스: 버튼 (체크박스도 버튼 클래스 사용)
        L"전체구간으로 출력",  // 체크박스 텍스트
        WS_VISIBLE | WS_CHILD | BS_CHECKBOX,  // 체크박스 스타일
        1020, 30, 150, 30,  // 위치 및 크기
        WinAPI_hwnd,
        reinterpret_cast<HMENU>(ICC_CHECKBOX),  // 체크박스 ID
        hInst,
        nullptr);

    hButton = CreateWindow(
        L"Button",
        L"실행",
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
            //Render();
        }
    }
    
    sd.CloseStandarDeviation();

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
    HDC hdc = GetDC(WinAPI_hwnd);
    double value;
    RECT rect2 = { 1130, 390, 1230, 420 };
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
            wchar_t editBoxCaption[8][11];

            for (int i = 0; i < 8; ++i)
            {
                GetDlgItemText(WinAPI_hwnd, ICE_XMIN + i, editBoxCaption[i], 11);
                //particleSystem.SetX(atoi(str)); 유니코드가 아님
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

        case ICC_CHECKBOX: {  // 체크박스 ID가 ICC_CHECKBOX일 때
            HWND hCheckbox = (HWND)lParam;
            int state = SendMessage(hCheckbox, BM_GETCHECK, 0, 0);  // 체크박스 상태 가져오기

            // 상태를 반전시킴 (체크/언체크 토글)
            if (state == BST_CHECKED) {
                // 현재 체크 상태이므로 체크 해제
                SendMessage(hCheckbox, BM_SETCHECK, BST_UNCHECKED, 0);
                sd.SetIsMaxProbabilityValue(false);
            }
            else {
                // 현재 체크 해제 상태이므로 체크
                SendMessage(hCheckbox, BM_SETCHECK, BST_CHECKED, 0);
                sd.SetIsMaxProbabilityValue(true);
            }


            break;
        }

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

std::string WinAPI::convertToTime(double time)
{
    // 정수 부분은 시간, 소수점 부분은 분으로 변환
    int hours = static_cast<int>(time);
    int minutes = static_cast<int>(std::round((time - hours) * 60));
    int seconds = 0; // 항상 00초로 설정

    // minutes가 60 이상이면 올바르게 조정
    if (minutes >= 60) {
        hours += minutes / 60;
        minutes %= 60;
    }

    // HH:MM:SS 형식으로 변환
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
    //    //초기값설정
    //    pCollider->isCollision = false;
    //    pCollider->impactForces = Vector(0, 0, 0); //충격량 초기화
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


    //    pCollider->CalcResistance(); //저항력 계산
    //    pCollider->UdpateRigidBody(time); //강체역학
    //}

}
