// Лабораторна робота №2. Завдання 2. Варіант 6
// Композиція: ялинка, будинок, сніговик, хмари
#include <windows.h>
#include <cstdio>

#pragma comment(linker, "/subsystem:windows")

// Клас "Хмара"
class Cloud
{
public:
    void show(HDC dc, int X, int Y)
    {
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        HGDIOBJ oldBrush = SelectObject(dc, brush);
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(220, 220, 220));
        HGDIOBJ oldPen = SelectObject(dc, pen);

        Ellipse(dc, X, Y, X + 60, Y + 30);
        Ellipse(dc, X + 25, Y - 15, X + 85, Y + 25);
        Ellipse(dc, X + 55, Y, X + 115, Y + 30);
        Ellipse(dc, X + 15, Y + 5, X + 100, Y + 35);

        SelectObject(dc, oldBrush);
        SelectObject(dc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }
};

// Клас "Ялинка"
class ChristmasTree
{
public:
    void show(HDC dc, int X, int Y)
    {
        // Стовбур
        HBRUSH trunkBrush = CreateSolidBrush(RGB(102, 51, 0));
        HGDIOBJ oldBrush = SelectObject(dc, trunkBrush);
        Rectangle(dc, X - 8, Y, X + 8, Y + 30);
        SelectObject(dc, oldBrush);
        DeleteObject(trunkBrush);

        // Три яруси хвої
        HBRUSH greenBrush = CreateSolidBrush(RGB(0, 128, 0));
        oldBrush = SelectObject(dc, greenBrush);

        POINT tier1[3] = { { X, Y - 130 }, { X - 40, Y - 70 }, { X + 40, Y - 70 } };
        Polygon(dc, tier1, 3);

        POINT tier2[3] = { { X, Y - 100 }, { X - 55, Y - 30 }, { X + 55, Y - 30 } };
        Polygon(dc, tier2, 3);

        POINT tier3[3] = { { X, Y - 60 }, { X - 70, Y }, { X + 70, Y } };
        Polygon(dc, tier3, 3);

        SelectObject(dc, oldBrush);
        DeleteObject(greenBrush);

        // Прикраси (кульки)
        HBRUSH ballBrush = CreateSolidBrush(RGB(255, 0, 0));
        oldBrush = SelectObject(dc, ballBrush);
        Ellipse(dc, X - 6, Y - 100, X + 6, Y - 88);
        Ellipse(dc, X - 30, Y - 50, X - 18, Y - 38);
        Ellipse(dc, X + 20, Y - 20, X + 32, Y - 8);
        SelectObject(dc, oldBrush);
        DeleteObject(ballBrush);

        // Зірка на верхівці
        HBRUSH starBrush = CreateSolidBrush(RGB(255, 215, 0));
        oldBrush = SelectObject(dc, starBrush);
        Ellipse(dc, X - 6, Y - 145, X + 6, Y - 133);
        SelectObject(dc, oldBrush);
        DeleteObject(starBrush);
    }
};


// Клас "Будинок"
class House
{
public:
    void show(HDC dc, int X, int Y)
    {
        HBRUSH brush;
        HGDIOBJ oldBrush;

        // Стіни
        brush = CreateSolidBrush(RGB(255, 228, 196));
        oldBrush = SelectObject(dc, brush);
        Rectangle(dc, X, Y, X + 120, Y + 90);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);

        // Дах
        POINT roof[3] = { { X - 15, Y }, { X + 60, Y - 60 }, { X + 135, Y } };
        brush = CreateSolidBrush(RGB(139, 0, 0));
        oldBrush = SelectObject(dc, brush);
        Polygon(dc, roof, 3);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);

        // Сніг на даху
        brush = CreateSolidBrush(RGB(255, 255, 255));
        oldBrush = SelectObject(dc, brush);
        POINT snow[3] = { { X - 5, Y - 5 }, { X + 60, Y - 55 }, { X + 125, Y - 5 } };
        Polygon(dc, snow, 3);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);

        // Двері
        brush = CreateSolidBrush(RGB(101, 67, 33));
        oldBrush = SelectObject(dc, brush);
        Rectangle(dc, X + 50, Y + 45, X + 75, Y + 90);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);

        // Вікна
        brush = CreateSolidBrush(RGB(135, 206, 250));
        oldBrush = SelectObject(dc, brush);
        Rectangle(dc, X + 15, Y + 20, X + 40, Y + 45);
        Rectangle(dc, X + 90, Y + 20, X + 115, Y + 45);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};

// Клас "Сніговик"
class Snowman
{
public:
    void show(HDC dc, int X, int Y)
    {
        HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
        HGDIOBJ oldBrush = SelectObject(dc, whiteBrush);

        // Три кулі тіла
        Ellipse(dc, X - 40, Y - 40, X + 40, Y + 40);
        Ellipse(dc, X - 28, Y - 95, X + 28, Y - 39);
        Ellipse(dc, X - 18, Y - 135, X + 18, Y - 99);

        SelectObject(dc, oldBrush);
        DeleteObject(whiteBrush);

        // Капелюх
        HBRUSH hatBrush = CreateSolidBrush(RGB(0, 0, 0));
        oldBrush = SelectObject(dc, hatBrush);
        Rectangle(dc, X - 20, Y - 150, X + 20, Y - 133);
        Rectangle(dc, X - 12, Y - 175, X + 12, Y - 150);
        SelectObject(dc, oldBrush);
        DeleteObject(hatBrush);

        // Очі та ґудзики
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        oldBrush = SelectObject(dc, blackBrush);
        Ellipse(dc, X - 8, Y - 122, X - 3, Y - 117);
        Ellipse(dc, X + 3, Y - 122, X + 8, Y - 117);
        Ellipse(dc, X - 4, Y - 15, X + 4, Y - 7);
        Ellipse(dc, X - 4, Y + 5, X + 4, Y + 13);
        SelectObject(dc, oldBrush);
        DeleteObject(blackBrush);

        // Ніс-морквина
        HBRUSH orangeBrush = CreateSolidBrush(RGB(255, 140, 0));
        oldBrush = SelectObject(dc, orangeBrush);
        POINT nose[3] = { { X, Y - 112 }, { X + 18, Y - 108 }, { X, Y - 104 } };
        Polygon(dc, nose, 3);
        SelectObject(dc, oldBrush);
        DeleteObject(orangeBrush);

        // Руки-гілки
        HPEN pen = CreatePen(PS_SOLID, 3, RGB(102, 51, 0));
        HGDIOBJ oldPen = SelectObject(dc, pen);
        MoveToEx(dc, X - 28, Y - 60, NULL);
        LineTo(dc, X - 60, Y - 85);
        MoveToEx(dc, X + 28, Y - 60, NULL);
        LineTo(dc, X + 60, Y - 85);
        SelectObject(dc, oldPen);
        DeleteObject(pen);
    }
};

// Малювання всієї сцени
void DrawScene(HDC dc, int width, int height)
{
    HBRUSH brush;
    HGDIOBJ oldBrush;

    // Небо
    brush = CreateSolidBrush(RGB(176, 224, 230));
    oldBrush = SelectObject(dc, brush);
    Rectangle(dc, 0, 0, width, height * 2 / 3);
    SelectObject(dc, oldBrush);
    DeleteObject(brush);

    // Земля, вкрита снігом
    brush = CreateSolidBrush(RGB(255, 250, 250));
    oldBrush = SelectObject(dc, brush);
    Rectangle(dc, 0, height * 2 / 3, width, height);
    SelectObject(dc, oldBrush);
    DeleteObject(brush);

    // Хмари (об'єкт повторюється 3 рази)
    Cloud cloud;
    cloud.show(dc, 60, 40);
    cloud.show(dc, 320, 70);
    cloud.show(dc, width - 200, 30);

    // Будинки (об'єкт повторюється 2 рази)
    House house;
    house.show(dc, 60, height * 2 / 3 - 90);
    house.show(dc, width - 220, height * 2 / 3 - 90);

    // Ялинки (об'єкт повторюється 3 рази)
    ChristmasTree tree;
    tree.show(dc, 250, height - 80);
    tree.show(dc, 450, height - 60);
    tree.show(dc, width - 100, height - 90);

    // Сніговик
    Snowman snowman;
    snowman.show(dc, width / 2, height - 60);
}

// Процедура обробки повідомлень
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        int width = rect.right;
        int height = rect.bottom;

        DrawScene(dc, width, height);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        wchar_t buf[64];
        swprintf_s(buf, 64, L"Клацання лівою кнопкою миші: (%d, %d)", x, y);
        MessageBox(hwnd, buf, L"WM_LBUTTONDOWN", MB_OK | MB_ICONINFORMATION);
        return 0;
    }

    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hwnd);
        }
        return 0;
    }

    case WM_SIZE:
    {
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

// Головна функція Windows-програми
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"WinterSceneApplication";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(UINT_PTR)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME,
        L"Варіант 6 — Ялинка, будинок, сніговик, хмари",
        WS_OVERLAPPEDWINDOW,
        100, 100, 900, 600, nullptr, nullptr, hInstance, nullptr);

    if (hwnd == nullptr)
        return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG message = {};
    while (GetMessage(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return 0;
}