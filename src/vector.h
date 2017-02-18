#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} vector2f_t;

typedef struct {
    int x;
    int y;
} vector2i_t;

int vector2iCollision(vector2i_t aPosision, vector2i_t aDimensions, vector2i_t bPosision, vector2i_t bDimensions);

#endif // VECTOR_H
