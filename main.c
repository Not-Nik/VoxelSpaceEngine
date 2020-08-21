#include <string.h>

#include <raylib.h>
#include <voxelspace.h>

const static int width = 1080;
const static int height = 720;

int main()
{
    vsGlobalGameState * gameState = createGlobalGameState("map/color.png", "map/height.png", width, height);
    vsScreenData * screenData = gameState->screenData;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "VoxelSpaceEngine");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    Texture2D tex = LoadTextureFromImage(LoadImageEx(screenData->screen, screenData->width, screenData->height));
    //--------------------------------------------------------------------------------------

    gameState->camera->position.z = 75;
    gameState->camera->position.y = 250;
    gameState->camera->position.x = 475;

    gameState->camera->rotation.x = 200;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        gameState->timeLastFrame = gameState->time;
        gameState->time = GetFrameTime();
        memset(screenData->screen, 0, screenData->width * screenData->height * sizeof(Color));

        // Update
        //----------------------------------------------------------------------------------
        gameState->camera->rotation.y += 0.6f * (float)gameState->time;
        gameState->camera->position.y -= 100 * (float)gameState->time;

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GetColor(screenData->backgroundColor));
        render(gameState);
        UpdateTexture(tex, screenData->screen);
        DrawTexture(tex, 0, 0, WHITE);
        DrawFPS(0,0);

        EndDrawing();

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
