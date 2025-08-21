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

#include <filesystem>
#include <tuple>
#include <vector>

namespace tc::img
{
/*!
 * \brief Load an image file as binary data without decoding.
 * \param filename Path to the image file to load
 * \return Vector containing the raw binary data of the image file
 */
auto image_load_as_binary(
    const std::filesystem::path& filename) -> std::vector<uint8_t>;

/*!
 * \brief Create image data tuple from raw image data pointer and dimensions.
 * \param image_data Pointer to the raw image data
 * \param width Width of the image in pixels
 * \param height Height of the image in pixels
 * \param channels Number of color channels (e.g., 3 for RGB, 4 for RGBA)
 * \return Tuple containing the image data vector and dimensions (data, width, height, channels)
 */
auto create_image_data(
    uint8_t* image_data,
    int width,
    int height,
    int channels) -> std::tuple<std::vector<uint8_t>, int, int, int>;

/*!
 * \brief Load an image from file and decode it.
 * \param image_path Path to the image file to load
 * \return Tuple containing the decoded image data and dimensions (data, width, height, channels)
 */
auto image_load(
    const std::filesystem::path& image_path) -> std::tuple<std::vector<uint8_t>, int, int, int>;

/*!
 * \brief Load and decode an image from memory buffer.
 * \param memory_data Pointer to the memory buffer containing image data
 * \param memory_data_size Size of the memory buffer in bytes
 * \return Tuple containing the decoded image data and dimensions (data, width, height, channels)
 */
auto image_load_from_memory(
    uint8_t* memory_data,
    std::size_t memory_data_size) -> std::tuple<std::vector<uint8_t>, int, int, int>;

/*!
 * \brief Save image data to a file.
 * \param output_path Path where the image file should be saved
 * \param image_data Pointer to the image pixel data buffer
 * \param width Width of the image in pixels
 * \param height Height of the image in pixels
 * \param channels Number of color channels (e.g., 3 for RGB, 4 for RGBA)
 */
void image_save(
    const std::filesystem::path& image_path,
    const uint8_t* image_data_ptr,
    int width,
    int height,
    int channels);

/*!
 * \brief Save image data to a file.
 * \param output_path Path where the image file should be saved
 * \param image_data Vector containing the image pixel data
 * \param width Width of the image in pixels
 * \param height Height of the image in pixels
 * \param channels Number of color channels (e.g., 3 for RGB, 4 for RGBA)
 */
void image_save(
    const std::filesystem::path& output_path,
    const std::vector<uint8_t>& image_data,
    int width,
    int height,
    int channels);
}
