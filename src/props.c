#include "world/grass.h"
#include "world/props.h"
#include <raylib.h>

Model bone,tree;
Trees trees[TREES_COUNT];
bool skip= false;

Model MakeTree(Shader light)
{
    tree = LoadModel("assets/models/tree1.obj");
    tree.materials[0].shader = light;
    return tree;
}


void InitTress(Vector3 cameraPos)
{
    srand((unsigned int)time(NULL));
    int validTrees = 0;
    while(validTrees < TREES_COUNT)
    {
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float distance = GetRandomValue(0, (int)(TREE_PATCH_SIZE * 10)) / 10.0f;
        float x = cameraPos.x + distance*cosf(angle);
        float z = cameraPos.z + distance* sinf(angle);
        Vector3 position = {x,0,z};

        for(int i=0;i<OBJECT_COUNT; i++)
        {
            if(Vector3Distance((Vector3){excludePos[i].posX,0,excludePos[i].posZ},position)< 10)
                skip = true;
        }
        if(skip) continue;

        trees[validTrees].active = true;
        trees[validTrees].rotation = angle;

        trees[validTrees].position = position;
        validTrees++;
    }
}

void UpdateTrees(bool draw, Vector3 playerPos)
{
    if(draw)
    {
        for(int i=0; i<TREES_COUNT; i++)
        {
            if(trees[i].position.x < playerPos.x - TREE_PATCH_SIZE / 2)
            {
                trees[i].position.x += TREE_PATCH_SIZE;
            }
            else if(trees[i].position.x > playerPos.x + TREE_PATCH_SIZE / 2)
            {
                trees[i].position.x -= TREE_PATCH_SIZE;
            }

            if(trees[i].position.z < playerPos.z - TREE_PATCH_SIZE / 2)
            {
                trees[i].position.z += TREE_PATCH_SIZE;
            }
            else if(trees[i].position.z > playerPos.z + TREE_PATCH_SIZE / 2)
            {
                trees[i].position.z -= TREE_PATCH_SIZE;
            }
        }
    }
}


void DrawTrees(Model tree, Vector3 cameraPos)
{
    UpdateTrees(true,cameraPos);
    for(int i=0; i<TREES_COUNT; i++)
    {
        if(trees[i].active)
        {
            DrawModelEx(
                tree,
                trees[i].position,
                (Vector3){0,trees[i].rotation,0},
                0,
                (Vector3){4,4,4},
                WHITE
            );
        }
    }
}