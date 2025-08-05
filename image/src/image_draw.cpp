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

#include <teiacare/image/image_draw.hpp>

#include <cmath>

namespace tc::img
{
void set_pixel_rgb(std::vector<uint8_t>& img, int width, int height, int x, int y, const tc::img::color& color)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    const int idx = (y * width + x) * 3;
    img[idx + 0] = color.r;
    img[idx + 1] = color.g;
    img[idx + 2] = color.b;
}

void draw_rectangle(std::vector<uint8_t>& img, int width, int height, int x0, int y0, int w, int h, const tc::img::color& color, int thickness)
{
    // Top and bottom horizontal lines
    for (int t = 0; t < thickness; ++t)
    {
        int y_top = y0 + t;
        int y_bottom = y0 + h - 1 - t;
        for (int x = x0; x < x0 + w; ++x)
        {
            set_pixel_rgb(img, width, height, x, y_top, color);
            set_pixel_rgb(img, width, height, x, y_bottom, color);
        }
    }

    // Left and right vertical lines
    for (int t = 0; t < thickness; ++t)
    {
        int x_left = x0 + t;
        int x_right = x0 + w - 1 - t;
        for (int y = y0; y < y0 + h; ++y)
        {
            set_pixel_rgb(img, width, height, x_left, y, color);
            set_pixel_rgb(img, width, height, x_right, y, color);
        }
    }
}

void draw_line(std::vector<uint8_t>& img, int width, int height, int x0, int y0, int x1, int y1, const tc::img::color& color, int thickness)
{
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true)
    {
        // Draw thick line by drawing pixels in a square around the main pixel
        int half_thickness = thickness / 2;
        for (int ty = -half_thickness; ty <= half_thickness; ++ty)
        {
            for (int tx = -half_thickness; tx <= half_thickness; ++tx)
            {
                set_pixel_rgb(img, width, height, x0 + tx, y0 + ty, color);
            }
        }

        if (x0 == x1 && y0 == y1)
            break;

        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_polygon(std::vector<uint8_t>& img, int width, int height, const std::vector<std::pair<int, int>>& points, const tc::img::color& color, int thickness)
{
    if (points.size() < 2)
        return;

    for (size_t i = 0; i < points.size(); ++i)
    {
        const auto& p0 = points[i];
        const auto& p1 = points[(i + 1) % points.size()];
        draw_line(img, width, height, p0.first, p0.second, p1.first, p1.second, color, thickness);
    }
}

}
