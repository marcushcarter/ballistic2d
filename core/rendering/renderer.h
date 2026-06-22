#pragma once
// #include <drivers/vulkan/rendering_device_driver_vulkan.h>
#include <core/error/error.h>
// #include <vector>
#include <cstdint>

namespace ballistic {

struct Renderer
{
    // drivers::RenderingDeviceDriverVulkan* device_driver = nullptr;

    uint32_t frame_count = 1;
    uint32_t current_frame = 0;

    Error create(uint32_t p_frame_count);
    void destroy();

    Error check_resize();

    Error begin_frame();
    Error end_frame();
};

}