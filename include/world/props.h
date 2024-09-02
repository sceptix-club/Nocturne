#ifndef PROPS_H
#define PROPS_H
#define THRESHOLD 3.0f
#include "grass.h"
#include <raylib.h>

Model bone;

Model Bone(Shader light)
{
    bone = LoadModel("assets/models/bone.obj");
    bone.materials[0].shader = light;
    return bone;
}

void DrawBone(bool draw,Vector3 position)
{
    if(draw)
    {
        for(int i=0 ;i<GRASSBLADE_COUNT; i++)
        {
            if(Vector3Distance(grassBlades[i].position,position) <= THRESHOLD)
            {   //disable the grass around certain distance around the model
                grassBlades[i].active = false;
            }
            else
            {
                grassBlades[i].active = true;
            }

        }
        DrawModel(bone, position, 3.0f, RAYWHITE);
    }
}

#endif