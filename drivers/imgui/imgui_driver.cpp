#include <drivers/imgui/imgui_driver.h>
#include <core/error/error_macros.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace ballistic2d::drivers {
    
Error ImGuiDriver::create(const ImGuiDriverCreateInfo& p_info)
{
    using enum Error;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();

    BALLISTIC_ERR_FAIL_COND_V_MSG(!ImGui_ImplGlfw_InitForOpenGL(p_info.window, true), Failed,
        "Failed to initialize ImGui GLFW backend.");

    BALLISTIC_ERR_FAIL_COND_V_MSG(!ImGui_ImplOpenGL3_Init(p_info.glsl_version), Failed,
        "Failed to initialize ImGui OpenGL3 backend.");

    return Ok;
}

void ImGuiDriver::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiDriver::new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiDriver::render()
{
    ImGui::Render();
}

void ImGuiDriver::record_commands()
{
    ImDrawData* draw_data = ImGui::GetDrawData();
    if (draw_data) {
        ImGui_ImplOpenGL3_RenderDrawData(draw_data);
    }
}

}