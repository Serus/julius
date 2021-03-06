#ifndef WIDGET_TEXT_H
#define WIDGET_TEXT_H

#include "widget.h"

namespace Widget
{

struct Text
{
    static void captureCursor();
    static void drawCursor(int xOffset, int yOffset);
    static int draw(const uint8_t *str, int x, int y, font_t font, color_t color);
    static void drawCentered(const uint8_t *str, int x, int y, int boxWidth, font_t font, color_t color);
    static int drawNumber(int value, char prefix, const char *postfix, int xOffset, int yOffset, font_t font);
    static int drawNumberColored(int value, char prefix, const char *postfix, int xOffset, int yOffset, font_t font, color_t color);
    static int drawMoney(int value, int xOffset, int yOffset, font_t font);
    static int drawPercentage(int value, int xOffset, int yOffset, font_t font);
    static void drawNumberCentered(int value, int xOffset, int yOffset, int boxWidth, font_t font);
    static void drawNumberCenteredColored(int value, int xOffset, int yOffset, int boxWidth, font_t font, color_t color);
};

}

#endif // WIDGET_TEXT_H
