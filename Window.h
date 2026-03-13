#pragma once
#include <windows.h>
#include <string>

class Window {
public:
    Window(int index, float alpha, int x, int y, int w, int h, HWND parentHwnd, HINSTANCE hInst);
    ~Window();

    HWND GetHwnd() const { return m_hwnd; }
    void SetTarget(int x, int y);
    void Show(int nShow);
    void Update();

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

private:
    LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    void OnMove(HWND hwnd);
    void OnTimer(HWND hwnd);

    HWND m_hwnd;
    int m_index;
    float m_curX, m_curY;
    float m_tgtX, m_tgtY;
    float m_alpha;
};
