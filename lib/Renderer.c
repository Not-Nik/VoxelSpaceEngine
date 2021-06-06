// VoxelSpaceEngine (c) Nikolas Wipper 2020

#include <Renderer.h>
#include <math.h>

#include <raylib.h>

void drawVerticalLine(vsGlobalGameState *gameState, int x, int yTop, int yBottom, Color color) {
    vsScreenData *screenData = gameState->screenData;
    for (int i = yTop; i < yBottom; i++) {
        screenData->screen[x + i * screenData->width] = color;
    }
}

void render(vsGlobalGameState *gameState) {
    vsMap *map = gameState->map;
    vsCamera *cam = gameState->camera;
    vsScreenData *screenData = gameState->screenData;

    const int mapWidthPeriod = map->width - 1;
    const int mapHeightPeriod = map->height - 1;

    float sinang = sinf(cam->rotation.y);
    float cosang = cosf(cam->rotation.y);

    int hiddenY[screenData->width];
    for (int i = 0; i < screenData->width; i++) hiddenY[i] = screenData->height;

    float deltaZ = 0.1f; // Initial LOD
    float z = 1.0f; // Near plane

    // Create a temp vector of all heights on screen per line and do linear interpolation
    // Maybe supersampling?

    float fovFactor = (cam->fov / 90);

    while (z < (float) cam->distance * fovFactor) {
        // Left
        float plx = -cosang * z - sinang * z / fovFactor;
        float ply = sinang * z - cosang * z / fovFactor;
        // Right
        float prx = cosang * z - sinang * z / fovFactor;
        float pry = -sinang * z - cosang * z / fovFactor;

        float dx = (prx - plx) / (float) screenData->width;
        float dy = (pry - ply) / (float) screenData->width;
        plx += cam->position.x;
        ply += cam->position.z;

        float invz = 1.f / z * 350.f; // Vertical stretch

        for (int i = 0; i < screenData->width; i++) {
            int mapOffset = (((int) ply & mapWidthPeriod) << map->shift) + ((int) plx & mapHeightPeriod);
            float heightOnScreen = (cam->position.y - (float) map->altitude[mapOffset].r) * invz + cam->rotation.x;

            if (mapOffset > map->width * map->height || mapOffset < 0) continue;
            if ((int) heightOnScreen > screenData->height) heightOnScreen = (float) screenData->height;
            if (heightOnScreen < 0) heightOnScreen = 0;

            if ((int) heightOnScreen < hiddenY[i]) {
                drawVerticalLine(gameState, i, (int) heightOnScreen, hiddenY[i], map->color[mapOffset]);
                hiddenY[i] = (int) heightOnScreen;
            }
            plx += dx;
            ply += dy;
        }

        z += deltaZ * fovFactor;
        deltaZ += 0.00025f; // LOD decrease
    }
}
