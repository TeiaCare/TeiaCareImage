// Copyright 2025 TeiaCare
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <teiacare/image/image_io.hpp>

//clang-format off
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
//clang-format on

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace tc::img
{
std::vector<uint8_t> image_load_as_binary(const std::filesystem::path& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + filename.string() + " " + std::string(stbi_failure_reason()));
    }

    // Read entire file into a vector
    std::vector<uint8_t> image_buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return image_buffer;
}

auto create_image_data(uint8_t* image_data, int width, int height, int channels) -> std::tuple<std::vector<uint8_t>, int, int, int>
{
    if (!image_data)
    {
        throw std::runtime_error("Error loading image: " + std::string(stbi_failure_reason()));
    }

    if (width <= 0 || height <= 0)
    {
        stbi_image_free(image_data);
        throw std::runtime_error("Invalid image size");
    }

    const size_t image_size = static_cast<size_t>(width * height * channels);
    std::vector<std::uint8_t> image_buffer(image_data, image_data + image_size);
    stbi_image_free(image_data);
    return std::make_tuple(image_buffer, width, height, channels);
}

auto image_load(const std::filesystem::path& image_path) -> std::tuple<std::vector<uint8_t>, int, int, int>
{
    int width, height, channels;
    constexpr const int channels_count = 3;
    uint8_t* image_data = stbi_load(image_path.string().c_str(), &width, &height, &channels, channels_count);
    return create_image_data(image_data, width, height, channels);
}

auto image_load_from_memory(uint8_t* memory_data, std::size_t memory_data_size) -> std::tuple<std::vector<uint8_t>, int, int, int>
{
    int width, height, channels;
    constexpr int channels_count = 3;
    std::uint8_t* image_data = stbi_load_from_memory(memory_data, static_cast<int>(memory_data_size), &width, &height, &channels, channels_count);
    return create_image_data(image_data, width, height, channels);
}

void image_save(const std::filesystem::path& image_path, const uint8_t* image_data_ptr, int width, int height, int channels)
{
    bool ok = false;
    const std::string image_ext = (image_path.has_extension() ? image_path.extension().string() : "png");

    if (image_ext == ".png")
    {
        ok = stbi_write_png(image_path.string().c_str(), width, height, channels, image_data_ptr, width * channels) != 0;
    }
    else if (image_ext == ".jpg" || image_ext == ".jpeg")
    {
        ok = stbi_write_jpg(image_path.string().c_str(), width, height, channels, image_data_ptr, /*quality*/ 100) != 0;
    }
    else if (image_ext == ".bmp")
    {
        ok = stbi_write_bmp(image_path.string().c_str(), width, height, channels, image_data_ptr) != 0;
    }
    else if (image_ext == ".tga")
    {
        ok = stbi_write_tga(image_path.string().c_str(), width, height, channels, image_data_ptr) != 0;
    }
    else
    {
        throw std::runtime_error("Unsupported output format: '" + image_ext + "'. Supported formats are: png, jpg, jpeg, bmp, tga.");
    }
    if (!ok)
    {
        throw std::runtime_error("Failed to write image: " + image_path.string());
    }
}

void image_save(const std::filesystem::path& image_path, const std::vector<uint8_t>& image_data, int width, int height, int channels)
{
    return image_save(image_path, image_data.data(), width, height, channels);
}

}
