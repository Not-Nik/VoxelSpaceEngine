#include <string.h>

#include <raylib.h>
#include <voxelspace.h>
#include <math.h>

const static int width = 1080;
const static int height = 720;

int main() {
    vsGlobalGameState *gameState = createGlobalGameState("map/color.png", "map/height.png", width, height);
    vsScreenData *screenData = gameState->screenData;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(width, height, "VoxelSpaceEngine");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    Image i = (Image) {
        .width = screenData->width, .height = screenData->height, .data = screenData->screen, .format = UNCOMPRESSED_R8G8B8A8, .mipmaps = 1
    };
    Texture2D tex = LoadTextureFromImage(i);
    //--------------------------------------------------------------------------------------

    gameState->camera->position.z = 250;
    gameState->camera->position.y = 90;
    gameState->camera->position.x = 490;

    gameState->camera->rotation.x = 200;

    vsCamera *cam = gameState->camera;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        gameState->timeLastFrame = gameState->time;
        gameState->time = GetFrameTime();
        memset(screenData->screen, 0, screenData->width * screenData->height * sizeof(Color));

        // Update
        //---------------------------------------------------------------------------------

        if (IsKeyDown(KEY_LEFT)) cam->rotation.y += 0.7f * gameState->time;
        if (IsKeyDown(KEY_RIGHT))cam->rotation.y -= 0.7f * gameState->time;

        if (IsKeyDown(KEY_UP)) cam->rotation.x -= 100.f * gameState->time;
        if (IsKeyDown(KEY_DOWN)) cam->rotation.x += 100.f * gameState->time;
        if (IsKeyDown(KEY_SPACE)) cam->position.y += 20.f * gameState->time;
        if (IsKeyDown(KEY_LEFT_SHIFT)) cam->position.y -= 20.f * gameState->time;

        if (IsKeyDown(KEY_COMMA)) cam->fov += 10.f * gameState->time;
        if (IsKeyDown(KEY_PERIOD)) cam->fov -= 10.f * gameState->time;

        float forward = IsKeyDown(KEY_W);
        float backward = IsKeyDown(KEY_S);
        float left = IsKeyDown(KEY_A);
        float right = IsKeyDown(KEY_D);

        cam->position.x +=
            (sinf(cam->rotation.y) * backward - sinf(cam->rotation.y) * forward - cosf(cam->rotation.y) * left + cosf(cam->rotation.y) * right)
                * gameState->time * 20.f;
        cam->position.z +=
            (cosf(cam->rotation.y) * backward - cosf(cam->rotation.y) * forward + sinf(cam->rotation.y) * left - sinf(cam->rotation.y) * right)
                * gameState->time * 20.f;

        cam->position.x = fmodf(cam->position.x, (float) gameState->map->width);
        cam->position.z = fmodf(cam->position.z, (float) gameState->map->height);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GetColor(screenData->backgroundColor));
        render(gameState);
        UpdateTexture(tex, screenData->screen);
        DrawTexture(tex, 0, 0, WHITE);

        DrawFPS(0, 0);

        EndDrawing();

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
