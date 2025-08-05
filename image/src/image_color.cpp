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

namespace tc::img
{
color::color()
    : r(0)
    , g(0)
    , b(0)
    , a(255)
{
}

color::color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)
    : r(red)
    , g(green)
    , b(blue)
    , a(alpha)
{
}

color color::black()
{
    return color(0, 0, 0);
}

color color::white()
{
    return color(255, 255, 255);
}

color color::red()
{
    return color(255, 0, 0);
}

color color::green()
{
    return color(0, 255, 0);
}

color color::blue()
{
    return color(0, 0, 255);
}

std::array<std::uint8_t, 3> color::to_rgb() const
{
    return std::array<std::uint8_t, 3>{r, g, b};
}

std::array<std::uint8_t, 4> color::to_rgba() const
{
    return std::array<std::uint8_t, 4>{r, g, b, a};
}

std::array<std::uint8_t, 3> color::to_bgr() const
{
    return std::array<std::uint8_t, 3>{b, g, r};
}

std::array<std::uint8_t, 4> color::to_bgra() const
{
    return std::array<std::uint8_t, 4>{b, g, r, a};
}

bool color::operator==(const color& other) const
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool color::operator!=(const color& other) const
{
    return !(*this == other);
}

}
