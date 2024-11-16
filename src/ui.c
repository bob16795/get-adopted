#include "ui.h"
#include "dialog.h"
#include "character.h"
#include "scene.h"

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
    ui->game.dialog = 0;
    ui->game.dialog_box = init_textbox(
        (Rectangle) {
            TEXTBOX_PADDING,
            HEIGHT - TEXTBOX_HEIGHT - TEXTBOX_PADDING,
            WIDTH - TEXTBOX_PADDING * 2,
            TEXTBOX_HEIGHT
        }
    );

    ui->dia = dia;
    ui->state = STATE_MENU;

    DialogLine line = get_line(dia, ui->game.dialog);
    show_message(&ui->game.dialog_box, line.dialog);

    return 0;
}

void update_ui(GameUI *ui) {
    const Vector2 mouse_pos = GetMousePosition();
    const int click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    const float dt = GetFrameTime();

    switch (ui->state) {
        case STATE_MENU:
            update_button(&ui->menu.play_button, mouse_pos, click);

            break;
        case STATE_GAME:
            update_textbox(&ui->game.dialog_box, dt);

            if (ui->game.dialog_box.done) {
                if (IsKeyPressed(KEY_ENTER)) {
                    DialogLine line = get_line(ui->dia, ui->game.dialog);
                    if (line.next_count) {
                        ui->game.dialog = line.next[0];

                        DialogLine msg_line = get_line(ui->dia, ui->game.dialog);
                        show_message(&ui->game.dialog_box, msg_line.dialog);
                    }
                }
            }

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
            Texture tex = {0};
            get_scene_texture(line.sceneID, &tex);

            DrawTexturePro(
                tex,
                (Rectangle){0, 0, (float)tex.width, (float)tex.height},
                (Rectangle){0, 0, WIDTH, HEIGHT},
                (Vector2){0, 0},
                0.0,
                WHITE
            );

            draw_textbox(&ui->game.dialog_box);
            
            // DrawText(line.dialog, 1, 1, 50, LIGHTGRAY);
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
