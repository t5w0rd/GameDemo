#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

int main(int argc, char *argv[])
{
#define USE_WIN32_CONSOLE
#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

#endif
    
    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("GameDemo");
#if 0
    //eglView->setFrameSize(1024, 768);
    //eglView->setFrameZoomFactor(0.8f);
#else
    eglView->setFrameSize(2048, 1536);
    //eglView->setFrameSize(1920, 1080);
    eglView->setFrameZoomFactor(0.4f);
#endif
    return CCApplication::sharedApplication()->run();
}