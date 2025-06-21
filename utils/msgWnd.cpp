#ifdef _WIN32
#include <windows.h>
#include <commctrl.h>
#include <string>
#pragma comment(lib, "comctl32.lib")
#endif

namespace QuavleEngine {

#ifdef _WIN32
    static HWND hProgress = nullptr;

    LRESULT CALLBACK ProgressWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return 0;
    }
#endif

    void windowMessageWithProgress(const std::string& title, const std::string& message, float progress) {
#ifdef _WIN32
        INITCOMMONCONTROLSEX icex;
        icex.dwSize = sizeof(icex);
        icex.dwICC = ICC_PROGRESS_CLASS;
        InitCommonControlsEx(&icex);

        WNDCLASSW wc = {};
        wc.lpfnWndProc = ProgressWndProc;
        wc.hInstance = GetModuleHandleW(nullptr);
        wc.lpszClassName = L"QuavleProgressBar";
        RegisterClassW(&wc);

        std::wstring wtitle(title.begin(), title.end());
        std::wstring wmsg(message.begin(), message.end());

        HWND hwnd = CreateWindowExW(
            0, L"QuavleProgressBar", wtitle.c_str(),
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 130,
            nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);

        CreateWindowExW(
            0, L"STATIC", wmsg.c_str(),
            WS_VISIBLE | WS_CHILD | SS_CENTER,
            20, 10, 340, 20, hwnd, nullptr, GetModuleHandleW(nullptr), nullptr);

        hProgress = CreateWindowExW(
            0, PROGRESS_CLASSW, nullptr,
            WS_VISIBLE | WS_CHILD,
            20, 40, 340, 30, hwnd, nullptr, GetModuleHandleW(nullptr), nullptr);

        SendMessageW(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessageW(hProgress, PBM_SETPOS, static_cast<WPARAM>(progress * 100.0f), 0);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        // Message loop (non-blocking, closes window after short delay)
        MSG msg;
        DWORD start = GetTickCount();
        while (IsWindow(hwnd) && GetTickCount() - start < 500) {
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            Sleep(10);
        }
        if (IsWindow(hwnd)) DestroyWindow(hwnd);
#else
        printf("%s: [%d%%] %s\n", title.c_str(), int(progress * 100), message.c_str());
#endif
    }
}
