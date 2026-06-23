#include <drivers/opengl/rendering_device_driver_opengl.h>
#include <core/error/error_macros.h>
#include <iostream>

namespace ballistic2d::drivers {

/***************/
/**** SETUP ****/
/***************/

void RenderingDeviceDriverOpengl::_get_device_properties()
{
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "OpenGL " << version
        << " - GLSL " << glsl_version
        << " - Using Device: " << renderer << " (" << vendor << ")\n";
}

Error RenderingDeviceDriverOpengl::initialize()
{
    using enum Error;

    _get_device_properties();

    return Ok;
}

void RenderingDeviceDriverOpengl::shutdown()
{
    
}

}