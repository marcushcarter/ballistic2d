#include <core/application/game_application.h>
#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    ballistic2d::ApplicationCreateInfo info;
    info.window_title = "Ballistic2D Game";
    info.width = 1280;
    info.height = 720;

    ballistic2d::GameApplication app;
    app.create(info);
    return app.run();
}