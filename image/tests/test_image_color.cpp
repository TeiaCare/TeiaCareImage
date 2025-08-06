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

#include <gtest/gtest.h>

namespace tc::img::tests
{
class color_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

// Test default constructor
TEST_F(color_test, default_constructor)
{
    tc::img::color c;
    EXPECT_EQ(c.r, 0);
    EXPECT_EQ(c.g, 0);
    EXPECT_EQ(c.b, 0);
    EXPECT_EQ(c.a, 255);
}

// Test parameterized constructor with alpha
TEST_F(color_test, parameterized_constructor_with_alpha)
{
    tc::img::color c(100, 150, 200, 128);
    EXPECT_EQ(c.r, 100);
    EXPECT_EQ(c.g, 150);
    EXPECT_EQ(c.b, 200);
    EXPECT_EQ(c.a, 128);
}

// Test parameterized constructor without alpha (default alpha = 255)
TEST_F(color_test, parameterized_constructor_without_alpha)
{
    tc::img::color c(75, 125, 175);
    EXPECT_EQ(c.r, 75);
    EXPECT_EQ(c.g, 125);
    EXPECT_EQ(c.b, 175);
    EXPECT_EQ(c.a, 255);
}

// Test static factory methods
TEST_F(color_test, static_red)
{
    auto red_color = tc::img::color::red();
    EXPECT_EQ(red_color.r, 255);
    EXPECT_EQ(red_color.g, 0);
    EXPECT_EQ(red_color.b, 0);
    EXPECT_EQ(red_color.a, 255);
}

TEST_F(color_test, static_green)
{
    auto green_color = tc::img::color::green();
    EXPECT_EQ(green_color.r, 0);
    EXPECT_EQ(green_color.g, 255);
    EXPECT_EQ(green_color.b, 0);
    EXPECT_EQ(green_color.a, 255);
}

TEST_F(color_test, static_blue)
{
    auto blue_color = tc::img::color::blue();
    EXPECT_EQ(blue_color.r, 0);
    EXPECT_EQ(blue_color.g, 0);
    EXPECT_EQ(blue_color.b, 255);
    EXPECT_EQ(blue_color.a, 255);
}

TEST_F(color_test, static_white)
{
    auto white_color = tc::img::color::white();
    EXPECT_EQ(white_color.r, 255);
    EXPECT_EQ(white_color.g, 255);
    EXPECT_EQ(white_color.b, 255);
    EXPECT_EQ(white_color.a, 255);
}

TEST_F(color_test, static_black)
{
    auto black_color = tc::img::color::black();
    EXPECT_EQ(black_color.r, 0);
    EXPECT_EQ(black_color.g, 0);
    EXPECT_EQ(black_color.b, 0);
    EXPECT_EQ(black_color.a, 255);
}

// Test conversion functions
TEST_F(color_test, to_rgb_conversion)
{
    tc::img::color c(100, 150, 200, 128);
    auto rgb_array = c.to_rgb();
    EXPECT_EQ(rgb_array.size(), 3);
    EXPECT_EQ(rgb_array[0], 100);
    EXPECT_EQ(rgb_array[1], 150);
    EXPECT_EQ(rgb_array[2], 200);
}

TEST_F(color_test, to_bgr_conversion)
{
    tc::img::color c(100, 150, 200, 128);
    auto bgr_array = c.to_bgr();
    EXPECT_EQ(bgr_array.size(), 3);
    EXPECT_EQ(bgr_array[0], 200); // b
    EXPECT_EQ(bgr_array[1], 150); // g
    EXPECT_EQ(bgr_array[2], 100); // r
}

TEST_F(color_test, to_rgba_conversion)
{
    tc::img::color c(100, 150, 200, 128);
    auto rgba_array = c.to_rgba();
    EXPECT_EQ(rgba_array.size(), 4);
    EXPECT_EQ(rgba_array[0], 100); // r
    EXPECT_EQ(rgba_array[1], 150); // g
    EXPECT_EQ(rgba_array[2], 200); // b
    EXPECT_EQ(rgba_array[3], 128); // a
}

TEST_F(color_test, to_bgra_conversion)
{
    tc::img::color c(100, 150, 200, 128);
    auto bgra_array = c.to_bgra();
    EXPECT_EQ(bgra_array.size(), 4);
    EXPECT_EQ(bgra_array[0], 200); // b
    EXPECT_EQ(bgra_array[1], 150); // g
    EXPECT_EQ(bgra_array[2], 100); // r
    EXPECT_EQ(bgra_array[3], 128); // a
}

// Test equality operators
TEST_F(color_test, equality_operator)
{
    tc::img::color c1(100, 150, 200, 128);
    tc::img::color c2(100, 150, 200, 128);
    tc::img::color c3(101, 150, 200, 128);

    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 == c3);
}

TEST_F(color_test, inequality_operator)
{
    tc::img::color c1(100, 150, 200, 128);
    tc::img::color c2(100, 150, 200, 128);
    tc::img::color c3(101, 150, 200, 128);

    EXPECT_FALSE(c1 != c2);
    EXPECT_TRUE(c1 != c3);
}

// Test boundary values
TEST_F(color_test, boundary_values)
{
    tc::img::color c_min(0, 0, 0, 0);
    EXPECT_EQ(c_min.r, 0);
    EXPECT_EQ(c_min.g, 0);
    EXPECT_EQ(c_min.b, 0);
    EXPECT_EQ(c_min.a, 0);

    tc::img::color c_max(255, 255, 255, 255);
    EXPECT_EQ(c_max.r, 255);
    EXPECT_EQ(c_max.g, 255);
    EXPECT_EQ(c_max.b, 255);
    EXPECT_EQ(c_max.a, 255);
}

// Test rgba/bgra conversions with boundary values
TEST_F(color_test, rgba_bgra_conversion_boundary_values)
{
    // Test minimum values
    tc::img::color c_min(0, 0, 0, 0);
    auto rgba_min = c_min.to_rgba();
    auto bgra_min = c_min.to_bgra();

    EXPECT_EQ(rgba_min[0], 0); // r
    EXPECT_EQ(rgba_min[1], 0); // g
    EXPECT_EQ(rgba_min[2], 0); // b
    EXPECT_EQ(rgba_min[3], 0); // a

    EXPECT_EQ(bgra_min[0], 0); // b
    EXPECT_EQ(bgra_min[1], 0); // g
    EXPECT_EQ(bgra_min[2], 0); // r
    EXPECT_EQ(bgra_min[3], 0); // a

    // Test maximum values
    tc::img::color c_max(255, 255, 255, 255);
    auto rgba_max = c_max.to_rgba();
    auto bgra_max = c_max.to_bgra();

    EXPECT_EQ(rgba_max[0], 255); // r
    EXPECT_EQ(rgba_max[1], 255); // g
    EXPECT_EQ(rgba_max[2], 255); // b
    EXPECT_EQ(rgba_max[3], 255); // a

    EXPECT_EQ(bgra_max[0], 255); // b
    EXPECT_EQ(bgra_max[1], 255); // g
    EXPECT_EQ(bgra_max[2], 255); // r
    EXPECT_EQ(bgra_max[3], 255); // a
}

// Test rgba/bgra conversions with static colors
TEST_F(color_test, rgba_bgra_conversion_static_colors)
{
    // Test red color
    auto red_color = tc::img::color::red();
    auto red_rgba = red_color.to_rgba();
    auto red_bgra = red_color.to_bgra();

    EXPECT_EQ(red_rgba[0], 255); // r
    EXPECT_EQ(red_rgba[1], 0);   // g
    EXPECT_EQ(red_rgba[2], 0);   // b
    EXPECT_EQ(red_rgba[3], 255); // a

    EXPECT_EQ(red_bgra[0], 0);   // b
    EXPECT_EQ(red_bgra[1], 0);   // g
    EXPECT_EQ(red_bgra[2], 255); // r
    EXPECT_EQ(red_bgra[3], 255); // a

    // Test green color
    auto green_color = tc::img::color::green();
    auto green_rgba = green_color.to_rgba();
    auto green_bgra = green_color.to_bgra();

    EXPECT_EQ(green_rgba[0], 0);   // r
    EXPECT_EQ(green_rgba[1], 255); // g
    EXPECT_EQ(green_rgba[2], 0);   // b
    EXPECT_EQ(green_rgba[3], 255); // a

    EXPECT_EQ(green_bgra[0], 0);   // b
    EXPECT_EQ(green_bgra[1], 255); // g
    EXPECT_EQ(green_bgra[2], 0);   // r
    EXPECT_EQ(green_bgra[3], 255); // a

    // Test blue color
    auto blue_color = tc::img::color::blue();
    auto blue_rgba = blue_color.to_rgba();
    auto blue_bgra = blue_color.to_bgra();

    EXPECT_EQ(blue_rgba[0], 0);   // r
    EXPECT_EQ(blue_rgba[1], 0);   // g
    EXPECT_EQ(blue_rgba[2], 255); // b
    EXPECT_EQ(blue_rgba[3], 255); // a

    EXPECT_EQ(blue_bgra[0], 255); // b
    EXPECT_EQ(blue_bgra[1], 0);   // g
    EXPECT_EQ(blue_bgra[2], 0);   // r
    EXPECT_EQ(blue_bgra[3], 255); // a
}

}
