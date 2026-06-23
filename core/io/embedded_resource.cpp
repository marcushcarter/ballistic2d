#include <core/io/embedded_resource.h>
#include <stb_image.h>
#include <vector>

namespace ballistic2d {

bool win32_get_resource_bytes(const std::wstring& p_resource_name, const void*& r_data, DWORD& r_size)
{
    HMODULE h_module = GetModuleHandleW(nullptr);
    HRSRC h_res_info = FindResourceW(h_module, p_resource_name.c_str(), RT_RCDATA);
    if (!h_res_info) {
        return false;
    }
    HGLOBAL h_res_data = LoadResource(h_module, h_res_info);
    if (!h_res_data) {
        return false;
    }
    r_data = LockResource(h_res_data);
    r_size = SizeofResource(h_module, h_res_info);
    return r_data != nullptr && r_size > 0;
}

HICON EmbeddedResource::load_icon(const std::wstring& p_resource_name) {
    return LoadIconW(GetModuleHandleW(nullptr), p_resource_name.c_str());
}

std::vector<uint8_t> EmbeddedResource::load_font(const std::wstring& p_resource_name)
{
    const void* raw_data = nullptr;
    DWORD raw_size = 0;
    if (!win32_get_resource_bytes(p_resource_name, raw_data, raw_size)) {
        return {};
    }

    const uint8_t* bytes = static_cast<const uint8_t*>(raw_data);
    return std::vector<uint8_t>(bytes, bytes + raw_size);
}

}