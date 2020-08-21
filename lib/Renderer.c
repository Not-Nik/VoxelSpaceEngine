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

int ifloorf(float f)
{
    return (int)f;
}

unsigned char ucclampf(float i)
{
    return i > 255 ? 255 : ifloorf(i) < 0 ? 0 : ifloorf(i);
}

void render(vsGlobalGameState * gameState)
{
    vsMap * map = gameState->map;
    vsCamera * camera = gameState->camera;
    vsScreenData * screenData = gameState->screenData;

    int scaleHeight = 250;

    float sinang = sinf(camera->rotation.y);
    float cosang = cosf(camera->rotation.y);

    int hiddeny[screenData->width];
    for (int i = 0; i < screenData->width; i++) hiddeny[i] = screenData->height;

    float deltaZ = 1.0f;
    float z = 1.0f;

    Color background = GetColor(screenData->backgroundColor);

    // Create a temp vector of all heights on screen per line and interpolate
    // Maybe supersampling?

    while (z < (float)camera->distance)
    {
        float saNew = sinang * z;
        float caNew = cosang * z;

        vsVector2f pLeft = {(-caNew - saNew) + camera->position.x,  (saNew - caNew) + camera->position.y};
        vsVector2f pRight = {(caNew - saNew) + camera->position.x, (-saNew - caNew) + camera->position.y};

        float dX = (pRight.x - pLeft.x) / (float)screenData->width;
        float dY = (pRight.y - pLeft.y) / (float)screenData->width;

        float yFactor = z / (float)camera->distance;
        float revertedYFactor = 1 - z / (float)camera->distance;

        for (int i = 0; i < screenData->width; i++)
        {
            int mapIndex = ((ifloorf(pLeft.y) & map->width - 1) << map->shift) +
                            (ifloorf(pLeft.x) & map->height - 1) | 0;
            pLeft.x += dX;
            pLeft.y += dY;

            int heightOnScreen = ifloorf((camera->position.z - (float)map->altitude[mapIndex].r) / z * (float)scaleHeight + camera->rotation.x);
            Color color = map->color[mapIndex];
            color = (Color) {
                    ucclampf((float)color.r * revertedYFactor + (float)background.r * yFactor),
                    ucclampf((float)color.g * revertedYFactor + (float)background.g * yFactor),
                    ucclampf((float)color.b * revertedYFactor + (float)background.b * yFactor),
                    255};
            drawVerticalLine(gameState, i, heightOnScreen, hiddeny[i], color);
            if (heightOnScreen < hiddeny[i]) hiddeny[i] = heightOnScreen;
        }

        z += deltaZ;
        deltaZ += 0.005f;
    }
}
