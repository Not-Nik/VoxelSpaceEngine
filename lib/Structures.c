// VoxelSpaceEngine (c) Nikolas Wipper 2020

#include <Structures.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

vsCamera *createCamera() {
    vsCamera *camera = (vsCamera *) malloc(sizeof(vsCamera));
    camera->position.x = 0;
    camera->position.y = 0;
    camera->position.z = 0;

    camera->rotation.y = 0.0f;
    camera->rotation.x = 100.0f;
    camera->distance = 1000;
    camera->fov = 90;
    return camera;
}

bool isWhole(float x) {
    return x == floorf(x);
}

vsMap *createMap(const char *colorMapPath, const char *heightMapPath) {
    vsMap *map = (vsMap *) malloc(sizeof(vsMap));

    Image height = LoadImage(heightMapPath);
    Image color = LoadImage(colorMapPath);

    if (!isWhole(log2f((float) color.width)) || !isWhole(log2f((float) color.height)) || !isWhole(log2f((float) height.width))
        || !isWhole(log2f((float) height.height))) {
        puts("Image dimensions must be powers of two");
        exit(1);
    }

    if (color.width != color.height || height.width != height.height || color.width != height.width) {
        puts("Image dimensions must be the same an both axis");
        exit(1);
    }

    map->width = color.width;
    map->height = color.height;
    map->shift = (int) log2f((float) color.width);
    map->altitude = LoadImageColors(height);
    map->color = LoadImageColors(color);
    return map;
}

vsScreenData *createScreenData(int w, int h) {
    vsScreenData *screenData = (vsScreenData *) malloc(sizeof(vsScreenData));
    screenData->width = w;
    screenData->height = h;
    screenData->backgroundColor = 0x6699ff90;
    screenData->screen = (Color *) malloc(sizeof(Color) * screenData->width * screenData->height);
    return screenData;
}

vsInput *createInput() {
    vsInput *input = (vsInput *) malloc(sizeof(vsInput));
    input->forwardBackward = 0;
    input->leftRight = 0;
    input->upDown = 0;
    input->lookUp = false;
    input->lookDown = false;
    input->mousePosition = NULL;
    input->keyPressed = false;
    return input;
}

vsGlobalGameState *createGlobalGameState(const char *colorMapPath, const char *heightMapPath, int w, int h) {
    vsGlobalGameState *globalGameState = (vsGlobalGameState *) malloc(sizeof(vsGlobalGameState));
    globalGameState->camera = createCamera();
    globalGameState->map = createMap(colorMapPath, heightMapPath);
    globalGameState->screenData = createScreenData(w, h);
    globalGameState->input = createInput();
    globalGameState->updateRunning = false;
    globalGameState->time = (float) 0;
    globalGameState->timeLastFrame = (float) 0;
    globalGameState->frames = 0;
    return globalGameState;
}
