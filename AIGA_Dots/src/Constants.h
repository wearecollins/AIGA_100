#pragma once

enum Mode {
    MODE_COLOR = 0,
    MODE_INTERACTIVE_COLOR,
    MODE_INTERACTIVE_SWIPE,
    MODE_INTERACTIVE_GRID,
    MODE_INTERACTIVE_TEXT,
    MODE_VIDEO,
    MODE_DATA
};


static const int spacing = 36;
static const int dotWidth = 24;
static const int gridSize = 10;
const float gridWidth = spacing * gridSize;