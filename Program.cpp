#include <Windows.h>
#include <CommCtrl.h>

// Визуальные стили
#pragma comment(linker,"\"/manifestdependency:type='win32' \
                        name='Microsoft.Windows.Common-Controls' \
                        version='6.0.0.0' processorArchitecture='*' \
                        publicKeyToken='6595b64144ccf1df' language='*'\"")

// Обработка сообщений
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND window;  // Окно
HWND progressBar;  // Полоза загрузки
HWND button;  // Кнопка "Отмена"
int value;  // Значение прогресса

int windowId = 0;
int progressBarId = 1;
int buttonId = 2;
int timerId = 1001;

// Главная функция
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
    _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	const wchar_t CLASS_NAME[] = L"Lab1WindowClass";
    value = 0.0f;
	WNDCLASS wc = { };
    RECT rcScreen;
    GetWindowRect(GetDesktopWindow(), &rcScreen);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);
  
    window = CreateWindowEx(0, CLASS_NAME, L"lab1", WS_OVERLAPPED |
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, rcScreen.right / 2 - 200, 
        rcScreen.bottom / 2 - 60, 400, 90, NULL, (HMENU)windowId, hInstance, NULL);
    progressBar = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL, WS_CHILD |
        WS_VISIBLE, 12, 12, 275, 26, window, (HMENU)progressBarId, hInstance, NULL);
    button = CreateWindowEx(0, L"BUTTON", L"Закрыть", WS_TABSTOP | WS_VISIBLE |
        WS_CHILD | BS_DEFPUSHBUTTON, 295, 11, 78, 28, window, (HMENU)buttonId,
        (HINSTANCE)GetWindowLongPtr(window, GWLP_HINSTANCE), NULL);

    SendMessage(progressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    SendMessage(progressBar, PBM_SETSTEP, (WPARAM)1, 0);

    if (!window || !progressBar || !button)
        return 1;

	ShowWindow(window, nShowCmd);
    SetTimer(window, timerId, 100, (TIMERPROC)NULL);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	return 0;
}

// Обработка сообщений
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:  // При закрытии окна
        PostQuitMessage(0);
        return 0;
    case WM_TIMER:  // При тике таймера
        if (value < 100)
        {
            SendMessage(progressBar, PBM_STEPIT, 0, 0);
            value++;
        }
        else
        {
            KillTimer(window, timerId);
                      
        }
        return 0;
    case WM_COMMAND:  // При нажатии кнопки
        KillTimer(window, timerId);
        DestroyWindow(window);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}