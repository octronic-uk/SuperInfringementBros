#include "vector.h"

int vector2iCollision(vector2i_t aPosition, vector2i_t aDimensions, vector2i_t bPosition, vector2i_t bDimensions) {
   return (
       aPosition.x < bPosition.x + bDimensions.x &&
       aPosition.x + aDimensions.x > bPosition.x &&
       aPosition.y < bPosition.y + bDimensions.y &&
       aDimensions.y + aPosition.y > bPosition.y
   );
}
