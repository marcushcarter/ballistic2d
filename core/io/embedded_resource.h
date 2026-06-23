#pragma once
#include <windows.h>
#include <string>
#include <vector>

namespace ballistic2d {

struct EmbeddedResource
{
    static HICON load_icon(const std::wstring& p_resource_name);
    static std::vector<uint8_t> load_font(const std::wstring& p_resource_name);
};

}