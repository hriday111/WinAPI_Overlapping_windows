#include "App.h"
#include "Constants.h"
#include "Window.h"
#include <string>

App* App::s_instance = nullptr;

App* App::GetInstance() {
    return s_instance;
}

App::App(HINSTANCE hInst) : m_hInst(hInst) {
    s_instance = this;
}

App::~App() {
    s_instance = nullptr;
}

bool App::Initialize() {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = Window::WndProc;
    wc.hInstance = m_hInst;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 
    wc.lpszClassName = L"ChainWnd";
    if (!RegisterClassExW(&wc)) return false;

    int baseW = Constants::WIN_W;
    int baseH = Constants::WIN_H;
    int shrinkAmount = 30;

    m_windows.resize(Constants::CHAIN_LEN);

    HWND lastHwnd = nullptr;
    for (int i = 0; i < Constants::CHAIN_LEN; ++i) {
        int curW = baseW - (i * shrinkAmount);
        int curH = baseH;
        if (curW < 100) curW = 100;
        if (curH < 60) curH = 60;

        int startX = 120 + (i * Constants::OFFSET_X) + (baseW - curW) / 2;
        int startY = 150 + (i * Constants::OFFSET_Y) + (baseH - curH) / 2;

        float alpha = Constants::ALPHA_BASE - i * Constants::ALPHA_STEP;
        if (alpha < 0.04f) alpha = 0.04f;

        // An owned window is ALWAYS above its owner.
        // If we want Follower 1 to be ABOVE Leader: Follower 1 (owned) -> Leader (owner).
        // If we want Follower 2 to be ABOVE Follower 1: Follower 2 (owned) -> Follower 1 (owner).
        // This puts Leader at the bottom and followers on top.
        auto win = std::make_unique<Window>(i, alpha, startX, startY, curW, curH, lastHwnd, m_hInst);
        if (!win->GetHwnd()) {
            return false;
        }
        lastHwnd = win->GetHwnd();
        m_windows[i] = std::move(win);
    }

    return true;
}

int App::Run(int nShow) {
    if (m_windows.empty()) return 0;

    for (size_t i = 1; i < m_windows.size(); ++i) {
        if (m_windows[i]) m_windows[i]->Show(SW_SHOWNOACTIVATE);
    }
    if (m_windows[0]) m_windows[0]->Show(nShow);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}

void App::UpdateFollowers(int leaderX, int leaderY) {
    int baseW = Constants::WIN_W;
    int baseH = Constants::WIN_H;
    int shrinkAmount = 30;

    for (size_t i = 1; i < m_windows.size(); ++i) {
        if (m_windows[i]) {
            int curW = baseW - ((int)i * shrinkAmount);
            int curH = baseH - ((int)i * shrinkAmount);
            if (curW < 100) curW = 100;
            if (curH < 60) curH = 60;

            m_windows[i]->SetTarget(
                leaderX + ((int)i * Constants::OFFSET_X) + (baseW - curW) / 2,
                leaderY + ((int)i * Constants::OFFSET_Y) + (baseH - curH) / 2
            );
        }
    }
}
