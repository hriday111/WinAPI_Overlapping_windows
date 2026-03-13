#include "Window.h"
#include "Constants.h"
#include "App.h"
#include <cmath>
#include <vector>

Window::Window(int index, float alpha, int x, int y, int w, int h, HWND parentHwnd, HINSTANCE hInst)
    : m_index(index), m_alpha(alpha), m_curX((float)x), m_curY((float)y),
      m_tgtX((float)x), m_tgtY((float)y), m_hwnd(nullptr)
{
    
    DWORD style = WS_POPUP | WS_CAPTION | WS_SYSMENU;
    if (index == 0) style |= WS_THICKFRAME;

    std::wstring title = (index == 0) ? L"Leader" : std::wstring(L"Follower ") + std::to_wstring(index);

    m_hwnd = CreateWindowExW(
        0, L"ChainWnd", title.c_str(), style,
        x, y, w, h,
        parentHwnd, nullptr, hInst, this);

    if (m_hwnd) {
        SetWindowLongPtrW(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        if (index != 0) {
            SetTimer(m_hwnd, Constants::TIMER_LERP, Constants::TICK_MS, nullptr);
        }
    }
}

Window::~Window() {
    if (m_hwnd) {
        KillTimer(m_hwnd, Constants::TIMER_LERP);
    }
}

void Window::SetTarget(int x, int y) {
    m_tgtX = (float)x;
    m_tgtY = (float)y;
}

void Window::Show(int nShow) {
    ShowWindow(m_hwnd, nShow);
    UpdateWindow(m_hwnd);
}

void Window::Update() {
    UpdateWindow(m_hwnd);
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    Window* self = nullptr;
    if (msg == WM_NCCREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lp);
        self = reinterpret_cast<Window*>(cs->lpCreateParams);
        self->m_hwnd = hwnd; 
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    } else {
        self = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    if (self) {
        return self->HandleMessage(hwnd, msg, wp, lp);
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

LRESULT Window::HandleMessage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_MOVE:
        OnMove(hwnd);
        break;
    case WM_TIMER:
        OnTimer(hwnd);
        return 0;
    case WM_DESTROY:
        if (m_index == 0) PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

void Window::OnMove(HWND hwnd) {
    if (m_index == 0) {
        RECT wr; GetWindowRect(hwnd, &wr);
        App::GetInstance()->UpdateFollowers(wr.left, wr.top);
    }
}

void Window::OnTimer(HWND hwnd) {
    if (m_index == 0) return;

    float dx = m_tgtX - m_curX;
    float dy = m_tgtY - m_curY;

    if (fabsf(dx) >= 0.5f || fabsf(dy) >= 0.5f) {
        m_curX += dx * m_alpha;
        m_curY += dy * m_alpha;

        int nx = (int)roundf(m_curX);
        int ny = (int)roundf(m_curY);

        RECT wr; GetWindowRect(hwnd, &wr);
        if (nx != wr.left || ny != wr.top) {
            SetWindowPos(hwnd, nullptr, nx, ny, 0, 0,
                SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
}
