#include "ui.h"
#include "dialog.h"

#include "raylib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int init_ui() {
    InitWindow(640, 480, "Dad Picker");

    return 0;
}

void loop_ui() {
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("INIT DAD AI", 1, 1, 50, LIGHTGRAY);

        EndDrawing();
    }
}

void deinit_ui() {
    CloseWindow();
}
