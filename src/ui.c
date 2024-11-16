#include "ui.h"
#include "dialog.h"
#include "character.h"

#include "raylib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int ui_dialog = 0;

int init_ui() {
    InitWindow(WIDTH, HEIGHT, APP_NAME);

    return 0;
}

void update_ui() {
    // TODO
}

void draw_ui() {
    BeginDrawing();
    
    ClearBackground(BLACK);

    struct dialogLine line = getDialogLine(ui_dialog);
    Texture tex;
    if (get_texture(line.characterFrameID, &tex)) {
        DrawTexturePro(
            tex,
            (Rectangle){0, 0, (float)tex.width, (float)tex.height},
            (Rectangle){0, 0, WIDTH, HEIGHT},
            (Vector2){0, 0},
            0,
            WHITE
        );
    }

    DrawText(line.dialog, 1, 1, 50, LIGHTGRAY);

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
