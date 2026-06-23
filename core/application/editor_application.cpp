#include <core/application/editor_application.h>
#include <core/io/embedded_resource.h>
#include <core/error/error_macros.h>
#include <imgui.h>
#include "IconsFontAwesome6.h"
#include <shobjidl.h>
#include <filesystem>
#include <fstream>
#include <windows.h>

namespace ballistic2d {

Error EditorApplication::on_init()
{
    using enum Error;
    Error err;

    err = window.set_icon(EmbeddedResource::load_icon(L"BALLISTIC_ICON"));
    BALLISTIC_ERR_FAIL_COND_V(err != Ok, err);

    err = window.set_titlebar_color(RGB(20, 20, 25));
    BALLISTIC_ERR_FAIL_COND_V(err != Ok, err);
    
    ImGuiIO& io = ImGui::GetIO();
    {
        std::vector<uint8_t> jbData = EmbeddedResource::load_font(L"EMBEDDED_ASSETS_EDITOR_FONTS_JETBRAINS_MONO_REGULAR_TTF");
        if (!jbData.empty()) {
            void* jbCopy = IM_ALLOC(jbData.size());
            memcpy(jbCopy, jbData.data(), jbData.size());
            ImFontConfig jbCfg;
            jbCfg.FontDataOwnedByAtlas = true;
            io.Fonts->AddFontFromMemoryTTF(jbCopy, (int)jbData.size(), 14.0f, &jbCfg);
        }

        std::vector<uint8_t> faData = EmbeddedResource::load_font(L"EMBEDDED_ASSETS_EDITOR_FONTS_FA_SOLID_900_OTF");
        if (!faData.empty()) {
            void* faCopy = IM_ALLOC(faData.size());
            memcpy(faCopy, faData.data(), faData.size());
            static const ImWchar faRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
            ImFontConfig faCfg;
            faCfg.MergeMode = true;
            faCfg.PixelSnapH = true;
            faCfg.FontDataOwnedByAtlas = true;
            io.Fonts->AddFontFromMemoryTTF(faCopy, (int)faData.size(), 14.0f, &faCfg, faRanges);
        }

        io.Fonts->Build();
    }
    
    return Ok;
}

void EditorApplication::on_update(float p_dt)
{
    (void)p_dt;

    if (project_path.empty()) {
        _update_launcher();
    } else {
        _update_editor();
    }
}

void EditorApplication::on_shutdown()
{
    
}

static bool open_folder_dialog(std::string& out_path)
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    bool com_inited = SUCCEEDED(hr);

    IFileDialog* pFileDialog = nullptr;

    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
                          IID_PPV_ARGS(&pFileDialog));

    if (FAILED(hr)) {
        if (com_inited) CoUninitialize();
        return false;
    }

    DWORD options;
    pFileDialog->GetOptions(&options);
    pFileDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

    hr = pFileDialog->Show(nullptr);
    if (FAILED(hr)) {
        pFileDialog->Release();
        if (com_inited) CoUninitialize();
        return false;
    }

    IShellItem* item = nullptr;
    hr = pFileDialog->GetResult(&item);

    if (SUCCEEDED(hr)) {
        PWSTR wide_path = nullptr;
        hr = item->GetDisplayName(SIGDN_FILESYSPATH, &wide_path);

        if (SUCCEEDED(hr)) {
            int size = WideCharToMultiByte(CP_UTF8, 0, wide_path, -1, nullptr, 0, nullptr, nullptr);
            out_path.resize(size - 1);
            WideCharToMultiByte(CP_UTF8, 0, wide_path, -1, out_path.data(), size, nullptr, nullptr);

            CoTaskMemFree(wide_path);
        }

        item->Release();
    }

    pFileDialog->Release();
    if (com_inited) CoUninitialize();

    return SUCCEEDED(hr);
}

static void set_hidden(const std::string& path)
{
    DWORD attrs = GetFileAttributesA(path.c_str());
    if (attrs != INVALID_FILE_ATTRIBUTES) SetFileAttributesA(path.c_str(), attrs | FILE_ATTRIBUTE_HIDDEN);
}

void EditorApplication::_create_new_project(std::string p_project_folder, bool p_version_control)
{
    std::filesystem::path folder = p_project_folder;
    std::filesystem::create_directories(folder);

    std::ofstream marker(folder / "ballistic2d.project");
    marker << "ballistic_project=1\n";

    std::filesystem::path ballistic_dir = folder / ".ballistic";
    std::filesystem::create_directories(ballistic_dir);
    set_hidden(ballistic_dir.string());
    
    std::filesystem::create_directories(folder / "assets");
    std::filesystem::create_directories(folder / ".scenes");

    if (p_version_control) {
        std::filesystem::path gitignore_path = folder / ".gitignore";
        std::ofstream gitignore(gitignore_path, std::ios::trunc);
        if (gitignore.is_open()) {
            gitignore << ".ballistic/\n";
            gitignore << "*.exe\n";
        }
    }
}

void EditorApplication::_update_launcher()
{
    window.set_title("");

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);
    ImGui::SetNextWindowViewport(main_viewport->ID);

    ImGuiWindowFlags launcher_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
        | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::Begin("LauncherHost", nullptr, launcher_flags);

    const ImVec2 button_size(160.0f, 40.0f);
    const float spacing = 8.0f;
    const ImVec2 region = ImGui::GetContentRegionAvail();

    float block_height = button_size.y * 2.0f + spacing;
    float start_y = (region.y - block_height) * 0.5f;
    float start_x = (region.x - button_size.x) * 0.5f;

    const char* new_project_string = ICON_FA_ROCKET " New Project";
    const char* open_project_string = ICON_FA_SHARE " Open Project";

    ImGui::SetCursorPos(ImVec2(start_x, start_y));
    if (ImGui::Button(new_project_string, button_size)) {
        new_project_popup_open = true;
        ImGui::OpenPopup(new_project_string);
    }

    ImGui::SetCursorPos(ImVec2(start_x, start_y + button_size.y + spacing));
    if (ImGui::Button(open_project_string, button_size)) {
        std::string path;
        if (open_folder_dialog(path)) {
            _load_project(path);
        }
    }

    ImGui::SetNextWindowSize(ImVec2(400.0f, 0.0f));
    if (ImGui::BeginPopupModal(new_project_string, &new_project_popup_open, ImGuiWindowFlags_NoResize)) {

        ImGui::Text("Project Name");
        ImGui::SetNextItemWidth(-1.0f);
        ImGui::InputTextWithHint("##ProjectName", "Project name", new_project_name, sizeof(new_project_name));

        ImGui::Spacing();

        ImGui::Text("Location");
        ImGui::SetNextItemWidth(-1.0f);
        float btn_size = ImGui::GetFrameHeight();
        ImGui::PushID("path_row");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - btn_size - ImGui::GetStyle().ItemSpacing.x);
        ImGui::InputText("##ProjectLocation", new_project_location, sizeof(new_project_location));
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_FOLDER_OPEN, ImVec2(btn_size, btn_size))) {
            std::string selected;
            if (open_folder_dialog(selected)) strncpy_s(new_project_location, selected.c_str(), _TRUNCATE);
        }
        ImGui::PopID();

        ImGui::Checkbox("Version Control", &new_project_use_git);

        bool name_empty = (new_project_name[0] == '\0');
        bool location_empty = new_project_location[0] == '\0';
        std::string base = new_project_location;
        std::string project_folder = base + "\\" + new_project_name;
        bool base_exists = std::filesystem::exists(base);
        bool project_exists = std::filesystem::exists(project_folder);
        bool invalid = name_empty || location_empty || !base_exists || project_exists;

        if (location_empty || !base_exists) {
            ImGui::TextColored(ImVec4(0.9f, 0.3f, 0.3f, 1.0f), "Invalid location: folder does not exist.");
        } else if (name_empty) {
            ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "Project name cannot be empty.");
        } else if (project_exists) {
            ImGui::TextColored(ImVec4(0.9f, 0.3f, 0.3f, 1.0f), "Project already exists in this location.");
        }

        ImGui::Spacing();
        ImGui::Spacing();

        float create_width = 140.0f;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - create_width) * 0.5f);
        ImGui::BeginDisabled(invalid);
        if (ImGui::Button(ICON_FA_ARROW_UP_RIGHT_FROM_SQUARE " Create Project", ImVec2(create_width, 0))) {            
            _create_new_project(project_folder, new_project_use_git);
            _load_project(project_folder);
            new_project_popup_open = false;
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
    }

    ImGui::End();
}

void EditorApplication::_update_editor()
{
    window.set_title("Ballistic2D Editor - " + project_path);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin(ICON_FA_EYE " Viewport");

    ImVec2 size = ImGui::GetContentRegionAvail();
    if (size.x > 0 && size.y > 0) {
        uint32_t w = (uint32_t)size.x;
        uint32_t h = (uint32_t)size.y;
        renderer.set_size(w, h);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::Image((ImTextureID)(uint64_t)renderer.color_target.image, size, ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::Begin("Back");
    if (ImGui::Button("Launcher")) { project_path.clear(); }
    ImGui::End();
}

}