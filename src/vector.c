#include "vector.h"

int vector2iCollision(vector2i_t aPosition, vector2i_t aDimensions, vector2i_t bPosition, vector2i_t bDimensions) {
   return (
       aPosition.x < bPosition.x + bDimensions.x &&
       aPosition.x + aDimensions.x > bPosition.x &&
       aPosition.y < bPosition.y + bDimensions.y &&
       aDimensions.y + aPosition.y > bPosition.y
   );
}


int vector2iScaledCollision(vector2i_t aPosition, vector2i_t aDimensions, vector2i_t bPosition, vector2i_t bDimensions,float scaleX, float scaleY) {
    aDimensions.x = aDimensions.x*scaleX;
    aDimensions.y = aDimensions.y*scaleY;

    aPosition.x += aDimensions.x/2;
    aPosition.y += aDimensions.y/2;

    bDimensions.x = bDimensions.x*scaleX;
    bDimensions.y = bDimensions.y*scaleY;

    bPosition.x += bDimensions.x/2;
    bPosition.y += bDimensions.y/2;

   return (
       aPosition.x < bPosition.x + bDimensions.x &&
       aPosition.x + aDimensions.x > bPosition.x &&
       aPosition.y < bPosition.y + bDimensions.y &&
       aDimensions.y + aPosition.y > bPosition.y
   );
}
