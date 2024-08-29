#ifndef PROPS_H
#define PROPS_H
#include <raylib.h>

Model Tree(Shader light)
{
    Model rubble = LoadModel("assets/tree1.obj");
    rubble.materials[0].shader = light;
    return rubble;
}

#endif