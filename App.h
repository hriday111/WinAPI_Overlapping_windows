#pragma once
#include <windows.h>
#include <vector>
#include <memory>

class Window;

class App {
public:
    static App* GetInstance();
    
    App(HINSTANCE hInst);
    ~App();

    bool Initialize();
    int Run(int nShow);

    void UpdateFollowers(int leaderX, int leaderY);

private:
    static App* s_instance;
    HINSTANCE m_hInst;
    std::vector<std::unique_ptr<Window>> m_windows;
};
