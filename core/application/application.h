#pragma once
#include <drivers/glfw/window_driver_glfw.h>
#include <drivers/opengl/rendering_device_driver_opengl.h>
#include <drivers/imgui/imgui_driver.h>
#include <core/rendering/renderer.h>
#include <core/error/error.h>
#include <string>

namespace ballistic2d {

struct ApplicationCreateInfo
{
    std::string window_title;
    int width = 1280;
    int height = 720;
};

struct Application
{
    ApplicationCreateInfo create_info;

    drivers::WindowDriverGlfw window;
    drivers::RenderingDeviceDriverOpengl device_driver;
    drivers::ImGuiDriver imgui;

    Renderer renderer;

    std::string project_path;

    Error create(const ApplicationCreateInfo& p_info);
    void destroy();

    int run();
    
    void _load_project(const std::string& p_project_folder);

    virtual Error on_init() { return Error::Ok; }
    virtual void on_update(float p_dt) { (void)p_dt; }
    virtual void on_shutdown() {}

    virtual ~Application() = default;
};

}