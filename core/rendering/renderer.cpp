#include <core/rendering/renderer.h>
#include <core/error/error_macros.h>

namespace ballistic2d {

Error Renderer::create(uint32_t p_frame_count)
{
    using enum Error;

    frame_count = p_frame_count;
    current_frame = 0;

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

    return Ok;
}

Error Renderer::begin_frame()
{
    using enum Error;
    
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,width,height);
    
    return Ok;
}

Error Renderer::end_frame()
{
    using enum Error;

    current_frame = (current_frame + 1) % frame_count;

    return Ok;
}

}