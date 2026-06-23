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

/***************/
/**** IMAGE ****/
/***************/

void RenderingDeviceDriverOpengl::_image_release(Image& r_image)
{
    if (r_image.storage_handle) {
        glMakeImageHandleNonResidentARB(r_image.storage_handle);
        r_image.storage_handle = 0;
    }
    
    if (r_image.sampled_handle) {
        glMakeTextureHandleNonResidentARB(r_image.sampled_handle);
        r_image.sampled_handle = 0;
    }
    
    if (r_image.image) {
        glDeleteTextures(1, &r_image.image);
        r_image.image = 0;
    }
}

RenderingDeviceDriverOpengl::Image RenderingDeviceDriverOpengl::image_create(uint32_t p_width, uint32_t p_height, GLenum p_format)
{
    Image image;

    image.width = p_width;
    image.height = p_height;
    image.format = p_format;

    glCreateTextures(GL_TEXTURE_2D, 1, &image.image);
    glTextureStorage2D(image.image, 1, image.format, static_cast<GLsizei>(image.width), static_cast<GLsizei>(image.height));
    glTextureParameteri(image.image, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(image.image, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(image.image, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(image.image, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    image.sampled_handle = glGetTextureHandleARB(image.image);
    glMakeTextureHandleResidentARB(image.sampled_handle);

    image.storage_handle = glGetImageHandleARB(image.image, 0, GL_FALSE, 0, image.format);
    glMakeImageHandleResidentARB(image.storage_handle, GL_READ_WRITE);

    return image;
}

void RenderingDeviceDriverOpengl::image_free(Image& r_image)
{
    _image_release(r_image);
}

Error RenderingDeviceDriverOpengl::image_resize(Image& r_image, uint32_t p_width, uint32_t p_height)
{
    using enum Error;

    if (r_image.width == p_width && r_image.height == p_height) return Ok;
    if (p_width == 0 || p_height == 0) return Ok;

    _image_release(r_image);
    r_image = image_create(p_width, p_height, r_image.format);

    return Ok;
}

void RenderingDeviceDriverOpengl::image_bind_sampled(Image& r_image, uint32_t p_unit)
{
    glBindTextureUnit(p_unit, r_image.image);
}

void RenderingDeviceDriverOpengl::image_bind_storage(Image& r_image, uint32_t p_unit, GLenum p_access)
{
    glBindImageTexture(p_unit, r_image.image, 0, GL_FALSE, 0, p_access, r_image.format);
}
    
}