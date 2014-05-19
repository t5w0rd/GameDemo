#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

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

        auto wsz = Size(1920, 1080);
        //auto wsz = Size(2048, 1536);
        //auto wsz = Size(800, 600);

        glview->setDesignResolutionSize(wsz.width, wsz.height, ResolutionPolicy::SHOW_ALL);
        glview->setFrameSize(wsz.width, wsz.height);
        glview->setFrameZoomFactor(0.4f);
    }

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
