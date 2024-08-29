#ifndef WRAP_H
#define WRAP_H

#include <raylib.h>

inline void CalculateMinMax(Vector3* cameraPosition, float* minX, float* maxX, float* minZ, float* maxZ, float MAX_OFFSET);
inline void WrapPosition(float *position, float min, float max);

#endif // WRAP_H