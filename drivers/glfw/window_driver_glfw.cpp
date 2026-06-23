#include <drivers/glfw/window_driver_glfw.h>
#include <core/error/error_macros.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>

namespace ballistic2d::drivers {    

Error WindowDriverGlfw::create(const std::string& p_title, int p_width, int p_height)
{
    using enum Error;
    
    BALLISTIC_ERR_FAIL_COND_V(!glfwInit(), Failed);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(p_width, p_height, p_title.c_str(), nullptr, nullptr);
    BALLISTIC_ERR_FAIL_COND_V(!window, Failed);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    bool err = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    BALLISTIC_ERR_FAIL_COND_V(!err, Failed);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return Ok;
}

void WindowDriverGlfw::destroy()
{
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
    }
}

void WindowDriverGlfw::poll_events()
{
    glfwPollEvents();
}

bool WindowDriverGlfw::should_close() const
{
    return close_requested || glfwWindowShouldClose(window);
}

void WindowDriverGlfw::request_close()
{
    close_requested = true;
}

void WindowDriverGlfw::swap_buffers() const
{
    glfwSwapBuffers(window);
}

Error WindowDriverGlfw::set_title(const std::string& p_title)
{
    using enum Error;
    BALLISTIC_ERR_FAIL_COND_V(!window, Failed);
    glfwSetWindowTitle(window, p_title.c_str());
    return Ok;
}

Error WindowDriverGlfw::set_icon(HICON p_icon)
{
    using enum Error;
    BALLISTIC_ERR_FAIL_COND_V(!p_icon, Failed);
    HWND hwnd = glfwGetWin32Window(window);
    SendMessageW(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(p_icon));
    SendMessageW(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(p_icon));
    return Ok;
}

Error WindowDriverGlfw::set_titlebar_color(COLORREF p_color)
{    
    using enum Error;
    HWND hwnd = glfwGetWin32Window(window);
    HRESULT result = DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &p_color, sizeof(p_color));
    BALLISTIC_ERR_FAIL_COND_V_MSG(FAILED(result), Failed, "DwmSetWindowAttribute failed — DWMWA_CAPTION_COLOR requires Windows 11 (build 22000+).");
    return Ok;
}

void WindowDriverGlfw::framebuffer_size_callback(GLFWwindow* p_window, int p_width, int p_height)
{
    auto* self = static_cast<WindowDriverGlfw*>(glfwGetWindowUserPointer(p_window));
    self->width = p_width;
    self->height = p_height;
}

}