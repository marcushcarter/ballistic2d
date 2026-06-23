#pragma once
#include <drivers/opengl/rendering_device_driver_opengl.h>
#include <core/error/error.h>
// #include <vector>
#include <cstdint>

namespace ballistic2d {

struct Renderer
{
    drivers::RenderingDeviceDriverOpengl* device_driver = nullptr;

    uint32_t width = 0;
    uint32_t height = 0;

    uint32_t frame_count = 1;
    uint32_t current_frame = 0;

    Error create(uint32_t p_frame_count);
    void destroy();

    Error set_size(uint32_t p_width, uint32_t p_height);

    Error begin_frame();
    Error end_frame();
};

}