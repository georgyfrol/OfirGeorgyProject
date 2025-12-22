#pragma once

class Torch {
public:
    static const char SYMBOL = '!';
    static const int LIGHT_RADIUS = 7;
    static const int LIGHT_RADIUS_SQ = LIGHT_RADIUS * LIGHT_RADIUS;
    static bool isLit(int targetX, int targetY, int sourceX, int sourceY);
};