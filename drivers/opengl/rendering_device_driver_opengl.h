#pragma once

#include <drivers/opengl/ballistic_opengl.h>
#include <core/error/error.h>

namespace ballistic2d::drivers {

struct RenderingDeviceDriverOpengl
{
    /***************/
    /**** SETUP ****/
    /***************/

    void _get_device_properties();

    Error initialize();
    void shutdown();
};

}