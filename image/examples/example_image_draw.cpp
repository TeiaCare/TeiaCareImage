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
 * @example example_image_draw.cpp
 * @brief Drawing primitives example demonstrating tc::img::draw_rectangle, tc::img::draw_polygon, and tc::img::color usage
 */

#include <teiacare/image/image_draw.hpp>
#include <teiacare/image/image_io.hpp>

#include <filesystem>
#include <iostream>
#include <vector>

int main(int, char**)
{
    // Load image from jpeg file
    const std::filesystem::path input_image_path = "img.jpeg";
    auto [img_data, width, height, channels] = tc::img::image_load(input_image_path);

    {
        std::array<int, 4> box = {50, 50, 200, 100};
        tc::img::color color = tc::img::color::red();
        int thickness = 1;
        draw_rectangle(img_data, width, height, box[0], box[1], box[2], box[3], color, thickness);
    }

    {
        std::array<int, 4> box = {300, 200, 150, 250};
        tc::img::color color(255, 165, 0); // Orange color
        int thickness = 3;
        draw_rectangle(img_data, width, height, box[0], box[1], box[2], box[3], color, thickness);
    }

    {
        std::vector<std::pair<int, int>> polygon = {{100, 500}, {200, 600}, {50, 600}};
        draw_polygon(img_data, width, height, polygon, tc::img::color(0, 0, 255), 3);
    }

    {
        std::vector<std::pair<int, int>> polygon = {{150, 350}, {250, 320}, {320, 380}, {280, 450}, {200, 480}, {120, 460}, {80, 400}};
        draw_polygon(img_data, width, height, polygon, tc::img::color(0, 255, 127), 2);
    }

    // Save image to png file
    const std::filesystem::path output_image_path = "img_draw.png";
    tc::img::image_save(output_image_path, img_data, width, height, channels);

    return 0;
}
