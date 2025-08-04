#pragma once

#include <cstdint>
#include <vector>

namespace tc::cms
{
struct image_buffer
{
    std::vector<uint8_t> data;
    int width;
    int height;
    int channels;
};

}
