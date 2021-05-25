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
    
    int frame_rate = 25;
    String message;
    String scene;
    String color = "default";
    bool diffuse = true;
    bool no_diffuse = false;

    Core::ArgsParser args_parser;

    args_parser.set_general_help("Matrix screensaver (Heavily inspired/stolen from cxxmatrix but converted to LibGUI instead of tty).");
    args_parser.add_option(frame_rate, "Set the frame rate per second. A positive number less than or equal to 1000. The default is 25.", "frame-rate", 'r', "frames");
    args_parser.add_option(message, "Add a message for 'banner' scene.", "message", 'm', "message");
    args_parser.add_option(scene, "Add scenes. Comma separated list of 'number', 'banner', 'rain','conway', 'mandelbrot', 'rain-forever' and 'loop'.", "scene", 's', "scene");
    args_parser.add_option(color, "Set color. One of 'default', 'black', 'red', 'green', 'yellow', 'blue', 'magenta', 'cyan', 'white', and integer 0-255 (256 index color).", "color", 'c', "color");
    args_parser.add_option(diffuse, "Turn on/off the background-color effect. Turned on by default.", "diffuse", '\0');
    args_parser.add_option(no_diffuse, "", "no-diffuse", '\0');
    
    args_parser.parse(argc, argv);
    
    if (frame_rate == 0)
        frame_rate = 25;

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

    auto& matrix = window->set_main_widget<Matrix>(frame_rate);
    matrix.set_fill_with_background_color(false);
    matrix.set_override_cursor(Gfx::StandardCursor::Hidden);
    matrix.update();
    window->show();

    // matrix.create_stars(window->width(), window->height(), star_count);
    // matrix.update();

    window->move_to_front();
    window->set_cursor(Gfx::StandardCursor::Hidden);
    window->update();

    return app->exec();
}