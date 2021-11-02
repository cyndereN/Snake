// Snak3.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Snak3.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

int xP, yP, xE, yE;
const int size = 20;
int score;
int timeStep = 50;
int wndWidth = 0;								// 窗口尺寸
int wndHeight = 0;
char direction;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int RandPos()
{
    return rand() % 501;
}



void DrawBrick(HDC hdc, int x, int y, int s, int bPlayer)
{
    HBRUSH brush;
    brush = bPlayer ? CreateSolidBrush(RGB(255, 0, 0)) : CreateSolidBrush(RGB(0, 255, 0));
    RECT rc;
    rc.top = y - s / 2;
    rc.left = x - s / 2;
    rc.bottom = y + s / 2;
    rc.right = x + s / 2;
    FillRect(hdc, &rc, brush);
    DeleteObject(brush);
}

int IsCollider(int x1, int y1, int x2, int y2, int s)
{
    if (x1 + s<x2 || x1 - s>x2)
        return 0;
    if (y1 + s<y2 || y1 - s>y2)
        return 0;
    return 1;
}

void Collider()
{
    if (IsCollider(xP, yP, xE, yE, size))
    {
        score++;
        xE = RandPos(), yE = RandPos();
    }
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    xP = RandPos();
    xE = RandPos();
    yP = RandPos();
    yE = RandPos();

    score = 0;

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SNAK3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAK3));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_SNAK3));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SNAK3);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
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

    case WM_SIZE:			// 获取窗口的尺寸
        wndWidth = LOWORD(lParam);
        wndHeight = HIWORD(lParam);
        break;

    case WM_KEYDOWN:
        InvalidateRect(hWnd, NULL, TRUE);// 让窗口变为无效,从而触发重绘消息
        SetTimer(hWnd, 1, 100, NULL);
        switch (wParam)            // 用户控制主角进行上下左右移动
        {
        case VK_LEFT:
            direction = 'L';
            xP -= xP < 0 ? 0 : 10;
            break;
        case VK_RIGHT:
            direction = 'R';
            xP += xP > 500 ? 0 : 10;
            break;

        case VK_UP:
            direction = 'U';
            yP -= yP < 0 ? 0 : 10;
            break;

        case VK_DOWN:
            direction = 'D';
            yP += yP > 500 ? 0 : 10;
            break;
        }
        Collider();
        break;

    case WM_ERASEBKGND:		// 截获擦除背景消息,不擦除背景,避免闪烁
        break;

    case WM_TIMER:
        if (wParam == 1) // 如果是感兴趣的定时器(ID为1),则进行游戏更新
        {
            switch (direction) {
            case 'L':
                xP -= xP < 0 ? 0 : 10;
                Collider();
                break;
            case 'R':
                xP += xP > 500 ? 0 : 10;
                Collider();
                break;
            case 'U':
                yP -= yP < 0 ? 0 : 10;
                Collider();
                break;
            case 'D':
                yP += yP > 500 ? 0 : 10;
                Collider();
                break;
            }
            InvalidateRect(hWnd, NULL, TRUE); // 让窗口变为无效,从而触发重绘消息
        }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        //以下的步骤是为了避免产生屏幕闪烁, 将画面首先绘制到内存中, 然后一次性拷贝到屏幕上
        // 创建内存HDC
        HDC hdc = BeginPaint(hWnd, &ps);
        WCHAR str[16];
        swprintf(str, 16, L"Score: %d", score);
        TextOut(hdc, 0, 0, str, wcslen(str));
        DrawBrick(hdc, xE, yE, size, 0);
        DrawBrick(hdc, xP, yP, size, 1);

        HDC memHDC = CreateCompatibleDC(hdc);

        //获取客户区大小
        RECT rectClient;
        GetClientRect(hWnd, &rectClient);

        //创建位图
        HBITMAP bmpBuff = CreateCompatibleBitmap(hdc, wndWidth, wndHeight);
        HBITMAP pOldBMP = (HBITMAP)SelectObject(memHDC, bmpBuff);
        PatBlt(memHDC, 0, 0, wndWidth, wndHeight, WHITENESS);	// 设置背景为白

        DrawBrick(memHDC, xE, yE, size, 0);
        DrawBrick(memHDC, xP, yP, size, 1);
        swprintf(str, 16, L"Score: %d", score);
        TextOut(memHDC, 0, 0, str, wcslen(str));
        //拷贝内存HDC内容到实际HDC
        BOOL tt = BitBlt(hdc, rectClient.left, rectClient.top, wndWidth,
            wndHeight, memHDC, rectClient.left, rectClient.top, SRCCOPY);

        //内存回收
        SelectObject(memHDC, pOldBMP);
        DeleteObject(bmpBuff);
        DeleteDC(memHDC);


        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
        KillTimer(hWnd, 3);
        KillTimer(hWnd, 4);// 程序退出时，将定时器删除
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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