#include "ui.h"
#include "dialog.h"
#include "character.h"

#include "raylib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int init_ui(GameUI *ui, Dialog *dia) {
    InitWindow(WIDTH, HEIGHT, APP_NAME);

    ui->menu.play_button = init_button(
        (Rectangle) {
            (WIDTH - 300) / 2,
            150,
            300,
            75,
        },
        "Play",
        &ui_play,
        ui
    );
    ui->dia = dia;
    ui->state = STATE_MENU;

    return 0;
}

void update_ui(GameUI *ui) {
    const Vector2 mouse_pos = GetMousePosition();
    const int click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    switch (ui->state) {
        case STATE_MENU:
            update_button(&ui->menu.play_button, mouse_pos, click);

            break;
        case STATE_GAME:
            break;
    }
}

void draw_ui(GameUI *ui) {
    BeginDrawing();
    
    ClearBackground(BLACK);

    switch (ui->state) {
        case STATE_MENU:
            int w = MeasureText(APP_NAME, 50);
            
            DrawText(
                APP_NAME,
                (WIDTH - w) / 2,
                50,
                50,
                WHITE
            );

            draw_button(&ui->menu.play_button);

            break;
        case STATE_GAME:
            DialogLine line = get_line(ui->dia, ui_dialog);
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

void loop_ui(GameUI *ui) {
    while (!WindowShouldClose()) {
        update_ui(ui);
        draw_ui(ui);
    }
}

void ui_play(void *data) {
    GameUI* ui = data;
            
    if (ui->state == STATE_MENU)
        ui->state = STATE_GAME;
}

void deinit_ui(GameUI *ui) {
    CloseWindow();
}
