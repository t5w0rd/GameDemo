#include "CommHeader.h"
#include "main.h"
#include "AppDelegate.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#define USE_WIN32_CONSOLE
#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview)
    {
        glview = GLView::create("GameDemo");
        director->setOpenGLView(glview);

        const auto wsz = Size(1794, 1080);
        //const auto wsz = Size(2048, 1536);
        //const auto wsz = Size(800, 480);

        const auto ssz = Size(1200, 700);
        glview->setFrameSize(wsz.width, wsz.height);
        float zoom = min(ssz.width / wsz.height, ssz.height / wsz.height);
        glview->setFrameZoomFactor(zoom);
    }
    
    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
