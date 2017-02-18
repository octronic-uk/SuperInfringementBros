#include "vector.h"

int vector2iCollision(vector2i_t aPosition, vector2i_t aDimensions, vector2i_t bPosition, vector2i_t bDimensions) {
    vector2i_t aTopLeft, aTopRight, aBottomLeft, aBottomRight;
    vector2i_t bTopLeft, bTopRight, bBottomLeft, bBottomRight;

    aTopLeft.x = aPosition.x;
    aTopLeft.y = aPosition.y;

    aTopRight.x = aPosition.x + aDimensions.x;
    aTopRight.y = aPosition.y;

    aBottomLeft.x = aPosition.x;
    aBottomLeft.y = aPosition.y + aDimensions.y;

    aBottomRight.x = aPosition.x + aDimensions.x;
    aBottomRight.y = aPosition.y + aDimensions.y;

    bTopLeft.x = bPosition.x;
    bTopLeft.y = bPosition.y;

    bTopRight.x = bPosition.x + bDimensions.x;
    bTopRight.y = bPosition.y;

    bBottomLeft.x = bPosition.x;
    bBottomLeft.y = bPosition.y + bDimensions.y;

    bBottomRight.x = bPosition.x + bDimensions.x;
    bBottomRight.y = bPosition.y + bDimensions.y;

    // a top left inside b
    if (aTopLeft.x >= bTopLeft.x && aTopLeft.x <= bTopRight.x) {
        if (aTopLeft.y >= bTopLeft.y && aTopLeft.y <= bBottomRight.y) {
            return 1;
        }
    } 
    // b top right inside a
    if (aTopRight.x >= bTopLeft.x && aTopRight.x <= bTopRight.x) {
        if (aTopRight.y >= bTopLeft.y && aTopRight.y <= bBottomRight.y) {
            return 1;
        }
    }
    // b bottom left inside a
    if (aBottomLeft.x >= bTopLeft.x && aBottomLeft.x <= bTopRight.x) {
        if (aBottomLeft.y >= bTopLeft.y && aBottomLeft.y <= bBottomRight.y) {
            return 1;
        }
    }
    // b bottom right inside a
    if (aBottomRight.x >= bTopLeft.x && aBottomRight.x <= bTopRight.x) {
        if (aBottomRight.y >= bTopLeft.y && aBottomRight.y <= bBottomRight.y) {
            return 1;
        }
    }
    return 0;
}

