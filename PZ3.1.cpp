
#include <windows.h>
#include <string>
#include <stdexcept>

#define ID_EDIT_X 101
#define ID_EDIT_Y 102
#define ID_BUTTON_CALC 103
#define ID_RESULT 104

static HWND hEditX, hEditY, hResult;

LRESULT CALLBACK WndProc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        HINSTANCE hInst =
            ((LPCREATESTRUCT)lParam)->hInstance;

        CreateWindowA("STATIC", "Vvedit tsili chysla x ta y:",
            WS_CHILD | WS_VISIBLE,
            25, 20, 260, 22,
            hwnd, nullptr, hInst, nullptr);

        hEditX = CreateWindowA("EDIT", "12",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
            25, 50, 100, 26,
            hwnd, (HMENU)ID_EDIT_X, hInst, nullptr);

        hEditY = CreateWindowA("EDIT", "3",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
            145, 50, 100, 26,
            hwnd, (HMENU)ID_EDIT_Y, hInst, nullptr);

        CreateWindowA("BUTTON", "Obchyslyty",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            25, 95, 120, 30,
            hwnd, (HMENU)ID_BUTTON_CALC, hInst, nullptr);

        hResult = CreateWindowA("STATIC", "Resultat:",
            WS_CHILD | WS_VISIBLE,
            25, 145, 300, 25,
            hwnd, (HMENU)ID_RESULT, hInst, nullptr);

        return 0;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON_CALC)
        {
            char bx[64]{}, by[64]{};

            GetWindowTextA(hEditX, bx, sizeof(bx));
            GetWindowTextA(hEditY, by, sizeof(by));

            try
            {
                size_t px = 0, py = 0;

                long long x = std::stoll(bx, &px);
                long long y = std::stoll(by, &py);

                if (bx[px] != '\0' || by[py] != '\0')
                    throw std::invalid_argument("invalid");

                if (y == 0)
                {
                    SetWindowTextA(
                        hResult,
                        "Pomylka: y ne mozhe buty 0");
                }
                else
                {
                    int result = (x % y == 0) ? 1 : 0;

                    std::string text =
                        "Resultat: " + std::to_string(result);

                    SetWindowTextA(hResult, text.c_str());
                }
            }
            catch (...)
            {
                SetWindowTextA(
                    hResult,
                    "Pomylka: vvedit tsili chysla");
            }
        }
        return 0;

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
    const char CLASS_NAME[] = "Variant6FunctionWindow";

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
        "Laboratorna robota 3 - Variant 6",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        390, 250,
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
