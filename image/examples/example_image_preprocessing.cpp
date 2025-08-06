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
 * @example example_image_preprocessing.cpp
 * @brief Image preprocessing pipeline example demonstrating tc::img::image_resize_aspect_ratio and tc::img::create_blob for ML preprocessing
 */

#include <teiacare/image/image_io.hpp>
#include <teiacare/image/image_processing.hpp>
#include <teiacare/image/image_resize.hpp>

#include "image_data_path.hpp"
#include <cassert>
#include <filesystem>
#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    // Load image from jpeg file
    const auto input_image_path = std::filesystem::path(tc::img::examples::image_data_path) / "portrait.jpg";
    auto [img_data, width, height, channels] = tc::img::image_load(input_image_path);

    // Preprocess image with aspect ratio resizing and blob creation
    const int target_width = 640;
    const int target_height = 640;
    const float scale_factor = 1.0f / 255.0f;
    const std::vector<float> mean = {0.0f, 0.0f, 0.0f};
    const bool swapRB_channels = false;

    const std::vector<std::uint8_t> resized_image = tc::img::image_resize_aspect_ratio(img_data, width, height, channels, target_width, target_height);
    const std::vector<float> blob = tc::img::create_blob(resized_image, target_width, target_height, channels, scale_factor, mean, swapRB_channels);

    std::cout << "Blob size: " << blob.size() << std::endl;
    std::cout << "Img size: " << target_width * target_height * channels << std::endl;
    assert(blob.size() == target_width * target_height * channels);

    return 0;
}
