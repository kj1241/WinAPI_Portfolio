// AcidRain.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "AcidRain.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
int level;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

enum Scene
{
    start = 0,
    main,
    end
};



struct EnemyWord //단어 구조체
{
    TCHAR name[20]; //문자열
    int x;
    int y;
    RECT rect = {x,y,x+50,y+20}; //위치
    COLORREF color; //색 저장
    BOOL visible; // 유효성 검사
};
typedef struct EnemyWord Enemys;

Enemys Enemy[50];

TCHAR TextWords[30][15] =			//산성비 단어
{
    _T("culture"),
    _T("experience"),
    _T("education"),
    _T("symbol"),
    _T("effect"),
    _T("liberty"),
    _T("affair"),
    _T("comfort"),
    _T("tradition"),
    _T("subject"),
    _T("object"),
    _T("source"),
    _T("revolution"),
    _T("pollution"),
    _T("system"),
    _T("triumph"),
    _T("respect"),
    _T("communication"),
    _T("foundation"),
    _T("glory"),
    _T("situation"),
    _T("competition"),
    _T("prairie"),
    _T("effort"),
    _T("section"),
    _T("rein"),
    _T("solution"),
    _T("honour"),
    _T("unity"),
    _T("population")
};

void StartScene(HDC hdc);
void CreateEnemy(Enemys *Word, int i);
void DrawEnemy(HDC hdc, Enemys* word, int size);
void DrawUI(HDC hdc, int total, RECT rectView);
void EndScene(HDC hdc, TCHAR* word, int total);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ACIDRAIN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ACIDRAIN));

    MSG msg;

    // 기본 메시지 루프입니다:
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}

    while (TRUE) //GetMessage(&msg, NULL, 0, 0) < -하나씩 꺼내서 처리하다간 느림
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // PeekMessage는 메시지가 있으면 true를 리턴하고, 그렇지 않으면  false 를 리턴
        {
            if (msg.message == WM_QUIT) // _REMOVE	메시지를 읽은 후 큐에서 메시지를 제거한다.
                break;
            TranslateMessage(&msg); // 키보드 메시지를 번역
            DispatchMessage(&msg); // 메시지를 해당 윈도우 프로시저로 보냄
        }
    }

    return (int)msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ACIDRAIN));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ACIDRAIN);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, 0, 640, 480, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc; // 핸들 변수
    PAINTSTRUCT ps; // 클라이언트 그리기 변수

    static RECT rectView;  // 윈도우창
    RECT rtTitlebar = { 0, 0, 640, 20 }; //타이트바
   
    static Enemys enemy[30];
    static int CurrentEnemyNumber=0;

    static TCHAR GetWord[100] = {}; //입력받을 문자
    static int count=0;

    SIZE size;// 캐럿 사이즈
    
    static int total=0;
    static TCHAR resultTitle[10];

    static Scene scene;


    switch (message)
    {
    case WM_CREATE:  //초기화
    {
        GetClientRect(hWnd, &rectView);
 
        
        scene = start;

        //변수 초기화
        count = 0;
        for (int i = 0; i < 30; ++i)
        {
            CreateEnemy(&enemy[i], i);
        }

   



    }
    break;


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;


    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        
        if (scene == start)
        {
            StartScene(hdc);
        }
        else if (scene == main)
        {

            DrawEnemy(hdc, enemy, 30);

            Rectangle(hdc, rectView.left, rectView.bottom - 31, rectView.right, rectView.bottom);
            GetTextExtentPoint(hdc, GetWord, wcslen(GetWord), &size); //내가 쓴 단어의 높이와 너비
            Rectangle(hdc, rectView.left, rectView.bottom - 30, rectView.left + 100, rectView.bottom); //캐럿 담을 사각박스그려주기
            TextOut(hdc, rectView.left + 5, rectView.bottom - 20, GetWord, wcslen(GetWord)); //단어를 출력

            SetCaretPos(rectView.left + 5 + size.cx, rectView.bottom - 20);//캐럿 위치

            DrawUI(hdc, total, rectView);
            //DrawGround(hdc, rtTitlebar);
        }
        else if (scene == end)
        {

            EndScene(hdc, resultTitle, total);
        }



        EndPaint(hWnd, &ps);
    }
    break;

    case WM_TIMER: //윈도우거 처음생성되고 시간지나면 
    {

        switch(wParam)
        {
        case 1: //현재 프레임
        {
            for (int i = 0; i < 30; ++i)
            {
                if (enemy[i].visible == true)
                {
                    if (enemy[i].y < rectView.bottom - 20)
                    {
                        enemy[i].y += 5;
                        enemy[i].rect = { enemy[i].x,enemy[i].y,(long)(enemy[i].x + (wcslen(enemy[i].name)*8)),enemy[i].y + 20};
                    }
                    else
                    {
                        DestroyCaret();	 //캐럿 파괴	
                        scene = end;
                        wcscpy(resultTitle, _T("게임 실패"));
                        wcscpy(enemy[0].name, _T(""));
                    }
                }
            }
            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;

        case 2: //3초마다 단어 갱신
            if (CurrentEnemyNumber < 30)
            {
                enemy[CurrentEnemyNumber].visible = true;
                CurrentEnemyNumber++;
            }
        }
        break;

       
       

    }
    break;
    case WM_CHAR:  //키보드의 값을 아스킷 코드로 입력받음
    {
        if (scene == main)
        {

            int nKey;
            nKey = (int)wParam;
            if (nKey == VK_RETURN) //엔터를 누르면
            {
                for (int i = 0; i < 30; ++i) //모든적을 상대로
                {
                    if (!wcscmp(GetWord, enemy[i].name) && enemy[i].visible) //비교해서 같으면 && 보이면
                    {
                        enemy[i].visible = FALSE;  //안보이게하고
                        total += 10; //점수 더해준다.
                        InvalidateRect(hWnd, &rectView, TRUE); //다시그려준다.

                        if (total == 50)
                        {
                            DestroyCaret();	 //캐럿 파괴	
                            scene = end;
                            wcscpy(resultTitle, _T("게임 성공"));
                        }

                    }
                }
                //일딴 받은 단어는 지워짐
                wcscpy(GetWord, _T(""));
                count = 0;
            }
            else if (nKey == VK_BACK) //백스페이스를 누르면
            {
                if (count > 0)
                {
                    GetWord[wcslen(GetWord) - 1] = '\0';
                    count--;
                }
            }
            else //문자가 입력되면 문자열에 추가
            {
                GetWord[count] = wParam;
                count++;
                GetWord[count] = '\0';
            }
            //InvalidateRect(hWnd, &rectView, TRUE);
        }
    }
    break;

    case WM_LBUTTONDOWN:
    {
    
  
        if (scene == start)
        {
            scene = main;
            CreateCaret(hWnd, NULL, 5, 15); //캐럿 만들고
            ShowCaret(hWnd);// 캐럿 그리기 시작
            SetTimer(hWnd, 1, 30, NULL);
            SetTimer(hWnd, 2, 3000, NULL);
            InvalidateRect(hWnd, &rectView, TRUE);
        }
    


    }
    break;


    case WM_DESTROY:
        HideCaret(hWnd); //캐럿을 숨기고
        KillTimer(hWnd, 1); //타이머 1번 파괴
        KillTimer(hWnd, 2); //타이머 2번 파괴
        DestroyCaret();	 //캐럿 파괴	
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void CreateEnemy(Enemys *Word, int i) //적 만들기
{
    int tempEnemyWordNumber; //랜덤 단어 선택
    int eventEnemyNumber;

    tempEnemyWordNumber = rand() % 30; //단어선택
    eventEnemyNumber = rand() % 50; //이벤트 단어인지 확인

  
    wcscpy(Word->name, TextWords[tempEnemyWordNumber]); //단어쓰기
    Word->color = RGB(0, 0, 0);

    Word->x = ((i*20)* tempEnemyWordNumber)%620;
    Word->y = 0;
    Word->rect = { Word->x,Word->y, (long)(Word->x + (wcslen(Word->name) * 8)),Word->y + 20 };

    Word->visible = false;
}

void DrawEnemy(HDC hdc, Enemys *word, int size) //적 만들기
{
    HBRUSH curBrush=NULL, oldBrush = NULL;
    HFONT curFont = NULL, oldFont = NULL;

    for (int i = 0; i < size; ++i)
    {
        if (word[i].visible == true)
        {
            curFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("맑은 고딕"));
            oldFont = (HFONT)SelectObject(hdc, curFont);

            if (word[i].color == RGB(252, 0, 0))
            {
                SetTextColor(hdc, RGB(252, 0, 0));
            }
            DrawText(hdc, word[i].name, wcslen(word[i].name), &word[i].rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            SetTextColor(hdc, RGB(0,0,0));
           
            SelectObject(hdc, oldFont);
            DeleteObject(curFont);
        }
    }
}


void DrawUI(HDC hdc, int total, RECT rectView)
{
    HBRUSH curBrush = NULL, oldBrush = NULL;
    HFONT curFont = NULL, oldFont = NULL;
    TCHAR totalName[20] = _T("Total socre :");
    TCHAR tempTotal[8];

    curFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("맑은 고딕"));
    oldFont = (HFONT)SelectObject(hdc, curFont);

    RECT rect = { rectView.right - 160,rectView.bottom - 25 ,rectView.right ,rectView.bottom -5};
    wcscat(totalName, _itow(total, tempTotal,10));
    DrawText(hdc, totalName, wcslen(totalName), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    SelectObject(hdc, oldFont);
    DeleteObject(curFont);


}




void StartScene(HDC hdc)
{
    HBRUSH curBrush = NULL, oldBrush = NULL;
    HFONT curFont = NULL, oldFont = NULL;

    TCHAR titleName[20] = _T("산성비");
    TCHAR makeName[20] = _T("-by KJ-");
    TCHAR StartName[40] = _T("시작하려면 '마우스 좌클릭'을 누르세요!");

    curFont = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("굴림"));
    oldFont = (HFONT)SelectObject(hdc, curFont);

    SetTextColor(hdc, RGB(0, 250, 0));

    RECT rect = { 190,100 ,390 ,150 };
    DrawText(hdc, titleName, wcslen(titleName), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    SetTextColor(hdc, RGB(0, 0, 0));
    SelectObject(hdc, oldFont);
    DeleteObject(curFont);

    curFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("맑은 고딕"));
    oldFont = (HFONT)SelectObject(hdc, curFont);

    SetTextColor(hdc, RGB(0, 30, 200));

    RECT rect2 = { 190,180 ,390 ,210 };
    DrawText(hdc, makeName, wcslen(makeName), &rect2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    SetTextColor(hdc, RGB(0, 0, 0));
    SelectObject(hdc, oldFont);
    DeleteObject(curFont);


    curFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("궁서체"));
    oldFont = (HFONT)SelectObject(hdc, curFont);

    SetTextColor(hdc, RGB(0, 0, 0));

    RECT rect3 = { 90,280 ,490 ,320 };
    DrawText(hdc, StartName, wcslen(StartName), &rect3, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    SetTextColor(hdc, RGB(0, 0, 0));
    SelectObject(hdc, oldFont);
    DeleteObject(curFont);
}

void EndScene(HDC hdc, TCHAR* word ,int total)
{
    HBRUSH curBrush = NULL, oldBrush = NULL;
    HFONT curFont = NULL, oldFont = NULL;

    TCHAR titleName[20] = {};
    TCHAR totalName[20] = _T("총 점수: ");
    TCHAR tempTotal[8];


    curFont = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("굴림"));
    oldFont = (HFONT)SelectObject(hdc, curFont);
    
    wcscpy(titleName, word);

    SetTextColor(hdc, RGB(0, 250, 0));

    RECT rect = { 90,100 ,490 ,150 };
    DrawText(hdc, titleName, wcslen(titleName), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    SetTextColor(hdc, RGB(0, 0, 0));
    SelectObject(hdc, oldFont);
    DeleteObject(curFont);

    curFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("맑은 고딕"));
    oldFont = (HFONT)SelectObject(hdc, curFont);

    SetTextColor(hdc, RGB(0, 0, 0));
    wcscat(totalName, _itow(total, tempTotal, 10));

    RECT rect2 = { 190,250 ,390 ,300 };
    DrawText(hdc, totalName, wcslen(totalName), &rect2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    SetTextColor(hdc, RGB(0, 0, 0));
    SelectObject(hdc, oldFont);
    DeleteObject(curFont);

  

 
}