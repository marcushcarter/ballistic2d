#pragma once
#include <core/error/error.h>
#include <windows.h>
#include <string>

struct GLFWwindow;

namespace ballistic2d::drivers {

struct WindowDriverGlfw
{
    GLFWwindow* window = nullptr;
    bool close_requested = false;
    uint32_t width = 0;
    uint32_t height = 0;

    Error create(const std::string& p_title, int p_width, int p_height);
    void destroy();

    void poll_events();
    bool should_close() const;
    void request_close();
    void swap_buffers() const;

    Error set_title(const std::string& p_title);

    Error set_icon(HICON p_icon);
    Error set_titlebar_color(COLORREF p_color);

    static void framebuffer_size_callback(GLFWwindow* p_window, int p_width, int p_height);
};

}