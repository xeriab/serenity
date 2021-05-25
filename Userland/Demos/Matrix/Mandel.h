/*
 * Copyright (c) $year, Xeriab Nabil <xeriab@tuta.io>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Matrix.h"
#include <algorithm>
#include <cstdint>

struct Conway {
    int width = 128;
    int height = 128;
    Vector<i8> data1;
    Vector<i8> data2;

public:
    void init()
    {
        this->time = 1;
        data1.resize(width * height);
        data2.resize(width * height);
        std::generate(data1.begin(), data1.end(), []() { return Util::rand() & 1; });
    }

private:
    int index(int x, int y) const
    {
        x = Util::mod(x, 2 * width);
        if (x >= width)
            y = -1 - y, x -= width; // Klein boundary condition
        //if (x >= width) x -= width; // T2 boundary condition
        return Util::mod(y, height) * width + x;
    }
    i8 const& get1(int x, int y) const { return data1[index(x, y)]; }
    i8 const& get2(int x, int y) const { return data2[index(x, y)]; }
    i8& get1(int x, int y) { return data1[index(x, y)]; }
    i8& get2(int x, int y) { return data2[index(x, y)]; }

private:
    u32 time = 1;

    void create4x4()
    {
        double const prob = (width / 100.0) * (height / 100.0);
        if (Util::rand() % min(1, 100 / prob) == 0) {
            int const x0 = Util::rand() % width;
            int const y0 = Util::rand() % height;
            u32 value = Util::rand();
            for (int a = 0; a < 4; a++) {
                for (int b = 0; b < 4; b++) {
                    get1(x0 + a, y0 + b) = value & 1;
                    value >>= 1;
                }
            }
        }
    }

public:
    void step(double time)
    {
        if (time < this->time)
            return;
        this->time++;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int count = 0;
                if (get1(x + 1, y))
                    count++;
                if (get1(x - 1, y))
                    count++;
                if (get1(x, y + 1))
                    count++;
                if (get1(x, y - 1))
                    count++;
                if (get1(x + 1, y + 1))
                    count++;
                if (get1(x + 1, y - 1))
                    count++;
                if (get1(x - 1, y + 1))
                    count++;
                if (get1(x - 1, y - 1))
                    count++;
                get2(x, y) = count == 2 ? get1(x, y) : count == 3 ? 1
                                                                  : 0;
            }
        }
        data1.swap(data2);

        create4x4();
    }

private:
    int origin_x, origin_y;

public:
    void set_size(int cols, int rows)
    {
        origin_x = cols / 2;
        origin_y = rows / 2;
    }

private:
    static constexpr double xscale = 0.5 * 0.7;
    static constexpr double yscale = -1.0;
    double u_x, u_y, v_x, v_y;

public:
    void set_transform(double scale, double theta)
    {
        this->u_x = +scale * xscale * cos(theta);
        this->u_y = -scale * yscale * sin(theta);
        this->v_x = +scale * xscale * sin(theta);
        this->v_y = +scale * yscale * cos(theta);
    }

    int get_pixel(int x, int y, double power) const
    {
        double const u = 0.5 + u_x * (x - origin_x) + u_y * (y - origin_y);
        double const v = 0.5 + v_x * (x - origin_x) + v_y * (y - origin_y);
        if (get1(ceil(u), ceil(v)))
            return 1;

        if (power >= 0.4) {
            double const dx1A = 0.5, dy1A = +0.5;
            double const dx1B = 0.5, dy1B = -0.5;
            double const duA = dx1A * u_x + dy1A * u_y;
            double const dvA = dy1A * v_x + dx1A * v_y;
            double const duB = dx1B * u_x + dy1B * u_y;
            double const dvB = dy1B * v_x + dx1B * v_y;
            bool const sec = ceil(u + duA) != ceil(u - duA) || ceil(v + dvA) != ceil(v - dvA) || ceil(u + duB) != ceil(u - duB) || ceil(v + dvB) != ceil(v - dvB);
            if (sec)
                return 2;
        }

        return 0;
    }
};