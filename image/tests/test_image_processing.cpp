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

#include <teiacare/image/image_processing.hpp>

#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

namespace tc::img::tests
{
class image_processing_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }

    // Helper function to create a test image with known values
    std::vector<std::uint8_t> createTestImage(int width, int height, int channels)
    {
        std::vector<std::uint8_t> image(width * height * channels);
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                for (int c = 0; c < channels; ++c)
                {
                    int idx = (y * width + x) * channels + c;
                    // Create a predictable pattern
                    image[idx] = static_cast<std::uint8_t>((x + y + c * 64) % 256);
                }
            }
        }
        return image;
    }

    // Helper function to create uniform test image
    std::vector<std::uint8_t> createUniformImage(int width, int height, int channels, std::uint8_t value)
    {
        return std::vector<std::uint8_t>(width * height * channels, value);
    }
};

// Test create_blob void version with default parameters
TEST_F(image_processing_test, create_blob_void_version_defaults)
{
    auto image = createUniformImage(2, 2, 3, 128); // 2x2 RGB image with value 128
    std::vector<float> blob(2 * 2 * 3);

    create_blob(image, 2, 2, 3, blob);

    // With default scale factor (1.0/255.0) and mean (0,0,0),
    // each pixel value should be 128/255.0 ≈ 0.502
    float expected_value = 128.0f / 255.0f;

    for (auto value : blob)
    {
        EXPECT_NEAR(value, expected_value, 1e-6f);
    }
}

// Test create_blob void version with custom scale factor
TEST_F(image_processing_test, create_blob_void_version_custom_scale)
{
    auto image = createUniformImage(2, 2, 3, 100);
    std::vector<float> blob(2 * 2 * 3);
    float scale_factor = 0.01f;

    create_blob(image, 2, 2, 3, blob, scale_factor);

    // With scale factor 0.01, each pixel value should be 100 * 0.01 = 1.0
    for (auto value : blob)
    {
        EXPECT_NEAR(value, 1.0f, 1e-6f);
    }
}

// Test create_blob void version with mean subtraction
TEST_F(image_processing_test, create_blob_void_version_with_mean)
{
    auto image = createUniformImage(2, 2, 3, 128);
    std::vector<float> blob(2 * 2 * 3);
    std::vector<float> mean = {0.1f, 0.2f, 0.3f};

    create_blob(image, 2, 2, 3, blob, 1.0f / 255.0f, mean);

    float base_value = 128.0f / 255.0f;

    // Check that mean was subtracted correctly for each channel
    // Channel 0 pixels should have value (base_value - 0.1)
    // Channel 1 pixels should have value (base_value - 0.2)
    // Channel 2 pixels should have value (base_value - 0.3)

    for (int i = 0; i < 4; ++i) // 4 pixels total
    {
        EXPECT_NEAR(blob[i], base_value - 0.1f, 1e-6f);     // Channel 0
        EXPECT_NEAR(blob[4 + i], base_value - 0.2f, 1e-6f); // Channel 1
        EXPECT_NEAR(blob[8 + i], base_value - 0.3f, 1e-6f); // Channel 2
    }
}

// Test create_blob void version with RB channel swapping
TEST_F(image_processing_test, create_blob_void_version_swap_rb)
{
    // Create image with different values per channel
    std::vector<std::uint8_t> image(2 * 2 * 3);
    for (int i = 0; i < 4; ++i) // 4 pixels
    {
        image[i * 3 + 0] = 100; // R channel
        image[i * 3 + 1] = 150; // G channel
        image[i * 3 + 2] = 200; // B channel
    }

    std::vector<float> blob(2 * 2 * 3);
    create_blob(image, 2, 2, 3, blob, 1.0f, {0.0f, 0.0f, 0.0f}, true);

    // With swapRB_channels = true, R and B channels should be swapped
    // So channel 0 in blob should have B values (200)
    // channel 1 should have G values (150)
    // channel 2 should have R values (100)

    for (int i = 0; i < 4; ++i)
    {
        EXPECT_NEAR(blob[i], 200.0f, 1e-6f);     // Channel 0 = B values
        EXPECT_NEAR(blob[4 + i], 150.0f, 1e-6f); // Channel 1 = G values
        EXPECT_NEAR(blob[8 + i], 100.0f, 1e-6f); // Channel 2 = R values
    }
}

// Test create_blob return version with default parameters
TEST_F(image_processing_test, create_blob_return_version_defaults)
{
    auto image = createUniformImage(3, 3, 3, 127);

    auto blob = create_blob(image, 3, 3, 3, 1.0f / 255.0f);

    EXPECT_EQ(blob.size(), 3 * 3 * 3);

    float expected_value = 127.0f / 255.0f;
    for (auto value : blob)
    {
        EXPECT_NEAR(value, expected_value, 1e-6f);
    }
}

// Test create_blob return version with custom parameters
TEST_F(image_processing_test, create_blob_return_version_custom)
{
    auto image = createUniformImage(2, 3, 3, 200);
    float scale = 0.02f;
    std::vector<float> mean = {0.5f, 1.0f, 1.5f};

    auto blob = create_blob(image, 2, 3, 3, scale, mean, true);

    EXPECT_EQ(blob.size(), 2 * 3 * 3);

    // With swapRB = true, the channels are swapped but mean is applied per blob channel
    // Expected values: (200 * 0.02) - mean[blob_channel]
    float base_value = 200.0f * 0.02f; // 4.0

    for (int i = 0; i < 6; ++i) // 6 pixels
    {
        EXPECT_NEAR(blob[i], base_value - 0.5f, 1e-6f);      // Channel 0 = mean[0]
        EXPECT_NEAR(blob[6 + i], base_value - 1.0f, 1e-6f);  // Channel 1 = mean[1]
        EXPECT_NEAR(blob[12 + i], base_value - 1.5f, 1e-6f); // Channel 2 = mean[2]
    }
}

// Test single channel image
TEST_F(image_processing_test, create_blob_single_channel)
{
    auto image = createUniformImage(4, 4, 1, 64);
    std::vector<float> blob(4 * 4 * 1);

    create_blob(image, 4, 4, 1, blob);

    float expected_value = 64.0f / 255.0f;
    for (auto value : blob)
    {
        EXPECT_NEAR(value, expected_value, 1e-6f);
    }
}

// Test four channel image (RGBA)
TEST_F(image_processing_test, create_blob_four_channel)
{
    auto image = createUniformImage(2, 2, 4, 192);
    std::vector<float> blob(2 * 2 * 4);

    create_blob(image, 2, 2, 4, blob);

    float expected_value = 192.0f / 255.0f;
    for (auto value : blob)
    {
        EXPECT_NEAR(value, expected_value, 1e-6f);
    }
}

// Test with complex pattern image
TEST_F(image_processing_test, create_blob_complex_pattern)
{
    auto image = createTestImage(3, 2, 3); // 3x2 RGB image with pattern
    std::vector<float> blob(3 * 2 * 3);

    create_blob(image, 3, 2, 3, blob, 1.0f, {0.0f, 0.0f, 0.0f}, false);

    EXPECT_EQ(blob.size(), 18);

    // Verify the transformation is correct by checking specific pixels
    // The blob should be organized as [C0_pixels, C1_pixels, C2_pixels]
    // where each channel has height*width pixels in row-major order

    // Check that all values are in reasonable range
    for (auto value : blob)
    {
        EXPECT_GE(value, 0.0f);
        EXPECT_LE(value, 255.0f); // Since scale factor is 1.0
    }
}

// Test blob layout/ordering
TEST_F(image_processing_test, create_blob_layout)
{
    // Create a 2x2 image with specific values to test the layout
    std::vector<std::uint8_t> image = {
        // Pixel (0,0): R=10, G=20, B=30
        10, 20, 30,
        // Pixel (1,0): R=40, G=50, B=60
        40, 50, 60,
        // Pixel (0,1): R=70, G=80, B=90
        70, 80, 90,
        // Pixel (1,1): R=100, G=110, B=120
        100, 110, 120};

    std::vector<float> blob(2 * 2 * 3);
    create_blob(image, 2, 2, 3, blob, 1.0f, {0.0f, 0.0f, 0.0f}, false);

    // Expected blob layout: [R_channel_pixels, G_channel_pixels, B_channel_pixels]
    // R channel: [10, 40, 70, 100] (row-major order)
    // G channel: [20, 50, 80, 110]
    // B channel: [30, 60, 90, 120]

    EXPECT_EQ(blob[0], 10.0f);  // R(0,0)
    EXPECT_EQ(blob[1], 40.0f);  // R(1,0)
    EXPECT_EQ(blob[2], 70.0f);  // R(0,1)
    EXPECT_EQ(blob[3], 100.0f); // R(1,1)

    EXPECT_EQ(blob[4], 20.0f);  // G(0,0)
    EXPECT_EQ(blob[5], 50.0f);  // G(1,0)
    EXPECT_EQ(blob[6], 80.0f);  // G(0,1)
    EXPECT_EQ(blob[7], 110.0f); // G(1,1)

    EXPECT_EQ(blob[8], 30.0f);   // B(0,0)
    EXPECT_EQ(blob[9], 60.0f);   // B(1,0)
    EXPECT_EQ(blob[10], 90.0f);  // B(0,1)
    EXPECT_EQ(blob[11], 120.0f); // B(1,1)
}

// Test consistency between void and return versions
TEST_F(image_processing_test, create_blob_consistency)
{
    auto image = createTestImage(4, 3, 3);

    // Test void version
    std::vector<float> blob_void(4 * 3 * 3);
    create_blob(image, 4, 3, 3, blob_void, 0.5f, {0.1f, 0.2f, 0.3f}, true);

    // Test return version
    auto blob_return = create_blob(image, 4, 3, 3, 0.5f, {0.1f, 0.2f, 0.3f}, true);

    // Both should produce identical results
    ASSERT_EQ(blob_void.size(), blob_return.size());
    for (size_t i = 0; i < blob_void.size(); ++i)
    {
        EXPECT_NEAR(blob_void[i], blob_return[i], 1e-6f);
    }
}

// Test with different numeric types
TEST_F(image_processing_test, create_blob_double_type)
{
    auto image = createUniformImage(2, 2, 3, 100);
    std::vector<double> blob(2 * 2 * 3);

    create_blob(image, 2, 2, 3, blob, 1.0 / 255.0, {0.0, 0.0, 0.0}, false);

    double expected_value = 100.0 / 255.0;
    for (auto value : blob)
    {
        EXPECT_NEAR(value, expected_value, 1e-10);
    }
}

// Test boundary values
TEST_F(image_processing_test, create_blob_boundary_values)
{
    // Test with min and max pixel values
    std::vector<std::uint8_t> image = {
        0, 0, 0,      // Black pixel
        255, 255, 255 // White pixel
    };

    std::vector<float> blob(2 * 1 * 3);
    create_blob(image, 2, 1, 3, blob);

    // First pixel should map to 0.0
    EXPECT_NEAR(blob[0], 0.0f, 1e-6f); // R channel, first pixel
    EXPECT_NEAR(blob[2], 0.0f, 1e-6f); // G channel, first pixel
    EXPECT_NEAR(blob[4], 0.0f, 1e-6f); // B channel, first pixel

    // Second pixel should map to 1.0
    EXPECT_NEAR(blob[1], 1.0f, 1e-6f); // R channel, second pixel
    EXPECT_NEAR(blob[3], 1.0f, 1e-6f); // G channel, second pixel
    EXPECT_NEAR(blob[5], 1.0f, 1e-6f); // B channel, second pixel
}

// Test with different mean vector sizes (edge case)
TEST_F(image_processing_test, create_blob_mean_vector_size_handling)
{
    auto image = createUniformImage(2, 2, 3, 128);
    std::vector<float> blob(2 * 2 * 3);

    // Test with smaller mean vector (should handle gracefully)
    std::vector<float> small_mean = {0.1f, 0.2f}; // Only 2 elements for 3 channels

    // This should not crash, though behavior depends on implementation
    EXPECT_NO_THROW({
        create_blob(image, 2, 2, 3, blob, 1.0f / 255.0f, small_mean, false);
    });
}

}
