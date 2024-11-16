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
    ui->timer = 0;
    ui->game.dialog = 0;
    ui->game.dialog_box = init_textbox(
        (Rectangle) {
            TEXTBOX_PADDING,
            HEIGHT - TEXTBOX_HEIGHT - TEXTBOX_PADDING,
            WIDTH - TEXTBOX_PADDING * 2,
            TEXTBOX_HEIGHT
        }
    );

    ui->menu.title[0] = LoadTexture("Art/title1.png");
    ui->menu.title[1] = LoadTexture("Art/title2.png");

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

            ui->timer += dt;

            break;
        case STATE_GAME:
            update_textbox(&ui->game.dialog_box, dt);

            if (ui->game.dialog_box.done) {
                if (IsKeyPressed(KEY_ENTER)) {
                    DialogLine line = get_line(ui->dia, ui->game.dialog);
                    ui->game.happynessPoint += line.pointGain;
                    if (line.characterID != 0) {
                        ui->game.currChar = line.characterID;
                    }
                    if (ui->game.happynessPoint < -1) {
                        ui->game.happynessPoint = 0;
                        switch (ui->game.currChar) {
                            case 1: line = get_line(ui->dia, 184); break;
                            case 2: line = get_line(ui->dia, 185); break;
                            case 3: line = get_line(ui->dia, 187); break;
                            case 4: line = get_line(ui->dia, 186); break;
                            case 5: line = get_line(ui->dia, 188); break;
                        }
                    }
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
            
    Texture tex = {0};

    switch (ui->state) {
        case STATE_MENU:
            tex = ui->menu.title[(int)(ui->timer) % 2];

            DrawTexturePro(
                tex,
                (Rectangle){0, 0, (float)tex.width, (float)tex.height},
                (Rectangle){0, 0, WIDTH, HEIGHT},
                (Vector2){0, 0},
                0.0,
                WHITE
            );

            int w = MeasureText(APP_NAME, 50);

            draw_button(&ui->menu.play_button);

            break;
        case STATE_GAME:
            DialogLine line = get_line(ui->dia, ui_dialog);
            get_scene_texture(line.sceneID, &tex);

            DrawTexturePro(
                tex,
                (Rectangle){0, 0, (float)tex.width, (float)tex.height},
                (Rectangle){0, 0, WIDTH, HEIGHT},
                (Vector2){0, 0},
                0.0,
                WHITE
            );
            
            get_character_texture(line.characterFrameID, &tex);

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
