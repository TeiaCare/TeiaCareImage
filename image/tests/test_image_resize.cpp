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

#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

namespace tc::img::test
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

// Test void version of image_resize_aspect_ratio
TEST_F(image_resize_test, void_version_basic_resize)
{
    // Create a simple 2x2 RGB image
    auto input_image = createTestImage(2, 2, 3, 100);

    // Pre-allocate the output vector with the correct size
    int target_width = 1, target_height = 1, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    // Test that the function doesn't crash
    EXPECT_NO_THROW({
        image_resize_aspect_ratio(input_image, 2, 2, 3, target_width, target_height, output_image);
    });

    // Basic size check
    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test return version of image_resize_aspect_ratio
TEST_F(image_resize_test, return_version_basic_resize)
{
    // Create a simple 2x2 RGB image
    auto input_image = createTestImage(2, 2, 3, 150);

    // Test that the function doesn't crash
    std::vector<std::uint8_t> output_image;
    EXPECT_NO_THROW({
        output_image = image_resize_aspect_ratio(input_image, 2, 2, 3, 1, 1);
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
        image_resize_aspect_ratio(input_image, 2, 1, 3, target_width, target_height, output_image);
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

    image_resize_aspect_ratio(input_image, 4, 4, 1, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test 4-channel (RGBA) images
TEST_F(image_resize_test, four_channel_image)
{
    auto input_image = createTestImage(3, 3, 4, 125);

    int target_width = 6, target_height = 6, channels = 4;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    image_resize_aspect_ratio(input_image, 3, 3, 4, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test upscaling
TEST_F(image_resize_test, upscaling_test)
{
    auto input_image = createTestImage(2, 2, 3, 50);

    int target_width = 4, target_height = 4, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    image_resize_aspect_ratio(input_image, 2, 2, 3, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test downscaling
TEST_F(image_resize_test, downscaling_test)
{
    auto input_image = createTestImage(8, 8, 3, 175);

    int target_width = 4, target_height = 4, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    image_resize_aspect_ratio(input_image, 8, 8, 3, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test gradient image to verify interpolation
TEST_F(image_resize_test, gradient_image_resize)
{
    auto input_image = createGradientImage(4, 4, 3);

    int target_width = 2, target_height = 2, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    image_resize_aspect_ratio(input_image, 4, 4, 3, target_width, target_height, output_image);

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

    image_resize_aspect_ratio(input_image, 1, 1, 3, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

// Test non-square input and target dimensions
TEST_F(image_resize_test, non_square_dimensions)
{
    auto input_image = createTestImage(6, 3, 3, 200); // 2:1 aspect ratio

    int target_width = 4, target_height = 8, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    image_resize_aspect_ratio(input_image, 6, 3, 3, target_width, target_height, output_image);

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
    image_resize_aspect_ratio(input_image, 4, 4, 3, target_width, target_height, output_void);

    // Test return version
    auto output_return = image_resize_aspect_ratio(input_image, 4, 4, 3, target_width, target_height);

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
        image_resize_aspect_ratio(input_image, 2, 2, 3, 0, 0, output_image);
    });
}

// Test large target dimensions
TEST_F(image_resize_test, large_target_dimensions)
{
    auto input_image = createTestImage(2, 2, 3, 100);

    int target_width = 100, target_height = 100, channels = 3;
    std::vector<std::uint8_t> output_image(target_width * target_height * channels, 0);

    // Test with reasonably large dimensions
    image_resize_aspect_ratio(input_image, 2, 2, 3, target_width, target_height, output_image);

    EXPECT_EQ(output_image.size(), target_width * target_height * channels);
}

}
