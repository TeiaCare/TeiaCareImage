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

#pragma once

#include <teiacare/image/image_color.hpp>

#include <cstdint>
#include <vector>

namespace tc::img
{
/*!
 * \brief Set the color of a specific pixel in an RGB image.
 * \param img Image data vector to modify
 * \param width Width of the image in pixels
 * \param height Height of the image in pixels
 * \param x X coordinate of the pixel to set
 * \param y Y coordinate of the pixel to set
 * \param color Color to set for the pixel
 */
void set_pixel_rgb(
    std::vector<uint8_t>& img,
    int width,
    int height,
    int x,
    int y,
    const tc::img::color& color);

/*!
 * \brief Draw a rectangle on an image.
 * \param img Image data vector to draw on
 * \param width Width of the image in pixels
 * \param height Height of the image in pixels
 * \param x0 X coordinate of the rectangle's top-left corner
 * \param y0 Y coordinate of the rectangle's top-left corner
 * \param w Width of the rectangle
 * \param h Height of the rectangle
 * \param color Color to use for drawing the rectangle
 * \param thickness Thickness of the rectangle border in pixels, defaults to 1
 */
void draw_rectangle(
    std::vector<uint8_t>& img,
    int width,
    int height,
    int x0,
    int y0,
    int w,
    int h,
    const tc::img::color& color,
    int thickness = 1);

/*!
 * \brief Draw a line between two points on an image.
 * \param img Image data vector to draw on
 * \param width Width of the image in pixels
 * \param height Height of the image in pixels
 * \param x0 X coordinate of the line's start point
 * \param y0 Y coordinate of the line's start point
 * \param x1 X coordinate of the line's end point
 * \param y1 Y coordinate of the line's end point
 * \param color Color to use for drawing the line
 * \param thickness Thickness of the line in pixels, defaults to 1
 */
void draw_line(
    std::vector<uint8_t>& img,
    int width,
    int height,
    int x0,
    int y0,
    int x1,
    int y1,
    const tc::img::color& color,
    int thickness = 1);

/*!
 * \brief Draw a polygon defined by two points on an image.
 * \param img Image data vector to draw on
 * \param width Width of the image in pixels
 * \param height Height of the image in pixels
 * \param x0 X coordinate of the first point
 * \param y0 Y coordinate of the first point
 * \param x1 X coordinate of the second point
 * \param y1 Y coordinate of the second point
 * \param color Color to use for drawing the polygon
 * \param thickness Thickness of the polygon lines in pixels, defaults to 1
 */
void draw_polygon(
    std::vector<uint8_t>& img,
    int width,
    int height,
    int x0,
    int y0,
    int x1,
    int y1,
    const tc::img::color& color,
    int thickness = 1);

/*!
 * \brief Draw a polygon defined by multiple points on an image.
 * \param img Image data vector to draw on
 * \param width Width of the image in pixels
 * \param height Height of the image in pixels
 * \param points Vector of coordinate pairs defining the polygon vertices
 * \param color Color to use for drawing the polygon
 * \param thickness Thickness of the polygon lines in pixels, defaults to 1
 */
void draw_polygon(
    std::vector<uint8_t>& img,
    int width,
    int height,
    const std::vector<std::pair<int, int>>& points,
    const tc::img::color& color,
    int thickness = 1);
}
