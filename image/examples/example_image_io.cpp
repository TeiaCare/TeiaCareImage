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

/**
 * @example example_image_io.cpp
 * @brief Image input/output example demonstrating tc::img::image_load, tc::img::image_save, tc::img::image_load_as_binary, and tc::img::image_load_from_memory
 */

#include <teiacare/image/image_io.hpp>

#include <filesystem>

int main(int, char**)
{
    {
        // Load image from jpeg file
        const std::filesystem::path input_image_path = "img.jpeg";
        auto [img_data, width, height, channels] = tc::img::image_load(input_image_path);

        // Save image to png file
        const std::filesystem::path output_image_path = "img.png";
        tc::img::image_save(output_image_path, img_data, width, height, channels);
    }

    {
        // Load image from binary data
        const std::filesystem::path input_image_path = "img.jpeg";
        auto bin_img = tc::img::image_load_as_binary(input_image_path);

        // Create image data from binary image data
        auto [img_data, width, height, channels] = tc::img::image_load_from_memory(bin_img.data(), bin_img.size());

        // Save image to png file
        const std::filesystem::path output_image_path = "out.png";
        tc::img::image_save(output_image_path, img_data, width, height, channels);
    }

    return 0;
}
