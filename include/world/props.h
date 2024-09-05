#ifndef PROPS_H
#define PROPS_H
#define THRESHOLD 3.0f
#define TREES_COUNT 10
#define TREE_PATCH_SIZE 200.0f

#include <raylib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raymath.h>

typedef struct
{
    Vector3 position;
    float rotation;
    bool active;
}Trees;

extern Trees trees[TREES_COUNT];


Model MakeTree(Shader light);
void InitTress(Vector3 cameraPos);
void UpdateTrees(bool draw, Vector3 playerPos);
void DrawTrees(Model tree, Vector3 cameraPos);

#endif