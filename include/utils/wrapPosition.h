#ifndef WRAP_H
#define WRAP_H

#include <raylib.h>

void CalculateMinMax(Vector3* cameraPosition, float* minX, float* maxX, float* minZ, float* maxZ, float MAX_OFFSET);
void WrapPosition(float *position, float min, float max);

#endif // WRAP_H