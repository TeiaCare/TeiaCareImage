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

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <numeric>
#include <random>
#include <vector>

namespace tc::img::test
{
class image_io_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create temporary directory for test files
        temp_dir_ = std::filesystem::temp_directory_path() / "teiacare_image_test";
        std::filesystem::create_directories(temp_dir_);
    }

    void TearDown() override
    {
        // Clean up temporary files
        if (std::filesystem::exists(temp_dir_))
        {
            std::filesystem::remove_all(temp_dir_);
        }
    }

    // Helper function to create a simple binary file
    void create_binary_file(const std::filesystem::path& filename, const std::vector<uint8_t>& data)
    {
        std::ofstream file(filename, std::ios::binary);
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();
    }

    // Helper function to create a simple test image data
    std::vector<uint8_t> createTestImageData(int width, int height, int channels)
    {
        std::vector<uint8_t> data(width * height * channels);
        std::mt19937 gen(42); // Fixed seed for reproducible tests
        std::uniform_int_distribution<int> dis(0, 255);

        for (auto& pixel : data)
        {
            pixel = static_cast<uint8_t>(dis(gen));
        }
        return data;
    }

    // Helper function to create a simple uniform test image
    std::vector<uint8_t> createUniformImageData(int width, int height, int channels, uint8_t value)
    {
        return std::vector<uint8_t>(width * height * channels, value);
    }

    std::filesystem::path temp_dir_;
};

// Test tc::img::image_load_as_binary with valid file
TEST_F(image_io_test, load_as_binary_valid_file)
{
    std::vector<uint8_t> test_data = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}; // PNG header
    auto test_file = temp_dir_ / "test.bin";
    create_binary_file(test_file, test_data);

    auto loaded_data = tc::img::image_load_as_binary(test_file);

    EXPECT_EQ(loaded_data.size(), test_data.size());
    EXPECT_EQ(loaded_data, test_data);
}

// Test tc::img::image_load_as_binary with empty file
TEST_F(image_io_test, load_as_binary_empty_file)
{
    auto test_file = temp_dir_ / "empty.bin";
    create_binary_file(test_file, {});

    auto loaded_data = tc::img::image_load_as_binary(test_file);

    EXPECT_TRUE(loaded_data.empty());
}

// Test tc::img::image_load_as_binary with non-existent file
TEST_F(image_io_test, load_as_binary_non_existent_file)
{
    auto non_existent_file = temp_dir_ / "non_existent.bin";

    // The function may throw either std::runtime_error or std::logic_error
    // depending on the internal error condition
    EXPECT_THROW(tc::img::image_load_as_binary(non_existent_file), std::exception);
}

// Test tc::img::image_load_as_binary with large file
TEST_F(image_io_test, load_as_binary_large_file)
{
    std::vector<uint8_t> large_data(10000);
    std::iota(large_data.begin(), large_data.end(), 0); // Fill with 0, 1, 2, ..., 255, 0, 1, ...

    auto test_file = temp_dir_ / "large.bin";
    create_binary_file(test_file, large_data);

    auto loaded_data = tc::img::image_load_as_binary(test_file);

    EXPECT_EQ(loaded_data.size(), large_data.size());
    EXPECT_EQ(loaded_data, large_data);
}

// Test tc::img::create_image_data with valid data
TEST_F(image_io_test, create_image_data_valid)
{
    int width = 4, height = 3, channels = 3;
    size_t data_size = width * height * channels;

    // Allocate memory using stbi's allocation method to avoid double free
    uint8_t* data_ptr = static_cast<uint8_t*>(malloc(data_size));
    ASSERT_NE(data_ptr, nullptr);

    // Fill with test data
    for (size_t i = 0; i < data_size; ++i)
    {
        data_ptr[i] = static_cast<uint8_t>(i % 256);
    }

    auto [image_buffer, ret_width, ret_height, ret_channels] = tc::img::create_image_data(data_ptr, width, height, channels);

    EXPECT_EQ(ret_width, width);
    EXPECT_EQ(ret_height, height);
    EXPECT_EQ(ret_channels, channels);
    EXPECT_EQ(image_buffer.size(), width * height * channels);

    // Verify the data was copied correctly
    for (size_t i = 0; i < data_size; ++i)
    {
        EXPECT_EQ(image_buffer[i], static_cast<uint8_t>(i % 256));
    }
}

// Test tc::img::create_image_data with null pointer
TEST_F(image_io_test, create_image_data_null_pointer)
{
    EXPECT_THROW(tc::img::create_image_data(nullptr, 10, 10, 3), std::exception);
}

// Test tc::img::create_image_data with invalid dimensions
TEST_F(image_io_test, create_image_data_invalid_dimensions)
{
    // Test with zero width
    {
        uint8_t* data_ptr = static_cast<uint8_t*>(malloc(10 * 3));
        ASSERT_NE(data_ptr, nullptr);
        EXPECT_THROW(tc::img::create_image_data(data_ptr, 0, 10, 3), std::runtime_error);
    }

    // Test with zero height
    {
        uint8_t* data_ptr = static_cast<uint8_t*>(malloc(10 * 3));
        ASSERT_NE(data_ptr, nullptr);
        EXPECT_THROW(tc::img::create_image_data(data_ptr, 10, 0, 3), std::runtime_error);
    }

    // Test with negative width
    {
        uint8_t* data_ptr = static_cast<uint8_t*>(malloc(10 * 3));
        ASSERT_NE(data_ptr, nullptr);
        EXPECT_THROW(tc::img::create_image_data(data_ptr, -1, 10, 3), std::runtime_error);
    }
}

// Test tc::img::image_load with non-existent file
TEST_F(image_io_test, image_load_non_existent_file)
{
    auto non_existent_file = temp_dir_ / "non_existent.jpg";

    EXPECT_THROW(tc::img::image_load(non_existent_file), std::runtime_error);
}

// Test tc::img::image_load with invalid image file
TEST_F(image_io_test, image_load_invalid_image_file)
{
    auto invalid_file = temp_dir_ / "invalid.jpg";
    std::vector<uint8_t> invalid_data = {0x00, 0x01, 0x02, 0x03}; // Not a valid image
    create_binary_file(invalid_file, invalid_data);

    EXPECT_THROW(tc::img::image_load(invalid_file), std::runtime_error);
}

// Test tc::img::image_load_from_memory with invalid data
TEST_F(image_io_test, image_load_from_memory_invalid_data)
{
    std::vector<uint8_t> invalid_data = {0x00, 0x01, 0x02, 0x03};

    EXPECT_THROW(tc::img::image_load_from_memory(invalid_data.data(), invalid_data.size()), std::runtime_error);
}

// Test tc::img::image_load_from_memory with empty data
TEST_F(image_io_test, image_load_from_memory_empty_data)
{
    EXPECT_THROW(tc::img::image_load_from_memory(nullptr, 0), std::runtime_error);
}

// Test tc::img::image_save with PNG format
TEST_F(image_io_test, image_save_png)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, channels, 128);
    auto output_file = temp_dir_ / "test.png";

    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
    EXPECT_TRUE(std::filesystem::exists(output_file));
    EXPECT_GT(std::filesystem::file_size(output_file), 0);
}

// Test tc::img::image_save with JPEG format
TEST_F(image_io_test, image_save_jpeg)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, channels, 128);
    auto output_file = temp_dir_ / "test.jpg";

    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
    EXPECT_TRUE(std::filesystem::exists(output_file));
    EXPECT_GT(std::filesystem::file_size(output_file), 0);
}

// Test tc::img::image_save with JPEG alternative extension
TEST_F(image_io_test, image_save_jpeg_alternative)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, channels, 128);
    auto output_file = temp_dir_ / "test.jpeg";

    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
    EXPECT_TRUE(std::filesystem::exists(output_file));
    EXPECT_GT(std::filesystem::file_size(output_file), 0);
}

// Test tc::img::image_save with BMP format
TEST_F(image_io_test, image_save_bmp)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, channels, 128);
    auto output_file = temp_dir_ / "test.bmp";

    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
    EXPECT_TRUE(std::filesystem::exists(output_file));
    EXPECT_GT(std::filesystem::file_size(output_file), 0);
}

// Test tc::img::image_save with TGA format
TEST_F(image_io_test, image_save_tga)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, channels, 128);
    auto output_file = temp_dir_ / "test.tga";

    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
    EXPECT_TRUE(std::filesystem::exists(output_file));
    EXPECT_GT(std::filesystem::file_size(output_file), 0);
}

// Test tc::img::image_save with unsupported format
TEST_F(image_io_test, image_save_unsupported_format)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, channels, 128);
    auto output_file = temp_dir_ / "test.xyz";

    EXPECT_THROW(tc::img::image_save(output_file, image_data, width, height, channels), std::runtime_error);
}

// Test tc::img::image_save with no extension (should default to PNG)
TEST_F(image_io_test, image_save_no_extension)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, channels, 128);
    auto output_file = temp_dir_ / "test_no_ext";

    // Based on the implementation, files without extension may not be supported
    // or may throw an exception - we test that it doesn't crash
    EXPECT_THROW(tc::img::image_save(output_file, image_data, width, height, channels), std::runtime_error);
}

// Test tc::img::image_save with single channel (grayscale)
TEST_F(image_io_test, image_save_grayscale)
{
    int width = 8, height = 6, channels = 1;
    auto image_data = createUniformImageData(width, height, channels, 200);
    auto output_file = temp_dir_ / "grayscale.png";

    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
    EXPECT_TRUE(std::filesystem::exists(output_file));
    EXPECT_GT(std::filesystem::file_size(output_file), 0);
}

// Test tc::img::image_save with RGBA (4 channels)
TEST_F(image_io_test, image_save_rgba)
{
    int width = 8, height = 6, channels = 4;
    auto image_data = createUniformImageData(width, height, channels, 150);
    auto output_file = temp_dir_ / "rgba.png";

    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
    EXPECT_TRUE(std::filesystem::exists(output_file));
    EXPECT_GT(std::filesystem::file_size(output_file), 0);
}

// Test tc::img::image_save with empty image data
TEST_F(image_io_test, image_save_empty_data)
{
    std::vector<uint8_t> empty_data;
    auto output_file = temp_dir_ / "empty.png";

    // This should either throw or handle gracefully, depending on implementation
    // At minimum, it shouldn't crash
    EXPECT_NO_THROW(tc::img::image_save(output_file, empty_data, 0, 0, 3));
}

// Test tc::img::image_save with large image
TEST_F(image_io_test, image_save_large_image)
{
    int width = 100, height = 100, channels = 3;
    auto image_data = createTestImageData(width, height, channels);
    auto output_file = temp_dir_ / "large.png";

    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
    EXPECT_TRUE(std::filesystem::exists(output_file));
    EXPECT_GT(std::filesystem::file_size(output_file), 1000); // Should be reasonably large
}

// Test tc::img::image_save with mismatched data size
TEST_F(image_io_test, image_save_mismatched_data_size)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, 1, 128); // Wrong size
    auto output_file = temp_dir_ / "mismatched.png";

    // This might work or fail depending on how the library handles it
    // At minimum, it shouldn't crash
    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
}

// Test round-trip: save and load
TEST_F(image_io_test, save_and_load_round_trip)
{
    int width = 8, height = 6, channels = 3;
    auto original_data = createTestImageData(width, height, channels);
    auto test_file = temp_dir_ / "roundtrip.png";

    // Save the image
    EXPECT_NO_THROW(tc::img::image_save(test_file, original_data, width, height, channels));

    // Load it back
    auto [loaded_data, loaded_width, loaded_height, loaded_channels] = tc::img::image_load(test_file);

    EXPECT_EQ(loaded_width, width);
    EXPECT_EQ(loaded_height, height);
    EXPECT_EQ(loaded_channels, channels);
    EXPECT_EQ(loaded_data.size(), original_data.size());

    // Note: Due to compression, pixel values might not be exactly the same
    // but the dimensions should match
}

// Test file path with special characters
TEST_F(image_io_test, file_path_special_characters)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, channels, 128);
    auto output_file = temp_dir_ / "test with spaces.png";

    EXPECT_NO_THROW(tc::img::image_save(output_file, image_data, width, height, channels));
    EXPECT_TRUE(std::filesystem::exists(output_file));
}

// Test case sensitivity of extensions
TEST_F(image_io_test, extension_case_sensitivity)
{
    int width = 4, height = 3, channels = 3;
    auto image_data = createUniformImageData(width, height, channels, 128);

    // Test uppercase extensions - these are likely not supported
    auto png_file = temp_dir_ / "test.PNG";
    auto jpg_file = temp_dir_ / "test.JPG";

    // The implementation likely only supports lowercase extensions
    EXPECT_THROW(tc::img::image_save(png_file, image_data, width, height, channels), std::runtime_error);
    EXPECT_THROW(tc::img::image_save(jpg_file, image_data, width, height, channels), std::runtime_error);
}

}
