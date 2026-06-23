#pragma once
#include <core/application/application.h>

namespace ballistic2d {

struct EditorApplication : Application
{
    bool new_project_popup_open = false;
    char new_project_name[128] = "";
    char new_project_location[260] = "";
    bool new_project_use_git = true;

    Error on_init() override;
    void on_update(float p_dt) override;
    void on_shutdown() override;

    void _create_new_project(std::string p_project_folder, bool p_version_control);

    void _update_launcher();
    void _update_editor();
};

}