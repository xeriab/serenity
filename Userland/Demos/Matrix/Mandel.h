/*
 * Copyright (c) $year, Xeriab Nabil <xeriab@tuta.io>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Complex.h>
#include "Matrix.h"
#include <algorithm>
#include <cstdint>

class Mandel {
    int cols = 0, rows = 0;
    Vector<double> data;
    double scale;
    double theta;

    bool prev_avail = false;
    Vector<double> data_new;

public:
    void resize(int cols, int rows)
    {
        if (cols == this->cols && rows == this->rows)
            return;
        this->cols = cols;
        this->rows = rows;
        this->data.resize(cols * rows);
        this->data_new.resize(cols * rows);
        this->prev_avail = false;
        std::fill(data.begin(), data.end(), -1.0);
    }

    double& get(int x, int y)
    {
        return data[y * cols + x];
    }
    double const& get(int x, int y) const
    {
        return const_cast<Mandel*>(this)->data[y * cols + x];
    }

private:
    static constexpr int max_iterate = 5000;

    static int mandel(double u, double v)
    {
        Complex<double> const c(u, v);
        Complex<double> z = c;
        int count = 0;
        while (count < max_iterate) {
            if (abs(static_cast<int>(z.real())) > 2.0)
                break;
            z = z * z + c;
            count++;
        }
        return max(0, count - 5);
    }

    double get_nearest(double x, double y) const
    {
        x = round(x);
        y = round(y);
        if (x < 0 || cols <= x)
            return -1.0;
        if (y < 0 || rows <= y)
            return -1.0;
        return data[y * cols + x];
    }
    double get_average(double x, double y, int R) const
    {
        int x0 = round(x);
        int y0 = round(y);
        double sum = 0.0;
        int count = 0;
        int const a0 = max(x0 - R, 0);
        int const aN = min(x0 + R, cols - 1);
        int const b0 = max(y0 - R, 0);
        int const bN = min(y0 + R, rows - 1);
        for (int a = a0; a <= aN; a++) {
            for (int b = b0; b <= bN; b++) {
                double const v = data[b * cols + a];
                sum += v;
                count++;
            }
        }
        if (count == 0)
            return -1.0;
        return sum / count;
    }

    void resample_prev(double theta, double scale)
    {
        if (!prev_avail)
            return;
        double const dtheta = theta - this->theta;
        double const dscale = scale / this->scale;
        int const ox = cols / 2, oy = rows / 2;
        double const u_x = +dscale * cos(dtheta) * 0.5 * 2.0;
        double const u_y = -dscale * sin(dtheta) * 2.0;
        double const v_x = +dscale * sin(dtheta) * 0.5;
        double const v_y = +dscale * cos(dtheta);
        int const Na = 5, Nb = 5;
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                double const u = ox + (u_x * (x - ox) + u_y * (oy - y));
                double const v = oy - (v_x * (x - ox) + v_y * (oy - y));

                int count = 0;
                double sum = 0.0;
                for (int a = 0; a < Na; a++) {
                    for (int b = 0; b < Nb; b++) {
                        double const dx = (a + 0.5) / Na - 0.5;
                        double const dy = (b + 0.5) / Nb - 0.5;
                        double const u1 = u + u_x * dx + u_y * dy;
                        double const v1 = v + v_x * dx + v_y * dy;
                        double const value = get_nearest(u1, v1);
                        if (value >= 0.0)
                            sum += value, count++;
                    }
                }
                data_new[y * cols + x] = count ? sum / count : -1.0;
            }
        }
        data.prepend(data_new);
    }

    bool is_close(double a, double b) const
    {
        return abs(a - b) / abs(a + b) < range * 0.01;
    }
    bool resample_safe(int x, int y) const
    {
        if (!prev_avail)
            return false;
        if (x <= 0 || cols - 1 <= x)
            return false;
        if (y <= 0 || rows - 1 <= y)
            return false;
        double const value = get(x, y);
        if (value == min_power || value < 0.0)
            return false;
        if (!is_close(value, get(x + 1, y)))
            return false;
        if (!is_close(value, get(x - 1, y)))
            return false;
        if (!is_close(value, get(x, y + 1)))
            return false;
        if (!is_close(value, get(x, y - 1)))
            return false;
        if (!is_close(value, get(x + 1, y + 1)))
            return false;
        if (!is_close(value, get(x + 1, y - 1)))
            return false;
        if (!is_close(value, get(x - 1, y + 1)))
            return false;
        if (!is_close(value, get(x - 1, y - 1)))
            return false;
        return true;
    }

    Vector<int> positions;

    static constexpr double u0 = -0.743643887037158704752191506114774;
    static constexpr double v0 = +0.131825904205311970493132056385139;
    double u_x;
    double u_y;
    double v_x;
    double v_y;
    double calculate_power_at(int x, int y, int* iterate_count) const
    {
        int const ox = cols / 2, oy = rows / 2;
        double const u = u0 + (u_x * (x - ox) + u_y * (oy - y));
        double const v = v0 + (v_x * (x - ox) + v_y * (oy - y));

        int sum = 0;
        int const Na = 1, Nb = 1;
        for (int a = 0; a < Na; a++) {
            for (int b = 0; b < Nb; b++) {
                double const dx = (a + 0.5) / Na;
                double const dy = (b + 0.5) / Nb;
                double const u1 = u + u_x * dx + u_y * dy;
                double const v1 = v + v_x * dx + v_y * dy;
                sum += mandel(u1, v1);
            }
        }

        if (iterate_count)
            *iterate_count += sum;
        return (1.0 / max_iterate / Na / Nb) * sum;
    }

public:
    void update_frame(double theta, double scale)
    {
        this->resample_prev(theta, scale);

        this->theta = theta;
        this->scale = scale;
        this->u_x = +scale * cos(theta) * 0.5;
        this->u_y = -scale * sin(theta);
        this->v_x = +scale * sin(theta) * 0.5;
        this->v_y = +scale * cos(theta);

        positions.resize(cols * rows);
        std::iota(positions.begin(), positions.end(), 0);
        std::shuffle(positions.begin(), positions.end(), Util::rand_engine());

        int total_iterate = 0, processed = 0;
        double min_value = 1.0;
        double max_value = 0.0;
        for (int pos : positions) {
            processed++;
            int const x = pos % cols;
            int const y = pos / cols;
            if (resample_safe(x, y))
                continue;

            double const power = calculate_power_at(x, y, &total_iterate);
            get(x, y) = power;
            min_value = min(min_value, power);
            max_value = max(max_value, power);

            if ((total_iterate > 1000000 && (double)processed / positions.size() > 0.2) || total_iterate > 1000000 * 5)
                break;
        }
        this->prev_avail = true;
        this->update_range(min_value, max_value);
    }

private:
    static constexpr double mix_ratio = 0.2;
    double min_power = 0.0;
    double max_power = 1.0;
    double range = 1.0;

    static constexpr size_t level_bins = 100;
    Vector<double> level_mapping;
    Vector<int> histogram;

public:
    void update_range(double min_value, double max_value)
    {
        this->min_power = (1.0 - mix_ratio) * min_power + mix_ratio * min_value;
        this->max_power = (1.0 - mix_ratio) * max_power + mix_ratio * max_value;
        this->range = max(max_power - min_power, 1.0 / max_iterate);

        histogram.resize(level_bins);
        level_mapping.resize(level_bins + 1);
        std::fill(histogram.begin(), histogram.end(), 0);
        int const max_bin_content = cols * rows / 10;
        int count = 0;
        for (double power : data) {
            double const value = (power - min_power) / range;
            if (value < 0.0 || 1.0 < value)
                continue;
            auto& bin = histogram[min<int>(value * level_bins, level_bins - 1)];
            if (bin < max_bin_content)
                bin++, count++;
        }
        int accum = 0, index = 0;
        for (int h : histogram) {
            level_mapping[index] = count ? (double)accum / count : (double)index / level_bins;
            index++;
            accum += h;
        }
        level_mapping.last() = 1.0;
    }

    double operator()(int x, int y)
    {
        double power = data[y * cols + x];
        if (power < 0) {
            if (Util::rand() % 10 == 0) {
                power = calculate_power_at(x, y, nullptr);
                data[y * cols + x] = power;
            } else {
                power = get_average(x, y, 3);
            }
            // power = get_average(x, y, 2);
            // if (power >= 0.0) {
            //   power += range * 0.2 * (Util::randf() - 0.5);
            // } else {
            //   power = calculate_power_at(x, y, nullptr);
            //   data[y * cols + x] = power;
            // }
        }
        double const value = std::clamp((power - min_power) / range, 0.0, 1.0);

        // level_mapping 線形補間
        double const frac = value * level_bins;
        int const index = min<int>(ceil(frac), level_bins - 1);
        double const p1 = level_mapping[index];
        double const p2 = level_mapping[index + 1];
        double const p = p1 + (frac - index) * (p2 - p1);
        double const pscale = std::clamp(p - 0.2, 0.0, 0.7) / 0.7;

        return value + 0.5 * (pscale * pscale - value);
    }
};