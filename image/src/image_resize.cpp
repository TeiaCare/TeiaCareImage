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

#include <teiacare/image/image_resize.hpp>

#include <algorithm>

namespace tc::img
{
void image_resize_aspect_ratio(
    const std::vector<std::uint8_t>& image,
    int image_width,
    int image_height,
    int image_channels,
    int target_width,
    int target_height,
    std::vector<std::uint8_t>& resized_image)
{
    // Calculate the aspect ratios
    double aspect_ratio_image = static_cast<double>(image_width) / image_height;
    double aspect_ratio_target = static_cast<double>(target_width) / target_height;

    // Determine the scaling factors and new dimensions
    int new_width;
    int new_height;
    if (aspect_ratio_image > aspect_ratio_target)
    {
        new_width = target_width;
        new_height = static_cast<int>(target_width / aspect_ratio_image);
    }
    else
    {
        new_height = target_height;
        new_width = static_cast<int>(target_height * aspect_ratio_image);
    }

    // Calculate padding
    int pad_x = (target_width - new_width) / 2;
    int pad_y = (target_height - new_height) / 2;

    // Scale factors
    double scale_x = static_cast<double>(image_width) / new_width;
    double scale_y = static_cast<double>(image_height) / new_height;

    // Resize with aspect ratio preservation
    for (int y = 0; y < new_height; ++y)
    {
        for (int x = 0; x < new_width; ++x)
        {
            int src_x = std::min(static_cast<int>(x * scale_x), image_width - 1);
            int src_y = std::min(static_cast<int>(y * scale_y), image_height - 1);

            for (int c = 0; c < image_channels; ++c)
            {
                auto idx = ((y + pad_y) * target_width + (x + pad_x)) * image_channels + c;
                resized_image[idx] = image[(src_y * image_width + src_x) * image_channels + c];
            }
        }
    }
}

std::vector<std::uint8_t> image_resize_aspect_ratio(
    const std::vector<std::uint8_t>& image,
    int image_width,
    int image_height,
    int image_channels,
    int target_width,
    int target_height)
{
    std::vector<std::uint8_t> resized_image(target_width * target_height * image_channels, std::uint8_t(0));
    image_resize_aspect_ratio(image, image_width, image_height, image_channels, target_width, target_height, resized_image);
    return resized_image;
}

}
