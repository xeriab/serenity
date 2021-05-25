/*
 * Copyright (c) $year, Xeriab Nabil <xeriab@tuta.io>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Random.h>
#include <AK/String.h>
#include <AK/Vector.h>
#include <LibCore/ArgsParser.h>
#include <LibGUI/Application.h>
#include <LibGUI/Event.h>
#include <LibGUI/Icon.h>
#include <LibGUI/Painter.h>
#include <LibGUI/Widget.h>
#include <LibGUI/Window.h>
#include <LibGfx/Bitmap.h>
#include <WindowServer/Screen.h>
#include <limits.h>
#include <stdint.h>
#include <random>

using Byte = u8;

namespace Util {

inline std::mt19937& rand_engine()
{
    static std::random_device seed_gen;
    static std::mt19937 engine(seed_gen());
    return engine;
}

inline auto rand() -> u32
{
    return AK::get_random<u32>();
}

inline auto randf() -> double
{
    return AK::get_random<u32>();
}

inline auto rand_char() -> char32_t
{
    auto r = Util::rand() % 80;

    if (r < 10)
        return U'0' + r;
    else
        r -= 10;

    if (r < 46)
        return U'ｰ' + r;
    else
        r -= 46;

    return U"<>*+.:=_|"[r % 9];
}

inline auto mod(int value, int modulo) -> int
{
    value %= modulo;
    if (value < 0)
        value += modulo;
    return value;
}

inline constexpr auto interpolate(double value, double a, double b) -> double
{
    return a + (b - a) * value;
}

} // namespace Util

class Matrix final : public GUI::Widget {
    C_OBJECT(Matrix)
public:
    virtual ~Matrix() override;

private:
    Matrix(int);

    auto draw() -> void;
    virtual auto paint_event(GUI::PaintEvent&) -> void override;
    // virtual auto timer_event(Core::TimerEvent&) -> void override;
    virtual auto keydown_event(GUI::KeyEvent&) -> void override;
    virtual auto mousedown_event(GUI::MouseEvent& event) -> void override;
    virtual auto mousemove_event(GUI::MouseEvent& event) -> void override;

    RefPtr<Gfx::Bitmap> m_bitmap;
    Gfx::IntPoint m_mouse_origin;
    int m_sweep_plane { 2000 };
};
