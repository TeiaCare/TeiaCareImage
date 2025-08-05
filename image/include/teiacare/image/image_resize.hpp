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

#pragma once

#include <cstdint>
#include <vector>

namespace tc::img
{
/*!
 * \brief Resize an image while maintaining aspect ratio, storing result in provided vector.
 * \param image Input image data vector
 * \param image_width Width of the input image in pixels
 * \param image_height Height of the input image in pixels
 * \param image_channels Number of color channels in the input image
 * \param target_width Target width for the resized image
 * \param target_height Target height for the resized image
 * \param resized_image Output vector to store the resized image data
 */
void image_resize_aspect_ratio(
    const std::vector<std::uint8_t>& image,
    int image_width,
    int image_height,
    int image_channels,
    int target_width,
    int target_height,
    std::vector<std::uint8_t>& resized_image);

/*!
 * \brief Resize an image while maintaining aspect ratio, returning result as new vector.
 * \param image Input image data vector
 * \param image_width Width of the input image in pixels
 * \param image_height Height of the input image in pixels
 * \param image_channels Number of color channels in the input image
 * \param target_width Target width for the resized image
 * \param target_height Target height for the resized image
 * \return Vector containing the resized image data
 */
std::vector<std::uint8_t> image_resize_aspect_ratio(
    const std::vector<std::uint8_t>& image,
    int image_width,
    int image_height,
    int image_channels,
    int target_width,
    int target_height);

}
