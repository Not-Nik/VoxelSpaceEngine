// VoxelSpaceEngine (c) Nikolas Wipper 2020

#include <Renderer.h>
#include <math.h>

void drawVerticalLine(vsGlobalGameState * gameState, int x, int yTop, int yBottom, Color color)
{
    vsScreenData * screenData = gameState->screenData;
    for (int i = yTop; i < yBottom; i++)
    {
        screenData->screen[x + i * screenData->width] = color;
    }
}

void render(vsGlobalGameState * gameState)
{
    vsMap * map = gameState->map;
    vsCamera * camera = gameState->camera;
    vsScreenData * screenData = gameState->screenData;

    int scaleHeight = 250;

    int mapwidthperiod = map->width - 1;
    int mapheightperiod = map->height - 1;

    float sinang = sinf(camera->rotation.y);
    float cosang = cosf(camera->rotation.y);

    int hiddeny[screenData->width];
    for (int i = 0; i < screenData->width; i++) hiddeny[i] = screenData->height;

    float deltaZ = 1.0f;

    float z = 1.0f;
    Color background = GetColor(screenData->backgroundColor);

    // Create a temp vector of all heights on screen per line and interpolate
    // Maybe supersampling?

    int mapIndexes[screenData->width];

    while (z < camera->distance)
    {
        vsVector2f pLeft = {(-cosang * z - sinang * z) + camera->position.x, (sinang * z - cosang * z) + camera->position.y};
        vsVector2f pRight = {(cosang * z - sinang * z) + camera->position.x, (-sinang * z - cosang * z) + camera->position.y};

        float dX = (pRight.x - pLeft.x) / screenData->width;
        float dY = (pRight.y - pLeft.y) / screenData->width;

        float yFactor = z / camera->distance;
        float revertedYFactor = 1 - z / camera->distance;

        for (int i = 0; i < screenData->width; i++)
        {
            int mapIndex = (((int) pLeft.y & mapwidthperiod) << map->shift) +
                           ((int) pLeft.x & mapheightperiod) | 0;
            mapIndexes[i] = mapIndex;
            pLeft.x += dX;
            pLeft.y += dY;
        }

        for (int i = 0; i < screenData->width; i++)
        {
            int mapIndex = mapIndexes[i];
            int heightOnScreen = (camera->position.z - map->altitude[mapIndex].r) / z * scaleHeight + camera->rotation.x;
            Color color = map->color[mapIndex];
            int r = color.r * revertedYFactor + background.r * yFactor;
            r = r > 255 ? 255 : r;
            int g = color.g * revertedYFactor + background.g * yFactor;
            g = g > 255 ? 255 : g;
            int b = color.b * revertedYFactor + background.b * yFactor;
            b = b > 255 ? 255 : b;
            unsigned char cR = r;
            unsigned char cG = g;
            unsigned char cB = b;
            color = (Color) {cR, cG, cB, 255};
            drawVerticalLine(gameState, i, heightOnScreen, hiddeny[i], color);
            if (heightOnScreen < hiddeny[i]) hiddeny[i] = heightOnScreen;
        }
        z += deltaZ;
        deltaZ += 0.005f;
    }
}
