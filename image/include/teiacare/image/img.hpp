// Copyright 2024 TeiaCare
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
#include <vector>

namespace tc::img
{
std::vector<uint8_t> load_image_as_binary(const std::filesystem::path& filename);

auto create_image_data(uint8_t* image_data, int width, int height, int channels) -> std::tuple<std::vector<uint8_t>, int, int, int>;

auto load_image(const std::filesystem::path& image_path) -> std::tuple<std::vector<uint8_t>, int, int, int>;

auto load_image_from_memory(uint8_t* memory_data, std::size_t memory_data_size) -> std::tuple<std::vector<uint8_t>, int, int, int>;

void save_image(const std::filesystem::path& output_path, const std::vector<uint8_t>& image_data, int width, int height, int channels);

}
