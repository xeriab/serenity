/*
 * Copyright (c) $year, Xeriab Nabil <xeriab@tuta.io>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Random.h>
#include <AK/String.h>
#include <LibCore/ArgsParser.h>
#include "Matrix.h"



auto main(int argc, char** argv) -> int
{
    if (pledge("stdio recvfd sendfd rpath unix", nullptr) < 0) {
        perror("pledge");
        return 1;
    }
    
    int refresh_rate = 16;

    Core::ArgsParser args_parser;
    args_parser.set_general_help("Matrix screensaver.");
    args_parser.add_option(refresh_rate, "Refresh rate (default = 16)", "rate", 'r', "milliseconds");
    
    args_parser.parse(argc, argv);
    
    if (refresh_rate == 0)
        refresh_rate = 16;

    auto app = GUI::Application::construct(argc, argv);

    if (pledge("stdio recvfd sendfd rpath", nullptr) < 0) {
        perror("pledge");
        return 1;
    }
    
    auto app_icon = GUI::Icon::default_icon("app-screensaver");
    auto window = GUI::Window::construct();

    window->set_double_buffering_enabled(true);
    window->set_title("Matrix");
    window->set_resizable(false);
    window->set_frameless(true);
    window->set_fullscreen(true);
    window->set_minimizable(false);
    window->set_icon(app_icon.bitmap_for_size(16));

    auto& matrix_screensaver = window->set_main_widget<Matrix::Matrix>(refresh_rate);
    matrix_screensaver.set_fill_with_background_color(false);
    matrix_screensaver.set_override_cursor(Gfx::StandardCursor::Hidden);
    matrix_screensaver.update();
    window->show();

    // matrix_screensaver.create_stars(window->width(), window->height(), star_count);
    // matrix_screensaver.update();

    window->move_to_front();
    window->set_cursor(Gfx::StandardCursor::Hidden);
    window->update();

    return app->exec();
}