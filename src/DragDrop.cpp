/*
 * Copyright (c) 2024 - 2026 ThorVG project. All rights reserved.

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
    unique_ptr<tvg::Animation> animation;
    uint32_t _w = 0, _h = 0;

    bool update(tvg::Canvas* canvas, uint32_t elapsed) override
    {
        if (!animation || animation->duration() <= 0) return false;

        auto progress = tvgexam::progress(elapsed, animation->duration());
        animation->frame(animation->totalFrame() * progress);
        canvas->update();

        return true;
    }

    bool content(tvg::Canvas* canvas, uint32_t w, uint32_t h) override
    {
        _w = w;
        _h = h;

        //The default font for fallback in case
        tvg::Text::load(EXAMPLE_DIR"/font/PublicSans-Regular.ttf");

        //Background
        auto shape = tvg::Shape::gen();
        shape->appendRect(0, 0, w, h);
        shape->fill(50, 50, 50);
        canvas->add(shape);

        return true;
    }

    bool drop(tvg::Canvas* canvas, const char* path, uint32_t w, uint32_t h) override
    {
        //remove previous picture
        if (animation) {
            canvas->remove(animation->picture());
            animation.reset();
        }

        _w = w;
        _h = h;

        auto len = strlen(path);
        if (len < 4) return false;

        const char* ext = path + len - 4;
        bool isLottie = (!strcmp(ext, "json") || !strcmp(ext, ".lot"));
        bool isSvg = !strcmp(ext, ".svg");
        bool isTvg = !strcmp(ext, ".tvg");

        if (!isLottie && !isSvg && !isTvg) {
            cout << "Unsupported format: " << path << endl;
            return false;
        }

        animation = unique_ptr<tvg::Animation>(tvg::Animation::gen());
        auto picture = animation->picture();
        picture->origin(0.5f, 0.5f);

        if (!tvgexam::verify(picture->load(path))) {
            animation.reset();
            return false;
        }

        //center the picture preserving its aspect ratio
        float pw, ph;
        picture->size(&pw, &ph);

        if (pw > 0 && ph > 0) {
            float scale = min(float(w) / pw, float(h) / ph) * 0.9f;
            picture->scale(scale);
            picture->translate(w * 0.5f, h * 0.5f);
        }

        canvas->add(animation->picture());

        //reset elapsed so animation starts from frame 0
        elapsed = 0;

        cout << "Loaded: " << path << endl;

        canvas->update();

        return true;
    }
};


/************************************************************************/
/* Entry Point                                                          */
/************************************************************************/

int main(int argc, char **argv)
{
    return tvgexam::main(new UserExample, argc, argv, false, 800, 800, 0, false);
}
