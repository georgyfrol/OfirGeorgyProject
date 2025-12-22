#include "Torch.h"

bool Torch::isLit(int targetX, int targetY, int sourceX, int sourceY) {
    int dx = targetX - sourceX;
    int dy = targetY - sourceY;
    return (dx * dx + dy * dy) <= LIGHT_RADIUS_SQ;
}