#pragma once

#include <drivers/opengl/ballistic_opengl.h>
#include <core/error/error.h>

#include <vector>

namespace ballistic2d::drivers {

struct RenderingDeviceDriverOpengl
{
    /***************/
    /**** SETUP ****/
    /***************/

    void _get_device_properties();

    Error initialize();
    void shutdown();
    
    /***************/
    /**** IMAGE ****/
    /***************/

    struct Image {
        uint32_t image = 0;
        uint64_t storage_handle = 0;
        uint64_t sampled_handle = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        GLenum format = GL_RGBA8_SNORM;
    };

    void _image_release(Image& r_image);

    Image image_create(uint32_t p_width, uint32_t p_height, GLenum p_format);
    void image_free(Image& r_image);
    Error image_resize(Image& r_image, uint32_t p_width, uint32_t p_height);
    void image_bind_sampled(const Image& r_image, uint32_t p_unit = 0) const;
    void image_bind_storage(const Image& r_image, uint32_t p_unit = 0, GLenum p_access = GL_READ_WRITE) const;
    
    /*********************/
    /**** FRAMEBUFFER ****/
    /*********************/

    struct Framebuffer {
        uint32_t framebuffer = 0;
    };

    Framebuffer framebuffer_create(const std::vector<Image*>&p_colors, Image* p_depth);
    void framebuffer_free(Framebuffer& r_framebuffer);
    void framebuffer_bind(const Framebuffer& r_framebuffer) const;
    static void framebuffer_unbind();
    
    /****************/
    /**** SHADER ****/
    /****************/
};

}