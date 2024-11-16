#include "ui.h"
#include "dialog.h"
#include "character.h"
#include "button.h"

#include "raylib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int ui_dialog = 0;
static State state = STATE_MENU;
static Button play_button;

int init_ui() {
    InitWindow(WIDTH, HEIGHT, APP_NAME);

    play_button = init_button(
        (Rectangle) {
            (WIDTH - 300) / 2,
            150,
            300,
            75,
        },
        "Play",
        &ui_play
    );

    return 0;
}

void update_ui() {
    const Vector2 mouse_pos = GetMousePosition();
    const int click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    switch (state) {
        case STATE_MENU:
            update_button(&play_button, mouse_pos, click);

            break;
        case STATE_GAME:
            break;
    }
}

void draw_ui() {
    BeginDrawing();
    
    ClearBackground(BLACK);

    switch (state) {
        case STATE_MENU:
            int w = MeasureText(APP_NAME, 50);
            
            DrawText(
                APP_NAME,
                (WIDTH - w) / 2,
                50,
                50,
                WHITE
            );

            draw_button(&play_button);

            break;
        case STATE_GAME:
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
            break;
    }

    EndDrawing();
}

void loop_ui() {
    while (!WindowShouldClose()) {
        update_ui();
        draw_ui();
    }
}

void ui_play() {
    if (state == STATE_MENU)
        state = STATE_GAME;
}

void deinit_ui() {
    CloseWindow();
}
