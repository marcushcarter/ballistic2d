#pragma once
#include <core/error/error.h>
#include <string>

struct GLFWwindow;

namespace ballistic2d::drivers {

struct ImGuiDriverCreateInfo
{
    GLFWwindow* window = nullptr;
    const char* glsl_version = "#version 460";
    std::string ini_path;
};

struct ImGuiDriver
{
    std::string ini_path;

    Error create(const ImGuiDriverCreateInfo& p_info);
    void destroy();

    void new_frame();
    void render();
    void record_commands();
};

}