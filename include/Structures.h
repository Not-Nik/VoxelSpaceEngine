// VoxelSpaceEngine (c) Nikolas Wipper 2020

#ifndef VOXELSPACEENGINE_STRUCTURES_H
#define VOXELSPACEENGINE_STRUCTURES_H

#include <raylib.h>

typedef struct {
    float x, y;
} vsVector2f;

typedef struct {
    float x, y, z;
} vsVector3f;

typedef struct {
    vsVector3f position;
    vsVector2f rotation;
    float distance, fov;
} vsCamera;

typedef struct {
    int width, height, shift;
    const Color *altitude;
    const Color *color;
} vsMap;

typedef struct {
    int width, height;
    int backgroundColor;
    Color *screen;
} vsScreenData;

typedef struct {
    int forwardBackward, leftRight, upDown;
    bool lookUp, lookDown, keyPressed;
    void *mousePosition;
} vsInput;

typedef struct {
    vsCamera *camera;
    vsMap *map;
    vsScreenData *screenData;
    vsInput *input;
    bool updateRunning;
    float time, timeLastFrame;
    int frames;
} vsGlobalGameState;

vsCamera *createCamera();
vsMap *createMap(const char *colorMapPath, const char *heightMapPath);
vsScreenData *createScreenData(int w, int h);
vsInput *createInput();
vsGlobalGameState *createGlobalGameState(const char *colorMapPath, const char *heightMapPath, int w, int h);

#endif //VOXELSPACEENGINE_STRUCTURES_H
