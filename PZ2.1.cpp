// ============================================================
// Лабораторна робота №2. Завдання 1. Варіант 6
// Побудова графіка функції y = sqrt(x^2 + 5), -2 <= x <= 8
// ============================================================
#include <windows.h>
#include <math.h>
#include <cstdio>

BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance;
wchar_t szClassName[] = L"GraphClass";

typedef struct
{
    wchar_t name[30];
    float x[100];
    float y[100];
} FUNC;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg; HWND hwnd;
    hInstance = hInst;
    if (!RegClass(WndProc, szClassName, COLOR_WINDOW))
        return FALSE;

    hwnd = CreateWindow(szClassName, L"Графік функції y = sqrt(x^2 + 5)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0, hInstance, NULL);

    if (!hwnd) return FALSE;

    while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
    return (int)msg.wParam;
}

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground)
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.lpfnWndProc = Proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szName;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(UINT_PTR)(brBackground + 1);
    wc.lpszMenuName = NULL;
    return (RegisterClass(&wc) != 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static short cx, cy;
    static FUNC myfunc;

    switch (msg)
    {
    case WM_SIZE:
    {
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);
        return 0;
    }
    case WM_CREATE:
    {
        wcscpy_s(myfunc.name, 30, L"y = sqrt(x^2 + 5)");
        float xStart = -2.0f, xEnd = 8.0f;
        float step = (xEnd - xStart) / 99.0f;
        for (int i = 0; i < 100; i++)
        {
            myfunc.x[i] = xStart + step * i;
            myfunc.y[i] = sqrtf(myfunc.x[i] * myfunc.x[i] + 5.0f);
        }
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        int x0 = cx / 10;              // лівий край графіка
        int xr = cx - x0;              // правий край графіка
        int y0 = cy / 10;              // верхній відступ
        int yBottom = cy - y0;         // нижня межа графіка (вісь x)

        // Функція завжди додатна, тому шукаємо реальні min/max для масштабу
        float ymin = myfunc.y[0], ymax = myfunc.y[0];
        for (int i = 0; i < 100; i++)
        {
            if (myfunc.y[i] < ymin) ymin = myfunc.y[i];
            if (myfunc.y[i] > ymax) ymax = myfunc.y[i];
        }

        int plotHeight = yBottom - y0;
        float dy = (float)plotHeight / (ymax - ymin + 0.001f);
        float dx = (float)(xr - x0) / (myfunc.x[99] - myfunc.x[0]);

        // Назва функції
        TextOut(hdc, x0 + 4, y0 / 3, myfunc.name, (int)wcslen(myfunc.name));
        TextOut(hdc, xr + x0 / 2 - 20, yBottom + 5, L"x", 1);
        TextOut(hdc, x0 - 25, y0 - y0 / 2, L"y", 1);

        // Вісь ординат (y)
        MoveToEx(hdc, x0, yBottom, NULL);
        LineTo(hdc, x0, y0 - y0 / 2);

        // Вісь абсцис (x) - на рівні мінімуму функції
        MoveToEx(hdc, x0, yBottom, NULL);
        LineTo(hdc, xr + x0 / 2, yBottom);

        // Розмітка та підписи осі x
        wchar_t buf[32];
        for (int i = 0; i <= 5; i++)
        {
            float xv = myfunc.x[0] + i * (myfunc.x[99] - myfunc.x[0]) / 5.0f;
            int xcurr = (int)(dx * (xv - myfunc.x[0]) + x0);
            MoveToEx(hdc, xcurr, yBottom - 3, NULL);
            LineTo(hdc, xcurr, yBottom + 3);
            swprintf_s(buf, 32, L"%.1f", xv);
            TextOut(hdc, xcurr - 10, yBottom + 6, buf, (int)wcslen(buf));
        }

        // Розмітка та підписи осі y
        for (int i = 0; i <= 5; i++)
        {
            float yv = ymin + i * (ymax - ymin) / 5.0f;
            int ycurr = (int)(yBottom - dy * (yv - ymin));
            MoveToEx(hdc, x0 - 3, ycurr, NULL);
            LineTo(hdc, x0 + 3, ycurr);
            swprintf_s(buf, 32, L"%.2f", yv);
            TextOut(hdc, x0 - 45, ycurr - 8, buf, (int)wcslen(buf));
        }

        // Малювання самого графіка
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        MoveToEx(hdc, x0, (int)(yBottom - dy * (myfunc.y[0] - ymin)), NULL);
        for (int i = 1; i < 100; i++)
        {
            int xcurr = (int)(dx * (myfunc.x[i] - myfunc.x[0]) + x0);
            int ycurr = (int)(yBottom - dy * (myfunc.y[i] - ymin));
            LineTo(hdc, xcurr, ycurr);
        }

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        MessageBox(hwnd, L"Графік функції y = sqrt(x^2 + 5), -2 <= x <= 8",
            L"Інформація про графік", MB_OK | MB_ICONINFORMATION);
        return 0;
    }
    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)
            DestroyWindow(hwnd);
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}