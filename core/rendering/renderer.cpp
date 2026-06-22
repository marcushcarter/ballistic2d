#include <core/rendering/renderer.h>
#include <core/error/error_macros.h>

namespace ballistic {

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

Error Renderer::check_resize()
{
    using enum Error;

    return Ok;
}

Error Renderer::begin_frame()
{
    using enum Error;
    
    return Ok;
}

Error Renderer::end_frame()
{
    using enum Error;

    current_frame = (current_frame + 1) % frame_count;

    return Ok;
}

}