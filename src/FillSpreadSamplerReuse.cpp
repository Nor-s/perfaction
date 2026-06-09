/*
 * Copyright (c) 2026 ThorVG project. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Example.h"

/************************************************************************/
/* ThorVG Drawing Contents                                              */
/************************************************************************/

struct UserExample : tvgexam::Example
{
    uint32_t mode = 0;

    bool content(tvg::Canvas* canvas, uint32_t, uint32_t) override
    {
        return draw(canvas, tvg::FillSpread::Pad);
    }

    bool update(tvg::Canvas* canvas, uint32_t elapsed) override
    {
        auto next = (elapsed / 1000) % 3;
        if (next == mode) return false;

        mode = next;

        if (mode == 0) return draw(canvas, tvg::FillSpread::Pad);
        if (mode == 1) return draw(canvas, tvg::FillSpread::Reflect);
        return draw(canvas, tvg::FillSpread::Repeat);
    }

    bool draw(tvg::Canvas* canvas, tvg::FillSpread spread)
    {
        if (!tvgexam::verify(canvas->remove())) return false;

        auto bg = tvg::Shape::gen();
        bg->appendRect(0, 0, 800, 800);
        bg->fill(0, 0, 0);
        canvas->add(bg);

        auto shape = tvg::Shape::gen();
        shape->appendRect(80, 80, 640, 640);

        auto fill = tvg::RadialGradient::gen();
        fill->radial(400, 400, 60, 400, 400, 0);

        tvg::Fill::ColorStop colorStops[4];
        colorStops[0] = {0.0f, 127, 39, 255, 255};
        colorStops[1] = {0.33f, 159, 112, 253, 255};
        colorStops[2] = {0.66f, 253, 191, 96, 255};
        colorStops[3] = {1.0f, 255, 137, 17, 255};

        fill->colorStops(colorStops, 4);
        fill->spread(spread);
        shape->fill(fill);

        canvas->add(shape);
        return true;
    }
};


/************************************************************************/
/* Entry Point                                                          */
/************************************************************************/

int main(int argc, char **argv)
{
    return tvgexam::main(new UserExample, argc, argv, false, 800, 800);
}
