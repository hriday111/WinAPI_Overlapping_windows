#include "App.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nShowCmd)
{
    App app(hInstance);
    if (!app.Initialize()) {
        return -1;
    }
    return app.Run(nShowCmd);
}
