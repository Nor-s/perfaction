/*
 * Copyright (c) 2026 ThorVG project. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

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
    bool content(tvg::Canvas* canvas, uint32_t w, uint32_t h) override
    {
        auto background = tvg::Shape::gen();
        if (!tvgexam::verify(background->appendRect(0, 0, 100, 100))) return false;
        if (!tvgexam::verify(background->fill(0, 128, 255, 255))) return false;
        if (!tvgexam::verify(canvas->add(background))) return false;

        tvg::Fill::ColorStop cs[4] = {
        {0.1f, 0, 0, 0, 0},
        {0.2f, 50, 25, 50, 25},
        {0.5f, 100, 100, 100, 125},
        {0.9f, 255, 255, 255, 255}
        };

        auto radial = tvg::RadialGradient::gen();
        if (!tvgexam::verify(radial->colorStops(cs, 4))) return false;
        if (!tvgexam::verify(radial->spread(tvg::FillSpread::Pad))) return false;
        if (!tvgexam::verify(radial->radial(50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 0.0f))) return false;

        auto shape = tvg::Shape::gen();
        if (!tvgexam::verify(shape->appendRect(20, 20, 60, 60))) return false;
        if (!tvgexam::verify(shape->fill(radial))) return false;
        if (!tvgexam::verify(shape->blend(tvg::BlendMethod::Hue))) return false;
        if (!tvgexam::verify(canvas->add(shape))) return false;

        return true;
    }
};


/************************************************************************/
/* Entry Point                                                          */
/************************************************************************/

int main(int argc, char **argv)
{
    return tvgexam::main(new UserExample, argc, argv, true, 100, 100);
}
