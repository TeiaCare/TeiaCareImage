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

#include <array>
#include <cstdint>

namespace tc::img
{
/*!
 * \class color
 * \brief Represents a color with red, green, blue, and alpha (transparency) components.
 *
 * This class provides functionality to create, manipulate, and convert colors between different formats.
 * Each color component is represented as an 8-bit unsigned integer (0-255).
 */
struct color
{
    /*!
     * \brief Default constructor creating a black color with full opacity.
     */
    explicit color();

    /*!
     * \brief Constructor creating a color with specified RGBA components.
     * \param red Red component value (0-255)
     * \param green Green component value (0-255)
     * \param blue Blue component value (0-255)
     * \param alpha Alpha (transparency) component value (0-255), defaults to 255 (fully opaque)
     */
    explicit color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255);

    /*!
     * \brief Create a black color (0, 0, 0, 255).
     * \return Black color instance
     */
    static color black();

    /*!
     * \brief Create a white color (255, 255, 255, 255).
     * \return White color instance
     */
    static color white();

    /*!
     * \brief Create a red color (255, 0, 0, 255).
     * \return Red color instance
     */
    static color red();

    /*!
     * \brief Create a green color (0, 255, 0, 255).
     * \return Green color instance
     */
    static color green();

    /*!
     * \brief Create a blue color (0, 0, 255, 255).
     * \return Blue color instance
     */
    static color blue();

    /*!
     * \brief Convert color to RGB format (3 components).
     * \return Array containing RGB values [red, green, blue]
     */
    std::array<std::uint8_t, 3> to_rgb() const;

    /*!
     * \brief Convert color to RGBA format (4 components).
     * \return Array containing RGBA values [red, green, blue, alpha]
     */
    std::array<std::uint8_t, 4> to_rgba() const;

    /*!
     * \brief Convert color to BGR format (3 components, blue-green-red order).
     * \return Array containing BGR values [blue, green, red]
     */
    std::array<std::uint8_t, 3> to_bgr() const;

    /*!
     * \brief Convert color to BGRA format (4 components, blue-green-red-alpha order).
     * \return Array containing BGRA values [blue, green, red, alpha]
     */
    std::array<std::uint8_t, 4> to_bgra() const;

    /*!
     * \brief Check if two colors are equal.
     * \param other The color to compare with
     * \return True if colors have identical RGBA components, false otherwise
     */
    bool operator==(const color& other) const;

    /*!
     * \brief Check if two colors are not equal.
     * \param other The color to compare with
     * \return True if colors have different RGBA components, false otherwise
     */
    bool operator!=(const color& other) const;

    std::uint8_t r, g, b, a;
};

}
