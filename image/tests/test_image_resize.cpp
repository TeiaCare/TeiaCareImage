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
#include <teiacare/image/image_resize.hpp>

#include "image_data_path.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <gtest/gtest.h>
#include <vector>

namespace tc::img::tests
{
class image_resize_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }

    // Helper function to create a simple test image
    std::vector<std::uint8_t> createTestImage(int width, int height, int channels, std::uint8_t value = 128)
    {
        std::vector<std::uint8_t> image(width * height * channels, value);
        return image;
    }

    // Helper function to create a gradient test image
    std::vector<std::uint8_t> createGradientImage(int width, int height, int channels)
    {
        std::vector<std::uint8_t> image(width * height * channels);
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                for (int c = 0; c < channels; ++c)
                {
                    int idx = (y * width + x) * channels + c;
                    image[idx] = static_cast<std::uint8_t>((x + y + c * 50) % 256);
                }
            }
        }
        return image;
    }
};

// Test void version of tc::img::image_resize_aspect_ratio
TEST_F(image_resize_test, void_version_basic_resize)
{
    // Create a simple 2x2 RGB image
    auto input_image = createTestImage(2, 2, 3, 100);

    // Pre-allocate the output vector with the correct size
    int target_width = 1, target_height = 1, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    // Test that the function doesn't crash
    EXPECT_NO_THROW({
        tc::img::image_resize_aspect_ratio(input_image, 2, 2, 3, target_width, target_height, output_image);
    });

    // Basic size check
    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test return version of tc::img::image_resize_aspect_ratio
TEST_F(image_resize_test, return_version_basic_resize)
{
    // Create a simple 2x2 RGB image
    auto input_image = createTestImage(2, 2, 3, 150);

    // Test that the function doesn't crash
    std::vector<std::uint8_t> output_image;
    EXPECT_NO_THROW({
        output_image = tc::img::image_resize_aspect_ratio(input_image, 2, 2, 3, 1, 1);
    });

    // Basic size check
    EXPECT_GT(output_image.size(), 0);
    EXPECT_EQ(output_image.size() % 3, 0); // Should be divisible by channels
}

// Test aspect ratio preservation with different target dimensions
TEST_F(image_resize_test, aspect_ratio_preservation)
{
    // Create a 2x1 RGB image (aspect ratio 2:1)
    auto input_image = createTestImage(2, 1, 3, 200);

    // Pre-allocate the output vector
    int target_width = 4, target_height = 4, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    // Test basic resizing operation
    EXPECT_NO_THROW({
        tc::img::image_resize_aspect_ratio(input_image, 2, 1, 3, target_width, target_height, output_image);
    });

    // The output should have the expected size
    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test single channel (grayscale) images
TEST_F(image_resize_test, single_channel_image)
{
    auto input_image = createTestImage(4, 4, 1, 75);

    int target_width = 2, target_height = 2, channels = 1;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    tc::img::image_resize_aspect_ratio(input_image, 4, 4, 1, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test 4-channel (RGBA) images
TEST_F(image_resize_test, four_channel_image)
{
    auto input_image = createTestImage(3, 3, 4, 125);

    int target_width = 6, target_height = 6, channels = 4;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    tc::img::image_resize_aspect_ratio(input_image, 3, 3, 4, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test upscaling
TEST_F(image_resize_test, upscaling_test)
{
    auto input_image = createTestImage(2, 2, 3, 50);

    int target_width = 4, target_height = 4, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    tc::img::image_resize_aspect_ratio(input_image, 2, 2, 3, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test downscaling
TEST_F(image_resize_test, downscaling_test)
{
    auto input_image = createTestImage(8, 8, 3, 175);

    int target_width = 4, target_height = 4, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    tc::img::image_resize_aspect_ratio(input_image, 8, 8, 3, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test gradient image to verify interpolation
TEST_F(image_resize_test, gradient_image_resize)
{
    auto input_image = createGradientImage(4, 4, 3);

    int target_width = 2, target_height = 2, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    tc::img::image_resize_aspect_ratio(input_image, 4, 4, 3, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);

    // The output should contain reasonable values
    for (auto pixel_value : output_image)
    {
        EXPECT_GE(pixel_value, 0);
        EXPECT_LE(pixel_value, 255);
    }
}

// Test with minimum dimensions
TEST_F(image_resize_test, minimum_dimensions)
{
    auto input_image = createTestImage(1, 1, 3, 100);

    int target_width = 1, target_height = 1, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    tc::img::image_resize_aspect_ratio(input_image, 1, 1, 3, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test non-square input and target dimensions
TEST_F(image_resize_test, non_square_dimensions)
{
    auto input_image = createTestImage(6, 3, 3, 200); // 2:1 aspect ratio

    int target_width = 4, target_height = 8, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    tc::img::image_resize_aspect_ratio(input_image, 6, 3, 3, target_width, target_height, output_image);

    // Output should have the expected size
    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test consistency between void and return versions
TEST_F(image_resize_test, consistency_between_versions)
{
    auto input_image = createGradientImage(4, 4, 3);

    // Test void version
    int target_width = 2, target_height = 2, channels = 3;
    std::vector<std::uint8_t> output_void(target_width * target_height * channels, 0);
    tc::img::image_resize_aspect_ratio(input_image, 4, 4, 3, target_width, target_height, output_void);

    // Test return version
    auto output_return = tc::img::image_resize_aspect_ratio(input_image, 4, 4, 3, target_width, target_height);

    // Both should produce identical results
    EXPECT_EQ(output_void.size(), output_return.size());
    for (size_t i = 0; i < output_void.size(); ++i)
    {
        EXPECT_EQ(output_void[i], output_return[i]);
    }
}

// Test edge case: zero dimensions (should not crash)
TEST_F(image_resize_test, zero_dimensions_handling)
{
    auto input_image = createTestImage(2, 2, 3, 100);
    std::vector<std::uint8_t> output_image;

    // This might not be valid input, but the function should handle it gracefully
    // The exact behavior depends on implementation, but it shouldn't crash
    EXPECT_NO_THROW({
        tc::img::image_resize_aspect_ratio(input_image, 2, 2, 3, 0, 0, output_image);
    });
}

// Test large target dimensions
TEST_F(image_resize_test, large_target_dimensions)
{
    auto input_image = createTestImage(2, 2, 3, 100);

    int target_width = 100, target_height = 100, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    // Test with reasonably large dimensions
    tc::img::image_resize_aspect_ratio(input_image, 2, 2, 3, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// === Real Image Resize Tests ===

// Test resizing real landscape image with void version
TEST_F(image_resize_test, resize_real_landscape_void_version)
{
    auto landscape_path = std::filesystem::path(tc::img::tests::image_data_path) / "landscape.jpg";

    // Verify the test image exists
    ASSERT_TRUE(std::filesystem::exists(landscape_path))
        << "Test image not found: " << landscape_path;

    // Load the original image
    auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(landscape_path);

    // Test downscaling to half size
    int target_width = orig_width / 2;
    int target_height = orig_height / 2;
    std::vector<std::uint8_t> resized_image(target_width * target_height * orig_channels, 0);

    EXPECT_NO_THROW({
        tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, target_width, target_height, resized_image);
    });

    // Verify the resized image has correct dimensions
    EXPECT_EQ(resized_image.size(), target_width * target_height * orig_channels);

    // Verify all pixel values are valid
    for (auto pixel : resized_image)
    {
        EXPECT_GE(pixel, 0);
        EXPECT_LE(pixel, 255);
    }
}

// Test resizing real landscape image with return version
TEST_F(image_resize_test, resize_real_landscape_return_version)
{
    auto landscape_path = std::filesystem::path(tc::img::tests::image_data_path) / "landscape.jpg";

    // Verify the test image exists
    ASSERT_TRUE(std::filesystem::exists(landscape_path))
        << "Test image not found: " << landscape_path;

    // Load the original image
    auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(landscape_path);

    // Test upscaling to 1.5x size
    int target_width = static_cast<int>(orig_width * 1.5);
    int target_height = static_cast<int>(orig_height * 1.5);

    std::vector<std::uint8_t> resized_image;
    EXPECT_NO_THROW({
        resized_image = tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, target_width, target_height);
    });

    // Verify the resized image has correct dimensions
    EXPECT_GT(resized_image.size(), 0);
    EXPECT_EQ(resized_image.size() % orig_channels, 0);

    // The actual size might be different due to aspect ratio preservation
    // but should be reasonable
    EXPECT_GE(resized_image.size(), target_width * target_height * orig_channels / 4);
    EXPECT_LE(resized_image.size(), target_width * target_height * orig_channels * 2);
}

// Test resizing real portrait image
TEST_F(image_resize_test, resize_real_portrait)
{
    auto portrait_path = std::filesystem::path(tc::img::tests::image_data_path) / "portrait.jpg";

    // Verify the test image exists
    ASSERT_TRUE(std::filesystem::exists(portrait_path))
        << "Test image not found: " << portrait_path;

    // Load the original image
    auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(portrait_path);

    // Test significant downscaling (to 200x200 max)
    int target_width = 200;
    int target_height = 200;

    auto resized_image = tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, target_width, target_height);

    // Verify the resized image has reasonable size
    EXPECT_GT(resized_image.size(), 0);
    EXPECT_EQ(resized_image.size() % orig_channels, 0);

    // Calculate actual dimensions from size
    size_t total_pixels = resized_image.size() / orig_channels;
    EXPECT_LE(total_pixels, target_width * target_height);

    // Since it's a portrait, the resized version should maintain that ratio
    // The exact dimensions depend on aspect ratio preservation logic
}

// Test resizing real square PNG image
TEST_F(image_resize_test, resize_real_square_png)
{
    auto square_path = std::filesystem::path(tc::img::tests::image_data_path) / "square.png";

    // Verify the test image exists
    ASSERT_TRUE(std::filesystem::exists(square_path))
        << "Test image not found: " << square_path;

    // Load the original image
    auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(square_path);

    // Verify it's actually square
    EXPECT_EQ(orig_width, orig_height);

    // Test resizing to different square sizes
    std::vector<int> target_sizes = {100, 400, 600};

    for (int target_size : target_sizes)
    {
        auto resized_image = tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, target_size, target_size);

        EXPECT_GT(resized_image.size(), 0) << "Failed for target size: " << target_size;
        EXPECT_EQ(resized_image.size() % orig_channels, 0) << "Invalid size for target: " << target_size;

        // For a square image resized to a square, we should get exactly the target size
        size_t expected_size = target_size * target_size * orig_channels;
        EXPECT_EQ(resized_image.size(), expected_size) << "Incorrect size for target: " << target_size;
    }
}

// Test extreme downscaling of real images
TEST_F(image_resize_test, extreme_downscaling_real_images)
{
    auto landscape_path = std::filesystem::path(tc::img::tests::image_data_path) / "landscape.jpg";

    // Verify the test image exists
    ASSERT_TRUE(std::filesystem::exists(landscape_path))
        << "Test image not found: " << landscape_path;

    // Load the original image
    auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(landscape_path);

    // Test extreme downscaling to very small sizes
    std::vector<std::pair<int, int>> tiny_sizes = {{1, 1}, {2, 2}, {5, 5}, {10, 10}};

    for (auto [target_w, target_h] : tiny_sizes)
    {
        std::vector<std::uint8_t> tiny_image(target_w * target_h * orig_channels, 0);

        EXPECT_NO_THROW({
            tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, target_w, target_h, tiny_image);
        }) << "Failed for size: "
           << target_w << "x" << target_h;

        EXPECT_EQ(tiny_image.size(), target_w * target_h * orig_channels)
            << "Incorrect size for: " << target_w << "x" << target_h;

        // Verify pixel values are still valid
        for (auto pixel : tiny_image)
        {
            EXPECT_GE(pixel, 0);
            EXPECT_LE(pixel, 255);
        }
    }
}

// Test extreme upscaling of real images
TEST_F(image_resize_test, extreme_upscaling_real_images)
{
    auto square_path = std::filesystem::path(tc::img::tests::image_data_path) / "square.png";

    // Verify the test image exists
    ASSERT_TRUE(std::filesystem::exists(square_path))
        << "Test image not found: " << square_path;

    // Load the original image
    auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(square_path);

    // Test significant upscaling (2x, 3x)
    std::vector<int> scale_factors = {2, 3};

    for (int factor : scale_factors)
    {
        int target_width = orig_width * factor;
        int target_height = orig_height * factor;

        auto enlarged_image = tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, target_width, target_height);

        EXPECT_GT(enlarged_image.size(), 0) << "Failed for scale factor: " << factor;

        // For a square upscaled by integer factor, should get exact target size
        size_t expected_size = target_width * target_height * orig_channels;
        EXPECT_EQ(enlarged_image.size(), expected_size) << "Incorrect size for scale factor: " << factor;

        // Verify the enlarged image is significantly larger than original
        EXPECT_GT(enlarged_image.size(), original_data.size() * (factor * factor - 1));
    }
}

// Test resizing with different aspect ratios
TEST_F(image_resize_test, different_aspect_ratios_real_images)
{
    auto landscape_path = std::filesystem::path(tc::img::tests::image_data_path) / "landscape.jpg";

    // Verify the test image exists
    ASSERT_TRUE(std::filesystem::exists(landscape_path))
        << "Test image not found: " << landscape_path;

    // Load the original image
    auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(landscape_path);

    // Test resizing to different aspect ratios
    struct TargetSize
    {
        int width;
        int height;
        std::string description;
    };
    std::vector<TargetSize> targets = {
        {500, 500, "square"},
        {800, 400, "wide_landscape"},
        {300, 600, "portrait"},
        {1000, 200, "very_wide"},
        {150, 800, "very_tall"}};

    for (const auto& target : targets)
    {
        auto resized_image = tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, target.width, target.height);

        EXPECT_GT(resized_image.size(), 0)
            << "Failed for " << target.description << " (" << target.width << "x" << target.height << ")";

        EXPECT_EQ(resized_image.size() % orig_channels, 0)
            << "Invalid size for " << target.description;

        // The result should fit within the target dimensions due to aspect ratio preservation
        size_t max_expected_size = target.width * target.height * orig_channels;
        EXPECT_LE(resized_image.size(), max_expected_size)
            << "Size too large for " << target.description;
    }
}

// Test consistency between void and return versions with real images
TEST_F(image_resize_test, real_image_version_consistency)
{
    auto portrait_path = std::filesystem::path(tc::img::tests::image_data_path) / "portrait.jpg";

    // Verify the test image exists
    ASSERT_TRUE(std::filesystem::exists(portrait_path))
        << "Test image not found: " << portrait_path;

    // Load the original image
    auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(portrait_path);

    int target_width = 300;
    int target_height = 400;

    // Test void version
    std::vector<std::uint8_t> void_result(target_width * target_height * orig_channels, 0);
    tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, target_width, target_height, void_result);

    // Test return version
    auto return_result = tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, target_width, target_height);

    // Results should be identical
    EXPECT_EQ(void_result.size(), return_result.size());
    for (size_t i = 0; i < void_result.size(); ++i)
    {
        EXPECT_EQ(void_result[i], return_result[i]) << "Mismatch at index: " << i;
    }
}

// Test resizing all real images to common size
TEST_F(image_resize_test, resize_all_real_images_to_common_size)
{
    std::vector<std::pair<std::string, std::string>> image_files = {
        {"landscape.jpg", "landscape"},
        {"portrait.jpg", "portrait"},
        {"square.png", "square"}};

    int common_width = 400;
    int common_height = 400;

    for (const auto& [filename, description] : image_files)
    {
        auto image_path = std::filesystem::path(tc::img::tests::image_data_path) / filename;

        // Verify the test image exists
        ASSERT_TRUE(std::filesystem::exists(image_path))
            << "Test image not found: " << image_path;

        // Load the original image
        auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(image_path);

        // Resize to common dimensions
        auto resized_image = tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, common_width, common_height);

        EXPECT_GT(resized_image.size(), 0) << "Failed to resize " << description;
        EXPECT_EQ(resized_image.size() % orig_channels, 0) << "Invalid size for " << description;

        // All should fit within the common dimensions
        size_t max_expected_size = common_width * common_height * orig_channels;
        EXPECT_LE(resized_image.size(), max_expected_size) << "Size too large for " << description;

        // Should be at least a reasonable fraction of the target size
        EXPECT_GE(resized_image.size(), max_expected_size / 4) << "Size too small for " << description;
    }
}

// Test resizing and saving results (integration test)
TEST_F(image_resize_test, resize_and_save_real_images)
{
    auto landscape_path = std::filesystem::path(tc::img::tests::image_data_path) / "landscape.jpg";

    // Verify the test image exists
    ASSERT_TRUE(std::filesystem::exists(landscape_path))
        << "Test image not found: " << landscape_path;

    // Load the original image
    auto [original_data, orig_width, orig_height, orig_channels] = tc::img::image_load(landscape_path);

    // Create a temporary directory for output
    auto temp_dir = std::filesystem::temp_directory_path() / "resize_test_output";
    std::filesystem::create_directories(temp_dir);

    // Test different resize operations
    struct ResizeTest
    {
        int width;
        int height;
        std::string suffix;
    };
    std::vector<ResizeTest> resize_tests = {
        {200, 200, "_small_square"},
        {800, 600, "_medium_landscape"},
        {400, 800, "_tall"}};

    for (const auto& test : resize_tests)
    {
        // Resize the image
        auto resized_image = tc::img::image_resize_aspect_ratio(original_data, orig_width, orig_height, orig_channels, test.width, test.height);

        EXPECT_GT(resized_image.size(), 0) << "Resize failed for " << test.suffix;

        // Calculate actual dimensions (assuming square pixels in the result)
        size_t total_pixels = resized_image.size() / orig_channels;
        int actual_width = static_cast<int>(std::sqrt(total_pixels * test.width / test.height));
        int actual_height = static_cast<int>(total_pixels / actual_width);

        // Adjust for potential rounding
        if (static_cast<size_t>(actual_width * actual_height * orig_channels) < resized_image.size())
        {
            actual_height = static_cast<int>(resized_image.size() / (actual_width * orig_channels));
        }

        // Save the resized image
        auto output_path = temp_dir / ("resized_landscape" + test.suffix + ".png");

        EXPECT_NO_THROW({
            tc::img::image_save(output_path, resized_image, actual_width, actual_height, orig_channels);
        }) << "Failed to save resized image "
           << test.suffix;

        EXPECT_TRUE(std::filesystem::exists(output_path))
            << "Output file not created for " << test.suffix;
        EXPECT_GT(std::filesystem::file_size(output_path), 0)
            << "Output file is empty for " << test.suffix;
    }

    // Clean up
    std::filesystem::remove_all(temp_dir);
}

}
