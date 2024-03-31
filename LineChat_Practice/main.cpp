#include <Windows.h>


const int ChartWidth = 600;
const int ChartHeight = 400;
const int NumPoints = 10;
const int Data[] = { 20, 30, 40, 50, 60, 55, 45, 35, 25, 20 };


LRESULT CALLBACK ChartWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

    
        MoveToEx(hdc, 50, ChartHeight - 50, NULL);
        LineTo(hdc, ChartWidth - 50, ChartHeight - 50);
        MoveToEx(hdc, 50, ChartHeight - 50, NULL);
        LineTo(hdc, 50, 50);

      
        int xStep = (ChartWidth - 100) / (NumPoints - 1);
        int yStep = (ChartHeight - 100) / 100;
        int x = 50, y = ChartHeight - 50 - Data[0] * yStep;
        for (int i = 1; i < NumPoints; i++)
        {
            int nextY = ChartHeight - 50 - Data[i] * yStep;
            MoveToEx(hdc, x, y, NULL);
            LineTo(hdc, x + xStep, nextY);
            x += xStep;
            y = nextY;
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    WNDCLASS wc = {};
    wc.lpfnWndProc = ChartWndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"ChartWindowClass";
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(wc.lpszClassName, L"Line Chart",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        ChartWidth, ChartHeight, NULL, NULL, hInstance, NULL);


    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}