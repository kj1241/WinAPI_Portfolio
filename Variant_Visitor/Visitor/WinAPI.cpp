#include "stdafx.h"
#include "WinAPI.h"

HWND WinAPI::WinAPI_hwnd = nullptr;
HWND WinAPI::Child_hwnd = nullptr;
WinAPI* WinAPI::pWinAPI = nullptr;

static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return WinAPI::pWinAPI->WindowProc(hWnd, msg, wParam, lParam);
}

static LRESULT CALLBACK childProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return WinAPI::pWinAPI->ChildProc(hWnd, msg, wParam, lParam);
}


static unsigned __stdcall ChartTread(LPVOID arg)
{
    return WinAPI::pWinAPI->ThreadsFunction(arg);
}

unsigned __stdcall WinAPI::ThreadsFunction(LPVOID arg)
{
    threadArg *_arg= (threadArg*)arg;
    
    //가변적으로 만들었던 부모 위치를 얻기위해서
    RECT parerentPoistion;
    GetWindowRect(WinAPI_hwnd, &parerentPoistion);

    if(!InitChartWindow(_arg, parerentPoistion))
        return 0;
    return RunChartWindow();
}



bool WinAPI::InitChartWindow(threadArg* _arg, RECT parerentPoistion)
{
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = ::childProc;
    windowClass.hInstance = _arg->hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"Visitor_test";
    windowClass.lpszMenuName = nullptr;
    RegisterClassEx(&windowClass);

    Child_hwnd = CreateWindow(
        windowClass.lpszClassName,
        L"Memory",
        WS_OVERLAPPEDWINDOW,
        parerentPoistion.right,
        parerentPoistion.top,
        400,
        800,
        WinAPI_hwnd,        // 부모창 없음
        nullptr,        // 메뉴 사용하지 않음
        _arg->hInstance,
        nullptr); //프로시저에 DirectXBase 클래스 주소 넘기기

    ShowWindow(Child_hwnd, _arg->nCmdShow); //윈도우 보여주기

    HDC hdc = GetDC(Child_hwnd);
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, false, false, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial"); //폰트 만들기
    HFONT OldFont = (HFONT)SelectObject(hdc, hFont);

    TextOut(hdc, 30, 30, L"현재 로직 동작 시간", static_cast<int>(wcslen(L"현재 로직 동작 시간")));
    TextOut(hdc, 30, 70, L"현재 가상 메모리", static_cast<int>(wcslen(L"현재 가상 메모리")));
    TextOut(hdc, 30, 110, L"현재 물리 메모리", static_cast<int>(wcslen(L"현재 물리 메모리")));

    HWND hEidt[6];
    
    for (int i = 0; i < 3; ++i)
    {
        //에디트 박스
        hEidt[i] = CreateWindow(
            L"edit",
            editBoxCaption[i],
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            200, 30 + (40 * i), 100, 25,
            Child_hwnd,
            reinterpret_cast<HMENU>(EditBox0 + i),
            _arg->hInstance,
            nullptr);
    }

    TextOut(hdc, 3, 200, L"10000", static_cast<int>(wcslen(L"10000")));
    TextOut(hdc, 3, 350, L"5000", static_cast<int>(wcslen(L"5000")));
    TextOut(hdc, 3, 500, L"0", static_cast<int>(wcslen(L"0")));
    TextOut(hdc, 30, 180, L"로직 시간 변화량 그래프(ms)", static_cast<int>(wcslen(L"로직 시간 변화량 그래프(ms)")));


    //세로 축그려주기
    MoveToEx(hdc, 50, 525, NULL);
    line(hdc, 50, 200, RGB(0, 0, 0), 3);
    //가로
    MoveToEx(hdc, 25, 500, NULL);
    line(hdc, 350, 500, RGB(0, 0, 0), 3);

    SelectObject(hdc, OldFont);
    DeleteObject(hFont);
    ReleaseDC(Child_hwnd, hdc);
    
    return true;
}

int WinAPI::RunChartWindow()
{
    MSG msg = { 0 };

    while (msg.message != WM_QUIT) //메시지가 winAPI종료가 아니라면
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // 메시지가 있으면 처리
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else //
        {
            for (int i = 0; i < MemoryQueue->size(); ++i)
            {
                MemoryStruct memory = MemoryQueue->front();
                MemoryQueue->pop();

                swprintf(editBoxCaption[0], 20, L"%d", memory.logicTime);
                swprintf(editBoxCaption[1], 20, L"%d", memory.vitrualMemory);
                swprintf(editBoxCaption[2], 20, L"%d", memory.physicsMemory);

                SetDlgItemText(Child_hwnd, EditBox0, editBoxCaption[0]);
                SetDlgItemText(Child_hwnd, EditBox0 + 1, editBoxCaption[1]);
                SetDlgItemText(Child_hwnd, EditBox0 + 2, editBoxCaption[2]);


                if (lineChart.size() < 10)
                    lineChart.push_back(memory.logicTime);
                else
                {
                    lineChart.pop_front();
                    lineChart.push_back(memory.logicTime);
                }

                HDC hdc = GetDC(Child_hwnd);
                Square s(hdc, 200, 350, RGB(255, 255, 255), 150);
                std::visit(Draw{}, ShapeVariant{ s });

                //세로 축그려주기
                MoveToEx(hdc, 50, 525, NULL);
                line(hdc, 50, 200, RGB(0, 0, 0), 3);
                //가로
                MoveToEx(hdc, 25, 500, NULL);
                line(hdc, 350, 500, RGB(0, 0, 0), 3);


                int x = 50;
                int nextY = lineChart.front();
                //int max= lineChart.front(), min= lineChart.front(); 
                for (auto it = lineChart.begin(); it != lineChart.end(); ++it)
                {
                    if (it == lineChart.begin())
                        continue;

                    if (nextY > 10000)
                        MoveToEx(hdc, x, 200, NULL);
                    else if (nextY < 0)
                        MoveToEx(hdc, x, 500, NULL);
                    else
                        MoveToEx(hdc, x, 500 - (nextY *3/100), NULL);
                        //MoveToEx(hdc, x, 500 - ((nextY - 1000) / 30), NULL);
                    
                    if (*it > 10000)
                        line(hdc, x + 30, 200, RGB(255, 0, 0),3);
                    else if (*it < 0)
                        line(hdc, x + 30, 500, RGB(0, 0, 255),3);
                    else
                        line(hdc, x+30, 500 - ((*it) * 3 / 100), RGB(0, 250, 250), 3);
                        //line(hdc, x + 30, 500 - ((*it) - 1000) / 30, RGB(0, 250, 250),3);

                    x += 30;
                    nextY = (*it);
                }
            }
        }
        Sleep(1);//제어권 넘기기
    }

    return static_cast<char>(msg.wParam);    // WM_QUIT 메시지로 반환
}


WinAPI::WinAPI()
{
	pWinAPI = this;
    MemoryQueue = new  std::queue<MemoryStruct>();
}

WinAPI::~WinAPI()
{
	pWinAPI = nullptr;
    if (MemoryQueue != nullptr)
        MemoryQueue = nullptr;
}

bool WinAPI::Init(HINSTANCE hInstance, int nCmdShow)
{
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    LocalFree(argv);

    // 윈도우 클라스 초기화
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = ::wndProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"Visitor_test";
    windowClass.lpszMenuName =nullptr;
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 1200,800 };//윈도우 창범위
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // 창과 핸들 만듦
    WinAPI_hwnd = CreateWindow(
        windowClass.lpszClassName,
        L"Visitor_test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,        // 부모창 없음
        nullptr,        // 메뉴 사용하지 않음
        hInstance,
        nullptr); //프로시저에 DirectXBase 클래스 주소 넘기기

    threadArg arg;
    arg.hInstance = hInstance;
    arg.nCmdShow = nCmdShow;

    chartThreadHandle = (HANDLE)_beginthreadex(nullptr, 0, ChartTread, &arg, 0, reinterpret_cast<unsigned int*>(&dwTreadID));

 
    ShowWindow(WinAPI_hwnd, nCmdShow); //윈도우 보여주기
    InitShape();
 
    return true;
}

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
            PROCESS_MEMORY_COUNTERS_EX pmc;
            memset(&pmc, 0, sizeof(PROCESS_MEMORY_COUNTERS_EX));
            pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

            clock_t start, end;
            double duration;

            start = std::clock();
            {
                //FixedUpdate();
                Render();
            }
            end = std::clock();
            GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));

            MemoryStruct memory;
            memory.logicTime = end - start;
            memory.vitrualMemory = pmc.PrivateUsage;
            memory.physicsMemory = pmc.WorkingSetSize;

            MemoryQueue->push(memory);
        }
    }

    return static_cast<char>(msg.wParam);    // WM_QUIT 메시지로 반환
}

HWND WinAPI::GetHwnd()
{
    return WinAPI_hwnd;
}

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

    case WM_COMMAND:
        return 0;

    case WM_KEYDOWN: 
        return 0;

    case WM_KEYUP: 
        return 0;

        //case WM_PAINT: 
        //    return 0;

    case WM_DESTROY: //파괴되었을때
        PostQuitMessage(0);
        return 0;
    }

    // 디폴트값 대신 모든 메시지 처리
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT WinAPI::ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //메시지
    switch (message)
    {
    case WM_CREATE: //창 만들기 전부터 들어옴 빠름
    {
 
    }
    return 0;


        //case WM_PAINT: 
        //    return 0;

    case WM_DESTROY: //파괴되었을때
        PostQuitMessage(0);
        return 0;
    }

    // 디폴트값 대신 모든 메시지 처리
    return DefWindowProc(hWnd, message, wParam, lParam);
}



void WinAPI::Render()
{
    //HDC hdc = GetDC(WinAPI_hwnd);

    //_Rectangle s(hdc, 600, 400, RGB(255, 255, 255), 600,400);
    //s.Accept(Draw{});
    DrawAllShape(shapes);
}

void WinAPI::FixedUpdate()
{
    Translate(shapes);
}



void WinAPI::InitShape()
{
    HDC hdc = GetDC(WinAPI_hwnd);

    _Rectangle s(hdc, 600, 400, RGB(255, 255, 255), 600,400);
    std::visit(Draw{}, ShapeVariant{ s });

    for (int i = 0; i < 100000; ++i)
    {
        switch (rand() % 4)
        {
        case 0:
            shapes.emplace_back(Circle(hdc,(rand()%12000)/10, (rand()%8000)/10,RGB(rand()%255, rand()%255, rand()%255), rand()%20));
            break;
        case 1:
            shapes.emplace_back(Square(hdc, (rand() % 12000) / 10, (rand() % 8000) / 10, RGB(rand() % 255, rand() % 255, rand() % 255), rand() % 20));
            break;
        case 2:
            shapes.emplace_back(_Ellipse(hdc, (rand() % 12000) / 10, (rand() % 8000) / 10, RGB(rand() % 255, rand() % 255, rand() % 255), rand() % 20,rand()%20));
            break;
        case 3:
            shapes.emplace_back(_Rectangle(hdc, (rand() % 12000) / 10, (rand() % 8000) / 10, RGB(rand() % 255, rand() % 255, rand() % 255), rand() % 20,rand()%20));
            break;
        }
    }


}

void WinAPI::line(HDC hdc,int x, int y, COLORREF color,int thk)
{
    HBRUSH		hBrush, OldBrush; //브러쉬
    HPEN		hPen, OldPen;  //팬 선의 상태 굵기 점선 등
    COLORREF	FColor = color;
    COLORREF	BColor = color;

    hBrush = CreateSolidBrush(BColor);
    OldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
    //hPen = CreatePen(PS_SOLID, thk, FColor); 
    hPen = CreatePen(PS_SOLID, thk, FColor); //팬 핸들 만들기
    OldPen = reinterpret_cast<HPEN>(SelectObject(hdc, hPen)); //화면에 등록하고 등록했던핸들을 반환

    LineTo(hdc, x, y); //선그리

    SelectObject(hdc, OldBrush);
    SelectObject(hdc, OldPen); //저장했던 팬을 다시 등록
    DeleteObject(hBrush);
    DeleteObject(hPen); //새로 만든 팬의 핸들을 지워버린다
}


