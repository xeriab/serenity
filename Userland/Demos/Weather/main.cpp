/*
 * Copyright (c) $year, Xeriab Nabil <xeriab@tuta.io>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Random.h>
#include <AK/String.h>
#include <LibCore/ArgsParser.h>
#include "Weather.h"

auto main(int argc, char** argv) -> int
{
    if (pledge("stdio recvfd sendfd rpath unix", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    Core::ArgsParser args_parser;
    //
    // args_parser.set_general_help("Matrix screensaver (Heavily inspired/stolen from cxxmatrix but converted to LibGUI instead of tty).");
    // args_parser.add_option(frame_rate, "Set the frame rate per second. A positive number less than or equal to 1000. The default is 25.", "frame-rate", 'r', "frames");
    // args_parser.add_option(message, "Add a message for 'banner' scene.", "message", 'm', "message");
    // args_parser.add_option(scene, "Add scenes. Comma separated list of 'number', 'banner', 'rain','conway', 'mandelbrot', 'rain-forever' and 'loop'.", "scene", 's', "scene");
    // args_parser.add_option(color, "Set color. One of 'default', 'black', 'red', 'green', 'yellow', 'blue', 'magenta', 'cyan', 'white', and integer 0-255 (256 index color).", "color", 'c', "color");
    // args_parser.add_option(diffuse, "Turn on/off the background-color effect. Turned on by default.", "diffuse", '\0');
    // args_parser.add_option(no_diffuse, "", "no-diffuse", '\0');
    
    args_parser.parse(argc, argv);
    
    return 0;
}