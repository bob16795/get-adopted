#include "textbox.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

TextBox init_textbox(Rectangle pos) {
    TextBox result = (TextBox) {
        pos,
        LoadTexture("down.png"),
        0,
        0,
        0,
        0,
        0,
        malloc(sizeof(char*)),
        NULL,
        0.0,
        0
    };

    result.text[0] = calloc(1, sizeof(char));

    return result;
}

void update_textbox(TextBox *box, float dt) {
    if (box->choices) {
        if (IsKeyDown(KEY_DOWN)) {
            box->choice += 1;
        }
        
        return;
    }

    if (!box->waiting) {
        if (IsKeyDown(KEY_ENTER)) {
            box->pc += dt * 15.0;
        } else {
            box->pc += dt * 5.0;
        }
    } else {
        if (IsKeyDown(KEY_ENTER)) {
            if (box->wait_line) {
                box->first_line += 4;
            } else {
                box->done = true;
            }
            box->waiting = false;
        }
    }
}

void draw_textbox(TextBox *box) {
    int space_size = MeasureText("  ", TEXTBOX_TEXT_HEIGHT);

    DrawRectangleRec(
        box->pos,
        WHITE
    );
    
    Rectangle inside = (Rectangle) {
        box->pos.x + TEXTBOX_BORDER,
        box->pos.y + TEXTBOX_BORDER,
        box->pos.width - TEXTBOX_BORDER * 2,
        box->pos.height - TEXTBOX_BORDER * 2,
    };

    DrawRectangleRec(
        inside,
        BLACK
    );

    if (box->choices == 0) {
        float remaining = box->pc;

        int len = strlen(box->text[0]);
        char *copy = malloc(len + 1);

        strcpy(copy, box->text[0]);

        const int total_width = inside.width - TEXTBOX_BORDER * 2 - TEXTBOX_TEXT_HEIGHT;
        int x = 0;
        int line = -box->first_line;

        char *pch = strtok(copy, " ");
        while (pch && (remaining > 0)) {
            int width = MeasureText(pch, TEXTBOX_TEXT_HEIGHT);

            if (x + width > total_width) {
                x = 0;
                line += 1;
                if (line >= TEXTBOX_LINES) {
                    box->waiting = 1;
                    break;
                }
            }

            if (line >= 0) {
                int cpy_len = strlen(pch) * MIN(1.0, remaining);
                char *cpy = malloc(cpy_len + 1);
                memcpy(cpy, pch, cpy_len);
                cpy[cpy_len] = 0;

                DrawText(
                    cpy,
                    inside.x + TEXTBOX_BORDER + x,
                    inside.y + TEXTBOX_BORDER + TEXTBOX_TEXT_HEIGHT * line,
                    TEXTBOX_TEXT_HEIGHT,
                    WHITE
                );
            }

            x += width + space_size;

            pch = strtok(NULL, " ");
            
            remaining -= 1;
        }

        if (remaining > 0)
            box->waiting = 1;

        if (box->waiting) {
            DrawTexturePro(
                box->down_texture,
                (Rectangle){0, 0, 7, 7},
                (Rectangle){
                    inside.x + inside.width - TEXTBOX_BORDER - TEXTBOX_TEXT_HEIGHT,
                    inside.y + inside.height - TEXTBOX_BORDER - TEXTBOX_TEXT_HEIGHT,
                    TEXTBOX_TEXT_HEIGHT,
                    TEXTBOX_TEXT_HEIGHT
                },
                (Vector2){0, 0},
                0,
                WHITE
            );
        }

        free(copy);
    } else {
        for (int line = 0; line < (box->choices); line++) {
            const char *str = box->text[line];
            char *copy = calloc(sizeof(char), MIN(30, strlen(str)));
            memcpy(copy, str, MIN(29, strlen(str)));

            DrawText(
                copy,
                inside.x + TEXTBOX_BORDER + TEXTBOX_TEXT_HEIGHT,
                inside.y + TEXTBOX_BORDER + TEXTBOX_TEXT_HEIGHT * line,
                TEXTBOX_TEXT_HEIGHT,
                WHITE
            );

            free(copy);
        }
    }
}   

void show_message(TextBox *box, char* text) {
    box->pc = 0;
    box->waiting = 0;
    box->first_line = 0;
    box->text = malloc(1 * sizeof(char*));
    box->choices = 0;
    box->text[0] = text;
}

void show_choose(TextBox *box, char **choices, int len, ChooseCall func) {
    box->pc = 0;
    box->waiting = 0;
    box->first_line = 0;
    box->text = malloc(1 * sizeof(char*));
    box->choices = len;
    box->text = choices;
    box->choice = 0;
    box->cb = func;
}