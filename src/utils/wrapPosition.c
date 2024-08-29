#include "utils/wrapPosition.h"

void CalculateMinMax(Vector3* cameraPosition, float* minX, float* maxX, float* minZ, float* maxZ, float MAX_OFFSET)
{
    *minX = cameraPosition->x - MAX_OFFSET;
    *maxX = cameraPosition->x + MAX_OFFSET;
    *minZ = cameraPosition->z - MAX_OFFSET;
    *maxZ = cameraPosition->z + MAX_OFFSET;
}

void WrapPosition(float *position, float min, float max)
{
    if (*position > max) {
        *position = min;
    } else if (*position < min) {
        *position = max;
    }
}