#include <core/application/application.h>
#include <core/error/error_macros.h>
#include <core/version.h>
#include <windows.h>
#include <shlobj.h>
#include <chrono>
#include <iostream>
#include <filesystem>

namespace ballistic2d {

static std::string get_user_data_ini_path()
{
    PWSTR appdata_path_w = nullptr;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &appdata_path_w);
    if (FAILED(result) || !appdata_path_w) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, appdata_path_w, -1, nullptr, 0, nullptr, nullptr);

    std::string appdata_path(size - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, appdata_path_w, -1, appdata_path.data(), size, nullptr, nullptr);
    CoTaskMemFree(appdata_path_w);

    std::string dir = appdata_path + "\\BallisticGames\\Ballistic2D";
    std::filesystem::create_directories(dir);
    return dir + "\\imgui.ini";
}

Error Application::create(const ApplicationCreateInfo& p_info)
{
    using enum Error;
    Error err;

    std::cout << BALLISTIC_VERSION_NAME << " v" << BALLISTIC_VERSION_NUMBER << ".stable.official - https://ballisticgames.ca\n";
    
    create_info = p_info;

    err = window.create(p_info.window_title, p_info.width, p_info.height);
    BALLISTIC_ERR_FAIL_COND_V(err != Ok, err);

    err = device_driver.initialize();
    BALLISTIC_ERR_FAIL_COND_V(err != Ok, err);

    renderer.device_driver = &device_driver;
    err = renderer.create(3);
    BALLISTIC_ERR_FAIL_COND_V(err != Ok, err);

    drivers::ImGuiDriverCreateInfo imgui_ci{};
    imgui_ci.window = window.window;
    imgui_ci.glsl_version = "#version 460";
    imgui_ci.ini_path = get_user_data_ini_path();

    err = imgui.create(imgui_ci);
    BALLISTIC_ERR_FAIL_COND_V(err != Ok, err);

    return Ok;
}

void Application::destroy()
{
    imgui.destroy();
    renderer.destroy();
    device_driver.shutdown();
    window.destroy();
}

int Application::run()
{
    using enum Error;
    Error err;

    err = on_init();
    BALLISTIC_ERR_FAIL_COND_V(err != Ok, static_cast<int>(err));

    auto lastTime = std::chrono::steady_clock::now();

    while (!window.should_close()) {
        auto now = std::chrono::steady_clock::now();
        double delta = std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        window.poll_events();

        err = renderer.set_size(window.width, window.height);
        BALLISTIC_ERR_FAIL_COND_V(err != Ok, static_cast<int>(err));

        imgui.new_frame();
        on_update((float)delta);
        imgui.render();

        renderer.begin_frame();
        imgui.record_commands();
        renderer.end_frame();

        window.swap_buffers();
    }

    on_shutdown();
    destroy();
    return 0;
}

void Application::_load_project(const std::string& p_project_folder)
{
    std::filesystem::path folder = p_project_folder;
    std::filesystem::path marker = folder / "ballistic2d.project";

    if (!std::filesystem::is_directory(folder)) return; 
    if (!std::filesystem::is_regular_file(marker)) return;

    project_path = folder.string();
}

}