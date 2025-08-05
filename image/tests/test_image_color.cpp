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

namespace tc::img::test
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

}
