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
 * @example example_image_resize.cpp
 * @brief Image resizing example demonstrating tc::img::image_resize_aspect_ratio for preserving aspect ratio during resize operations
 */

#include <teiacare/image/image_io.hpp>
#include <teiacare/image/image_resize.hpp>

#include "image_data_path.hpp"
#include <filesystem>
#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    // Create output directory for images
    std::filesystem::create_directory("img");

    // Load image from jpeg file
    const auto input_image_path = std::filesystem::path(tc::img::examples::image_data_path) / "landscape.jpg";
    auto [img_data, width, height, channels] = tc::img::image_load(input_image_path);

    // Resize image to 300x300 while preserving aspect ratio
    const int target_width = 300;
    const int target_height = 300;
    auto resized_image = tc::img::image_resize_aspect_ratio(img_data, width, height, channels, target_width, target_height);

    // Save image to png file
    const auto output_image_path = std::filesystem::path("img/landscape_resized.png");
    tc::img::image_save(output_image_path, resized_image, target_width, target_height, channels);

    return 0;
}
