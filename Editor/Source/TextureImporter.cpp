#include "TextureImporter.h"

#include <Resource/Texture.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBIR_FLAG_ALPHA_PREMULTIPLIED
#include <stb_image.h>

 gear::Texture* ImportTexture2D(const std::string& file) {
    // 加载纹理数据
    int width, height, channels;
    unsigned char* pixels = stbi_load(file.c_str(), &width, &height, &channels, STBI_rgb_alpha);

     // 预乘处理
    if (channels == 4) {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                unsigned char* pixel_offset = pixels + (i + width * j) * 4;
                float alpha = pixel_offset[3] / 255.0f;
                pixel_offset[0] *= alpha;
                pixel_offset[1] *= alpha;
                pixel_offset[2] *= alpha;
            }
        }
    }

    gear::Texture::Builder builder;
    builder.SetWidth(width);
    builder.SetHeight(height);
    builder.SetFormat(blast::FORMAT_R8G8B8A8_UNORM);
    gear::Texture* texture = builder.Build();
    texture->UpdateData(pixels);
    stbi_image_free(pixels);
    return texture;
}

gear::Texture* ImportTexture2DWithFloat(const std::string& file) {
    // 加载纹理数据
    int width, height, channels;
    float* pixels = stbi_loadf(file.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    // 预乘处理
    if (channels == 4) {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                float* pixel_offset = pixels + (i + width * j) * 4;
                float alpha = pixel_offset[3] / 255.0f;
                pixel_offset[0] *= alpha;
                pixel_offset[1] *= alpha;
                pixel_offset[2] *= alpha;
            }
        }
    }

    gear::Texture::Builder builder;
    builder.SetWidth(width);
    builder.SetHeight(height);
    builder.SetFormat(blast::FORMAT_R32G32B32A32_FLOAT);
    gear::Texture* texture = builder.Build();
    texture->UpdateData(pixels);
    stbi_image_free(pixels);
    return texture;
}