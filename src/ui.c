#include "ui.h"
#include "dialog.h"

#include "raylib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int init_ui() {
    InitWindow(640, 480, app_name);

    return 0;
}

void update_ui() {
    // TODO
}

void draw_ui() {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText(app_name, 1, 1, 50, LIGHTGRAY);

    EndDrawing();
}

void loop_ui() {
    while (!WindowShouldClose()) {
        update_ui();
        draw_ui();
    }
}

void deinit_ui() {
    CloseWindow();
}
