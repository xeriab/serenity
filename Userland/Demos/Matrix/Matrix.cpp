/*
 * Copyright (c) $year, Xeriab Nabil <xeriab@tuta.io>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Matrix.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>

Matrix::~Matrix()
{
}

Matrix::Matrix(int interval)
{
    srand(time(nullptr));
    stop_timer();
    start_timer(interval);
}


auto Matrix::draw() -> void
{
    GUI::Painter painter(*m_bitmap);
    painter.fill_rect(m_bitmap->rect(), Color::Black);
}

auto Matrix::paint_event(GUI::PaintEvent& event) -> void
{
    GUI::Painter painter(*this);
    painter.add_clip_rect(event.rect());
    painter.draw_scaled_bitmap(rect(), *m_bitmap, m_bitmap->rect());
}

// auto Matrix::timer_event(Core::TimerEvent&) -> void
// {
//     m_bitmap->fill(Color::Black);
//
//     // auto computed_point = Gfx::IntPoint();
//     // auto half_x = width() / 2;
//     // auto half_y = height() / 2;
//     //
//     // for (auto star : m_stars) {
//     //     auto z = ((star.z + m_sweep_plane) % 2000) * 0.0005;
//     //     computed_point.set_x(half_x + star.x / z);
//     //     computed_point.set_y(half_y + star.y / z);
//     //
//     //     if (computed_point.x() < 0 || computed_point.x() >= width() || computed_point.y() < 0 || computed_point.y() >= height())
//     //         continue;
//     //
//     //     u8 falloff = (1 - z * z) * 255;
//     //     m_bitmap->set_pixel(computed_point, Color(falloff, falloff, falloff));
//     // }
//    
//     m_sweep_plane -= 1;
//    
//     if (m_sweep_plane < 0)
//         m_sweep_plane = 2000;
//    
//     update();
// }

auto Matrix::mousemove_event(GUI::MouseEvent& event) -> void
{
    constexpr float max_distance_move = 10;

    if (m_mouse_origin.is_null()) {
        m_mouse_origin = event.position();
    } else if (event.position().distance_from(m_mouse_origin) > max_distance_move) {
        ::exit(0);
    }
}

auto Matrix::mousedown_event(GUI::MouseEvent&) -> void
{
    ::exit(0);
}

auto Matrix::keydown_event(GUI::KeyEvent&) -> void
{
    ::exit(0);
}