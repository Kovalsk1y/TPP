
#include <windows.h>

#define ID_IMAGE_DRAW   1001
#define ID_COLOR_CHANGE 1002
#define ID_ADD_OBJECT   1003
#define ID_ABOUT        1004
#define ID_EXIT         1005

static bool g_draw = false;
static bool g_extra = false;
static COLORREF g_color = RGB(40, 130, 210);

void DrawScene(HDC hdc, HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    HBRUSH bg = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &rc, bg);
    DeleteObject(bg);

    if (!g_draw)
    {
        const char* text =
            "Select Image -> Picture from the menu";
        TextOutA(hdc, 30, 30, text, lstrlenA(text));
        return;
    }

    HPEN pen = CreatePen(PS_SOLID, 3, RGB(25, 25, 25));
    HBRUSH brush = CreateSolidBrush(g_color);

    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, brush);

    // Main object: house
    POINT roof[3] = {
        {100, 150}, {200, 70}, {300, 150}
    };

    Polygon(hdc, roof, 3);
    Rectangle(hdc, 125, 150, 275, 290);
    Rectangle(hdc, 180, 215, 220, 290);
    Rectangle(hdc, 140, 170, 170, 200);
    Rectangle(hdc, 230, 170, 260, 200);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);

    DeleteObject(pen);
    DeleteObject(brush);

    if (g_extra)
    {
        // Additional object: tree
        HPEN treePen =
            CreatePen(PS_SOLID, 3, RGB(35, 90, 35));

        HBRUSH treeBrush =
            CreateSolidBrush(RGB(45, 160, 70));

        oldPen = SelectObject(hdc, treePen);
        oldBrush = SelectObject(hdc, treeBrush);

        Ellipse(hdc, 330, 105, 420, 195);
        Rectangle(hdc, 365, 190, 385, 285);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);

        DeleteObject(treePen);
        DeleteObject(treeBrush);
    }
}

LRESULT CALLBACK WndProc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        HMENU menuBar = CreateMenu();
        HMENU imageMenu = CreatePopupMenu();
        HMENU transformMenu = CreatePopupMenu();
        HMENU infoMenu = CreatePopupMenu();

        AppendMenuA(
            imageMenu, MF_STRING,
            ID_IMAGE_DRAW, "Rysunok");

        AppendMenuA(
            transformMenu, MF_STRING,
            ID_COLOR_CHANGE, "Zmina koloru");

        AppendMenuA(
            transformMenu, MF_STRING,
            ID_ADD_OBJECT, "Dodaty shche odyn obiekt");

        AppendMenuA(
            infoMenu, MF_STRING,
            ID_ABOUT, "Pro programu");

        AppendMenuA(
            infoMenu, MF_SEPARATOR, 0, nullptr);

        AppendMenuA(
            infoMenu, MF_STRING,
            ID_EXIT, "Vykhid");

        AppendMenuA(
            menuBar, MF_POPUP,
            (UINT_PTR)imageMenu, "Zobrazhennia");

        AppendMenuA(
            menuBar, MF_POPUP,
            (UINT_PTR)transformMenu, "Transformatsii");

        AppendMenuA(
            menuBar, MF_POPUP,
            (UINT_PTR)infoMenu, "Informatsiia");

        SetMenu(hwnd, menuBar);
        return 0;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_IMAGE_DRAW:
            g_draw = true;
            g_extra = false;
            InvalidateRect(hwnd, nullptr, TRUE);
            break;

        case ID_COLOR_CHANGE:
            g_color = RGB(220, 70, 70);
            g_draw = true;
            InvalidateRect(hwnd, nullptr, TRUE);
            break;

        case ID_ADD_OBJECT:
            g_draw = true;
            g_extra = true;
            InvalidateRect(hwnd, nullptr, TRUE);
            break;

        case ID_ABOUT:
            MessageBoxA(
                hwnd,
                "Laboratorna robota 3\n"
                "Variant 6\n"
                "Windows API, menu ta grafichni obiekty.",
                "Pro programu",
                MB_OK | MB_ICONINFORMATION);
            break;

        case ID_EXIT:
            DestroyWindow(hwnd);
            break;
        }
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        DrawScene(hdc, hwnd);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE,
    LPSTR, int nCmdShow)
{
    const char CLASS_NAME[] = "Variant6MenuWindow";

    WNDCLASSA wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassA(&wc))
        return 1;

    HWND hwnd = CreateWindowA(
        CLASS_NAME,
        "Laboratorna robota 3 - Menu, variant 6",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        620, 460,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd)
        return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg{};

    while (GetMessageA(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int)msg.wParam;
}
