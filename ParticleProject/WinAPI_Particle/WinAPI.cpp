#include "stdafx.h"
#include "WinAPI.h"

HWND WinAPI::WinAPI_hwnd = nullptr; //핸들 값 초기화
HINSTANCE WinAPI::hInst = nullptr;

WinAPI::WinAPI() //생성자
{
   

}

WinAPI::~WinAPI() //소멸자
{

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
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"Particle_ToyProjcet";
    windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 700,450 };//윈도우 창범위
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

    //정적 텍스트
    TextOut(hdc, 30, 30, L"X", static_cast<int>(strlen("X")));
    TextOut(hdc, 30, 70, L"Y", static_cast<int>(strlen("Y")));
    TextOut(hdc, 30, 110, L"v0", static_cast<int>(strlen("v0")));
    TextOut(hdc, 30, 150, L"Duration", static_cast<int>(strlen("Duration")));
    TextOut(hdc, 30, 190, L"Gravity", static_cast<int>(strlen("Gravity")));
    TextOut(hdc, 30, 230, L"Angle", static_cast<int>(strlen("Angle")));

    //버퍼에 쓰기
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

    //HDC hdc = GetDC(WinAPI_hwnd);
    //HDC hBufferDC = CreateCompatibleDC(hdc);
    //HBITMAP hBufferBitmap = CreateCompatibleBitmap(hdc, 400, 400);
    //ReleaseDC(WinAPI_hwnd, hdc);
    //SelectObject(hBufferDC, hBufferBitmap);

    return true;
}

//WinAPI실행
int WinAPI::Run()
{
    PAINTSTRUCT ps;
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
            HDC hdc = GetDC(WinAPI_hwnd); // HDC가저오기
            HDC hBufferDC = CreateCompatibleDC(hdc); //그래픽 드라이버 정보로 새로만듬

            particleSystem.DrawParticle(WinAPI_hwnd, hdc , hBufferDC);
            ReleaseDC(WinAPI_hwnd, hBufferDC);
            DeleteObject(hBufferDC);
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
    PAINTSTRUCT ps;

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
            //컨트롤박스 문자열 읽기

            wchar_t editBoxCaption[6][8];

            for (int i = 0; i < 6; ++i)
            {
                GetDlgItemText(WinAPI_hwnd, ICE_X + i, editBoxCaption[i], 8);
                //particleSystem.SetX(atoi(str)); 유니코드가 아님
            }
            //이렇게 안쓸려고 요즘 오퍼레이터 쓰던데.. 연습해야되는데 옜날사람이라
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


