// VoxelSpaceEngine (c) Nikolas Wipper 2020

#include <Structures.h>

#include <stdlib.h>
#include <stdio.h>

vsCamera * createCamera()
{
    vsCamera * camera = (vsCamera *)malloc(sizeof(vsCamera));
    camera->position.x = 512;
    camera->position.y = 800;
    camera->position.z = 200;

    camera->rotation.y = 0.0f;
    camera->rotation.x = 100.0f;
    camera->distance = 1000;
    return camera;
}

vsMap * createMap(const char * colorMapPath, const char * heightMapPath)
{
    vsMap * map = (vsMap *) malloc(sizeof(vsMap));
    map->width = 1024;
    map->height = 1024;
    map->shift = 10;
    Image height = LoadImage(heightMapPath);
    Image color = LoadImage(colorMapPath);
    if (height.width != 1024 || height.height != 1024 ||
        color.width != 1024 || color.height != 1024)
    {
        puts("Image size not 1024 * 1024!");
        exit(-1);
    }
    map->altitude = GetImageData(height);
    map->color = GetImageData(color);
    return map;
}

vsScreenData * createScreenData(int w, int h)
{
    vsScreenData * screenData = (vsScreenData *) malloc(sizeof(vsScreenData));
    screenData->width = w;
    screenData->height = h;
    screenData->backgroundColor = 0x6699ff90;
    screenData->screen = (Color *)malloc(sizeof(Color) * screenData->width * screenData->height);
    return screenData;
}

vsInput * createInput()
{
    vsInput * input = (vsInput *) malloc(sizeof(vsInput));
    input->forwardBackward = 0;
    input->leftRight = 0;
    input->upDown = 0;
    input->lookUp = false;
    input->lookDown = false;
    input->mousePosition = NULL;
    input->keyPressed = false;
    return input;
}

vsGlobalGameState * createGlobalGameState(const char * colorMapPath, const char * heightMapPath, int w, int h)
{
    vsGlobalGameState * globalGameState = (vsGlobalGameState *) malloc(sizeof(vsGlobalGameState));
    globalGameState->camera = createCamera();
    globalGameState->map = createMap(colorMapPath, heightMapPath);
    globalGameState->screenData = createScreenData(w, h);
    globalGameState->input = createInput();
    globalGameState->updateRunning = false;
    globalGameState->time = GetTime();
    globalGameState->timeLastFrame = GetTime();
    globalGameState->frames = 0;
    return globalGameState;
}
