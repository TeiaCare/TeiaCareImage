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

#include <teiacare/image/image_color.hpp>
#include <teiacare/image/image_draw.hpp>

#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

namespace tc::img::tests
{
class image_draw_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }

    // Helper function to create a blank RGB image
    std::vector<uint8_t> create_blank_image(int width, int height, uint8_t value = 0)
    {
        return std::vector<uint8_t>(width * height * 3, value);
    }

    // Helper function to get pixel tc::img::color from RGB image
    tc::img::color get_pixel_color(const std::vector<uint8_t>& img, int width, int x, int y)
    {
        int idx = (y * width + x) * 3;
        return tc::img::color(img[idx], img[idx + 1], img[idx + 2]);
    }

    // Helper function to check if a pixel has expected tc::img::color
    bool is_pixel_color(const std::vector<uint8_t>& img, int width, int x, int y, const tc::img::color& expected)
    {
        auto pixel = get_pixel_color(img, width, x, y);
        return pixel == expected;
    }
};

// Test tc::img::set_pixel_rgb bounds checking
TEST_F(image_draw_test, set_pixel_out_of_bounds)
{
    auto img = create_blank_image(10, 10);
    tc::img::color red_color = tc::img::color::red();

    // Store original image for comparison
    auto original_img = img;

    // Test condition: x < 0
    tc::img::set_pixel_rgb(img, 10, 10, -1, 5, red_color);
    EXPECT_EQ(img, original_img); // Image should remain unchanged

    // Test condition: x >= width
    tc::img::set_pixel_rgb(img, 10, 10, 10, 5, red_color);
    EXPECT_EQ(img, original_img); // Image should remain unchanged

    // Test condition: y < 0
    tc::img::set_pixel_rgb(img, 10, 10, 5, -1, red_color);
    EXPECT_EQ(img, original_img); // Image should remain unchanged

    // Test condition: y >= height
    tc::img::set_pixel_rgb(img, 10, 10, 5, 10, red_color);
    EXPECT_EQ(img, original_img); // Image should remain unchanged

    // Test multiple conditions at once
    tc::img::set_pixel_rgb(img, 10, 10, -1, -1, red_color); // x < 0 AND y < 0
    tc::img::set_pixel_rgb(img, 10, 10, 10, 10, red_color); // x >= width AND y >= height
    tc::img::set_pixel_rgb(img, 10, 10, -1, 10, red_color); // x < 0 AND y >= height
    tc::img::set_pixel_rgb(img, 10, 10, 10, -1, red_color); // x >= width AND y < 0
    EXPECT_EQ(img, original_img);                           // Image should remain unchanged

    // Verify that valid coordinates still work
    tc::img::set_pixel_rgb(img, 10, 10, 5, 5, red_color);
    EXPECT_TRUE(is_pixel_color(img, 10, 5, 5, red_color));
}

// Test tc::img::set_pixel_rgb basic functionality
TEST_F(image_draw_test, set_pixel_basic)
{
    auto img = create_blank_image(10, 10);
    tc::img::color red_color = tc::img::color::red();

    tc::img::set_pixel_rgb(img, 10, 10, 5, 5, red_color);

    EXPECT_TRUE(is_pixel_color(img, 10, 5, 5, red_color));

    // Check that other pixels remain unchanged
    EXPECT_TRUE(is_pixel_color(img, 10, 0, 0, tc::img::color::black()));
    EXPECT_TRUE(is_pixel_color(img, 10, 9, 9, tc::img::color::black()));
}

// Test tc::img::set_pixel_rgb at image boundaries
TEST_F(image_draw_test, set_pixel_boundaries)
{
    auto img = create_blank_image(5, 5);
    tc::img::color blue_color = tc::img::color::blue();

    // Test corners
    tc::img::set_pixel_rgb(img, 5, 5, 0, 0, blue_color);
    tc::img::set_pixel_rgb(img, 5, 5, 4, 0, blue_color);
    tc::img::set_pixel_rgb(img, 5, 5, 0, 4, blue_color);
    tc::img::set_pixel_rgb(img, 5, 5, 4, 4, blue_color);

    EXPECT_TRUE(is_pixel_color(img, 5, 0, 0, blue_color));
    EXPECT_TRUE(is_pixel_color(img, 5, 4, 0, blue_color));
    EXPECT_TRUE(is_pixel_color(img, 5, 0, 4, blue_color));
    EXPECT_TRUE(is_pixel_color(img, 5, 4, 4, blue_color));
}

// Test tc::img::set_pixel_rgb with custom tc::img::color
TEST_F(image_draw_test, set_pixel_custom_color)
{
    auto img = create_blank_image(10, 10);
    tc::img::color custom_color(128, 64, 192, 255);

    tc::img::set_pixel_rgb(img, 10, 10, 3, 7, custom_color);

    EXPECT_TRUE(is_pixel_color(img, 10, 3, 7, custom_color));
}

// Test tc::img::draw_rectangle basic functionality
TEST_F(image_draw_test, draw_rectangle_basic)
{
    auto img = create_blank_image(20, 20);
    tc::img::color green_color = tc::img::color::green();

    // Draw a 5x3 rectangle at position (5, 7)
    tc::img::draw_rectangle(img, 20, 20, 5, 7, 5, 3, green_color, 1);

    // Check rectangle boundary pixels
    EXPECT_TRUE(is_pixel_color(img, 20, 5, 7, green_color)); // Top-left
    EXPECT_TRUE(is_pixel_color(img, 20, 9, 7, green_color)); // Top-right
    EXPECT_TRUE(is_pixel_color(img, 20, 5, 9, green_color)); // Bottom-left
    EXPECT_TRUE(is_pixel_color(img, 20, 9, 9, green_color)); // Bottom-right

    // Check some edge pixels
    EXPECT_TRUE(is_pixel_color(img, 20, 7, 7, green_color)); // Top edge
    EXPECT_TRUE(is_pixel_color(img, 20, 5, 8, green_color)); // Left edge

    // Check that pixels outside the rectangle are unchanged
    EXPECT_TRUE(is_pixel_color(img, 20, 4, 7, tc::img::color::black()));
    EXPECT_TRUE(is_pixel_color(img, 20, 10, 7, tc::img::color::black()));
}

// Test tc::img::draw_rectangle with different thickness
TEST_F(image_draw_test, draw_rectangle_thickness)
{
    auto img = create_blank_image(30, 30);
    tc::img::color red_color = tc::img::color::red();

    // Draw rectangle with thickness 3
    tc::img::draw_rectangle(img, 30, 30, 10, 10, 8, 6, red_color, 3);

    // With thickness 3, the rectangle border should be 3 pixels thick
    // The exact implementation might vary, but we can check that pixels
    // around the boundary are colored
    EXPECT_TRUE(is_pixel_color(img, 30, 10, 10, red_color));
    EXPECT_TRUE(is_pixel_color(img, 30, 17, 15, red_color));
}

// Test tc::img::draw_rectangle at image boundaries
TEST_F(image_draw_test, draw_rectangle_boundary)
{
    auto img = create_blank_image(10, 10);
    tc::img::color white_color = tc::img::color::white();

    // Draw rectangle that touches image boundaries
    tc::img::draw_rectangle(img, 10, 10, 0, 0, 10, 10, white_color, 1);

    // Check corners
    EXPECT_TRUE(is_pixel_color(img, 10, 0, 0, white_color));
    EXPECT_TRUE(is_pixel_color(img, 10, 9, 0, white_color));
    EXPECT_TRUE(is_pixel_color(img, 10, 0, 9, white_color));
    EXPECT_TRUE(is_pixel_color(img, 10, 9, 9, white_color));
}

// Test tc::img::draw_line basic functionality
TEST_F(image_draw_test, draw_line_horizontal)
{
    auto img = create_blank_image(20, 20);
    tc::img::color blue_color = tc::img::color::blue();

    // Draw horizontal line from (5, 10) to (15, 10)
    tc::img::draw_line(img, 20, 20, 5, 10, 15, 10, blue_color, 1);

    // Check some points along the line
    EXPECT_TRUE(is_pixel_color(img, 20, 5, 10, blue_color));
    EXPECT_TRUE(is_pixel_color(img, 20, 10, 10, blue_color));
    EXPECT_TRUE(is_pixel_color(img, 20, 15, 10, blue_color));

    // Check that pixels above and below the line are unchanged
    EXPECT_TRUE(is_pixel_color(img, 20, 10, 9, tc::img::color::black()));
    EXPECT_TRUE(is_pixel_color(img, 20, 10, 11, tc::img::color::black()));
}

// Test tc::img::draw_line vertical
TEST_F(image_draw_test, draw_line_vertical)
{
    auto img = create_blank_image(20, 20);
    tc::img::color green_color = tc::img::color::green();

    // Draw vertical line from (8, 3) to (8, 17)
    tc::img::draw_line(img, 20, 20, 8, 3, 8, 17, green_color, 1);

    // Check some points along the line
    EXPECT_TRUE(is_pixel_color(img, 20, 8, 3, green_color));
    EXPECT_TRUE(is_pixel_color(img, 20, 8, 10, green_color));
    EXPECT_TRUE(is_pixel_color(img, 20, 8, 17, green_color));

    // Check that pixels to the left and right are unchanged
    EXPECT_TRUE(is_pixel_color(img, 20, 7, 10, tc::img::color::black()));
    EXPECT_TRUE(is_pixel_color(img, 20, 9, 10, tc::img::color::black()));
}

// Test tc::img::draw_line diagonal
TEST_F(image_draw_test, draw_line_diagonal)
{
    auto img = create_blank_image(20, 20);
    tc::img::color red_color = tc::img::color::red();

    // Draw diagonal line from (5, 5) to (15, 15)
    tc::img::draw_line(img, 20, 20, 5, 5, 15, 15, red_color, 1);

    // Check start and end points
    EXPECT_TRUE(is_pixel_color(img, 20, 5, 5, red_color));
    EXPECT_TRUE(is_pixel_color(img, 20, 15, 15, red_color));

    // Check some middle points (exact pixels depend on line algorithm)
    EXPECT_TRUE(is_pixel_color(img, 20, 10, 10, red_color));
}

// Test tc::img::draw_line with thickness
TEST_F(image_draw_test, draw_line_thickness)
{
    auto img = create_blank_image(30, 30);
    tc::img::color yellow_color(255, 255, 0);

    // Draw line with thickness 3
    tc::img::draw_line(img, 30, 30, 10, 15, 20, 15, yellow_color, 3);

    // The line should be thicker, so adjacent pixels should also be colored
    EXPECT_TRUE(is_pixel_color(img, 30, 15, 15, yellow_color));
    // Exact behavior depends on implementation
}

// Test tc::img::draw_polygon basic functionality
TEST_F(image_draw_test, draw_polygon_triangle)
{
    auto img = create_blank_image(30, 30);
    tc::img::color purple_color(128, 0, 128);

    // Draw a triangle
    std::vector<std::pair<int, int>> triangle_points = {
        {15, 5},  // Top vertex
        {10, 20}, // Bottom left
        {20, 20}  // Bottom right
    };

    tc::img::draw_polygon(img, 30, 30, triangle_points, purple_color, 1);

    // Check that vertices are colored
    EXPECT_TRUE(is_pixel_color(img, 30, 15, 5, purple_color));
    EXPECT_TRUE(is_pixel_color(img, 30, 10, 20, purple_color));
    EXPECT_TRUE(is_pixel_color(img, 30, 20, 20, purple_color));
}

// Test tc::img::draw_polygon square
TEST_F(image_draw_test, draw_polygon_square)
{
    auto img = create_blank_image(25, 25);
    tc::img::color orange_color(255, 165, 0);

    // Draw a square using polygon
    std::vector<std::pair<int, int>> square_points = {
        {10, 10}, // Top left
        {15, 10}, // Top right
        {15, 15}, // Bottom right
        {10, 15}  // Bottom left
    };

    tc::img::draw_polygon(img, 25, 25, square_points, orange_color, 1);

    // Check vertices
    EXPECT_TRUE(is_pixel_color(img, 25, 10, 10, orange_color));
    EXPECT_TRUE(is_pixel_color(img, 25, 15, 10, orange_color));
    EXPECT_TRUE(is_pixel_color(img, 25, 15, 15, orange_color));
    EXPECT_TRUE(is_pixel_color(img, 25, 10, 15, orange_color));
}

// Test tc::img::draw_polygon with thickness
TEST_F(image_draw_test, draw_polygon_thickness)
{
    auto img = create_blank_image(30, 30);
    tc::img::color cyan_color(0, 255, 255);

    std::vector<std::pair<int, int>> triangle_points = {
        {15, 8},
        {12, 18},
        {18, 18}};

    tc::img::draw_polygon(img, 30, 30, triangle_points, cyan_color, 2);

    // With thickness 2, the lines should be thicker
    EXPECT_TRUE(is_pixel_color(img, 30, 15, 8, cyan_color));
    EXPECT_TRUE(is_pixel_color(img, 30, 12, 18, cyan_color));
    EXPECT_TRUE(is_pixel_color(img, 30, 18, 18, cyan_color));
}

// Test tc::img::draw_polygon with single point
TEST_F(image_draw_test, draw_polygon_single_point)
{
    auto img = create_blank_image(20, 20);
    tc::img::color magenta_color(255, 0, 255);

    std::vector<std::pair<int, int>> single_point = {{10, 10}};

    // Drawing a polygon with a single point should work gracefully
    EXPECT_NO_THROW({
        tc::img::draw_polygon(img, 20, 20, single_point, magenta_color, 1);
    });
}

// Test tc::img::draw_polygon with two points (line)
TEST_F(image_draw_test, draw_polygon_two_points)
{
    auto img = create_blank_image(20, 20);
    tc::img::color brown_color(165, 42, 42);

    std::vector<std::pair<int, int>> two_points = {{5, 5}, {15, 15}};

    tc::img::draw_polygon(img, 20, 20, two_points, brown_color, 1);

    // Should draw a line between the two points
    EXPECT_TRUE(is_pixel_color(img, 20, 5, 5, brown_color));
    EXPECT_TRUE(is_pixel_color(img, 20, 15, 15, brown_color));
}

// Test edge case: empty polygon
TEST_F(image_draw_test, draw_polygon_empty)
{
    auto img = create_blank_image(20, 20);
    tc::img::color gray_color(128, 128, 128);

    std::vector<std::pair<int, int>> empty_points;

    // Drawing an empty polygon should not crash
    EXPECT_NO_THROW({
        tc::img::draw_polygon(img, 20, 20, empty_points, gray_color, 1);
    });

    // Image should remain unchanged
    EXPECT_TRUE(is_pixel_color(img, 20, 10, 10, tc::img::color::black()));
}

// Test complex polygon (pentagon)
TEST_F(image_draw_test, draw_polygon_pentagon)
{
    auto img = create_blank_image(40, 40);
    tc::img::color navy_color(0, 0, 128);

    // Draw a pentagon
    std::vector<std::pair<int, int>> pentagon_points = {
        {20, 10}, // Top
        {30, 18}, // Top right
        {26, 30}, // Bottom right
        {14, 30}, // Bottom left
        {10, 18}  // Top left
    };

    tc::img::draw_polygon(img, 40, 40, pentagon_points, navy_color, 1);

    // Check all vertices
    for (const auto& point : pentagon_points)
    {
        EXPECT_TRUE(is_pixel_color(img, 40, point.first, point.second, navy_color));
    }
}

// Test multiple drawing operations on same image
TEST_F(image_draw_test, multiple_drawing_operations)
{
    auto img = create_blank_image(50, 50);

    // Draw rectangle
    tc::img::draw_rectangle(img, 50, 50, 10, 10, 10, 8, tc::img::color::red(), 1);

    // Draw line
    tc::img::draw_line(img, 50, 50, 0, 25, 49, 25, tc::img::color::green(), 1);

    // Set individual pixel
    tc::img::set_pixel_rgb(img, 50, 50, 25, 35, tc::img::color::blue());

    // Verify all operations
    EXPECT_TRUE(is_pixel_color(img, 50, 10, 10, tc::img::color::red()));   // Rectangle
    EXPECT_TRUE(is_pixel_color(img, 50, 25, 25, tc::img::color::green())); // Line
    EXPECT_TRUE(is_pixel_color(img, 50, 25, 35, tc::img::color::blue()));  // Pixel
}

}
