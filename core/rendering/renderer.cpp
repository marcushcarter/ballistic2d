#include <core/rendering/renderer.h>
#include <core/error/error_macros.h>
#include <iostream>

namespace ballistic2d {

Error Renderer::create(uint32_t p_frame_count)
{
    using enum Error;

    frame_count = p_frame_count;
    current_frame = 0;

    color_target = device_driver->image_create(1, 1, GL_RGBA8);
    depth_target = device_driver->image_create(1, 1, GL_DEPTH24_STENCIL8);
    picking_target = device_driver->image_create(1, 1, GL_R32UI);
    framebuffer = device_driver->framebuffer_create({ &color_target, &picking_target }, &depth_target);

    return Ok;
}

void Renderer::destroy()
{

}

Error Renderer::set_size(uint32_t p_width, uint32_t p_height)
{
    using enum Error;

    if (p_width == 0 || p_height == 0) return Ok;
    if (p_width == width && p_height == height) return Ok;

    width = p_width;
    height = p_height;

    device_driver->framebuffer_free(framebuffer);
    device_driver->image_resize(color_target, p_width, p_height);
    device_driver->image_resize(depth_target, p_width, p_height);
    device_driver->image_resize(picking_target, p_width, p_height);
    framebuffer = device_driver->framebuffer_create({ &color_target, &picking_target }, &depth_target);

    // std::cout << "widht: " << p_width << " height: " << p_height << "\n";

    return Ok;
}

Error Renderer::begin_frame()
{
    using enum Error;
    
    device_driver->framebuffer_unbind();
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    device_driver->framebuffer_bind(framebuffer);
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    return Ok;
}

Error Renderer::end_frame()
{
    using enum Error;

    current_frame = (current_frame + 1) % frame_count;

    return Ok;
}

}