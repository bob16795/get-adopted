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

    InitAudioDevice();

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

    ui->sounds = calloc(20, sizeof(Sound));
    ui->numSounds = 20;

    ui->sounds[0] = LoadSound("sounds/click1.wav");
    ui->sounds[1] = LoadSound("sounds/click2.wav");
    ui->sounds[2] = LoadSound("sounds/click3.wav");
    ui->sounds[3] = LoadSound("sounds/creepy1.wav");
    ui->sounds[4] = LoadSound("sounds/creepy2.wav");
    ui->sounds[5] = LoadSound("sounds/creepy3.wav");
    ui->sounds[6] = LoadSound("sounds/gibberish1.wav");
    ui->sounds[7] = LoadSound("sounds/gibberish2.wav");
    ui->sounds[8] = LoadSound("sounds/gibberish3.wav");
    ui->sounds[9] = LoadSound("sounds/meow1.wav");
    ui->sounds[10] = LoadSound("sounds/meow2.wav");
    ui->sounds[11] = LoadSound("sounds/meow3.wav");
    ui->sounds[12] = LoadSound("sounds/news1.wav");
    ui->sounds[13] = LoadSound("sounds/sim1.wav");
    ui->sounds[14] = LoadSound("sounds/sim2.wav");
    ui->sounds[15] = LoadSound("sounds/sim3.wav");
    ui->sounds[16] = LoadSound("sounds/snarky1.wav");
    ui->sounds[17] = LoadSound("sounds/snarky2.wav");
    ui->sounds[18] = LoadSound("sounds/snarky3.wav");
    ui->sounds[19] = LoadSound("sounds/dadSound.wav");
    ui->winSound = LoadSound("sounds/youwin.wav");

    
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

                    int random_sound = GetRandomValue(0, 20);
                    PlaySound(ui->sounds[random_sound]);
                    if (ui->game.dialog == 9 || ui->game.dialog == 54 || ui->game.dialog == 102 || ui->game.dialog == 138 || ui->game.dialog == 183) {
                        PlaySound(ui->winSound);
                    }

                    if (!line.next_count) {
                        ui->game.dialog = line.next[0];

                        DialogLine msg_line = get_line(ui->dia, ui->game.dialog);
                        show_message(&ui->game.dialog_box, msg_line.dialog);
                    } else {
                        char** stringOptions = parse_options(ui->dia, line.next, line.next_count);
                        show_choose(&ui->game.dialog_box, stringOptions, line.next_count, choose_ui);
                    }
                }
            }

            break;
    }
}

void choose_ui(int choice) {
    
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
                (Rectangle){0, 0, WIDTH/2, HEIGHT * 0.7},
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
