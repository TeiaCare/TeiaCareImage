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
 * \brief Create a blob from image data with optional preprocessing (in-place version).
 * \tparam T Numeric type for the output blob (typically float or double)
 * \param image Input image data vector
 * \param width Width of the input image in pixels
 * \param height Height of the input image in pixels
 * \param channels Number of color channels in the input image
 * \param blob Output vector to store the processed blob data
 * \param scale_factor Scaling factor applied to pixel values, defaults to 1.0/255.0
 * \param mean Vector of mean values to subtract from each channel, defaults to {0.0, 0.0, 0.0}
 * \param swapRB_channels Whether to swap red and blue channels (RGB to BGR conversion), defaults to false
 */
template <typename T>
void create_blob(
    const std::vector<std::uint8_t>& image,
    int width,
    int height,
    int channels,
    std::vector<T>& blob,
    T scale_factor = 1.0 / 255.0,
    const std::vector<T>& mean = {0.0, 0.0, 0.0},
    bool swapRB_channels = false)
{
    for (int c = 0; c < channels; ++c)
    {
        const int channel_offset = (swapRB_channels ? (2 - c) : c);

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const int idx_offset = y * width + x;
                const int blob_idx = c * height * width + idx_offset;
                const int image_idx = idx_offset * channels + channel_offset;
                blob[blob_idx] = static_cast<T>(image[image_idx]) * scale_factor - mean[c];
            }
        }
    }
}

/*!
 * \brief Create a blob from image data with optional preprocessing (return version).
 * \tparam T Numeric type for the output blob (typically float or double)
 * \param image Input image data vector
 * \param width Width of the input image in pixels
 * \param height Height of the input image in pixels
 * \param channels Number of color channels in the input image
 * \param scale_factor Scaling factor applied to pixel values, defaults to 1.0/255.0
 * \param mean Vector of mean values to subtract from each channel, defaults to {0.0, 0.0, 0.0}
 * \param swapRB_channels Whether to swap red and blue channels (RGB to BGR conversion), defaults to false
 * \return Vector containing the processed blob data
 */
template <typename T>
std::vector<T> create_blob(
    const std::vector<std::uint8_t>& image,
    int width,
    int height,
    int channels,
    T scale_factor = 1.0 / 255.0,
    const std::vector<T>& mean = {0.0, 0.0, 0.0},
    bool swapRB_channels = false)
{
    std::vector<float> blob(channels * width * height);
    create_blob(image, width, height, channels, blob, scale_factor, mean, swapRB_channels);
    return blob;
}

}
