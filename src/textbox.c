#include "textbox.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

TextBox init_textbox(Rectangle pos, void *data) {
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
        0,
        data
    };

    result.text[0] = calloc(1, sizeof(char));
    result.bg = LoadTexture("others/dialoguebox.png");

    return result;
}

void update_textbox(TextBox *box, float dt) {
    if (box->choices) {
        if (IsKeyPressed(KEY_DOWN)) {
            box->choice = (box->choice + 1) % box->choices;
        }
        if (IsKeyPressed(KEY_UP)) {
            box->choice = (box->choice - 1 + box->choices) % box->choices;
        }
        if (IsKeyPressed(KEY_ENTER) && box->cb) {
            box->cb(box->data, box->choice);
            box->cb = NULL;
        }

        return;
    }

    if (!box->waiting) {
        if (IsKeyDown(KEY_ENTER)) {
            box->pc += dt * 10.0;
        } else {
            box->pc += dt * 5.0;
        }
    } else {
        if (IsKeyPressed(KEY_ENTER)) {
            if (box->wait_line) {
                box->first_line += 1;
            } else {
                box->done = true;
            }
            box->waiting = false;
        }
    }
}

void draw_textbox(TextBox *box) {
    int space_size = MeasureText("  ", TEXTBOX_TEXT_HEIGHT);
    
    DrawTexturePro(
        box->bg,
        (Rectangle){0, 0, box->bg.width, box->bg.height},
        box->pos,
        (Vector2){0, 0},        
        0.0,
        WHITE
    );
    
    Rectangle inside = (Rectangle) {
        box->pos.x + TEXTBOX_BORDER,
        box->pos.y + TEXTBOX_BORDER,
        box->pos.width - TEXTBOX_BORDER * 2,
        box->pos.height - TEXTBOX_BORDER * 2,
    };

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
                    box->wait_line = 1;
                    return;
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
                    BLACK
                );
            }

            x += width + space_size;

            pch = strtok(NULL, " ");
            
            remaining -= 1;
        }

        if (remaining > 0) {
            box->waiting = 1;
            box->wait_line = 0;

            return;
        }

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
    } else {  // choices section
        for (int line = 0; line < box->choices; line++) {
            const char *str = box->text[line];
            char *copy = calloc(sizeof(char), MIN(30, strlen(str) + 1));
            if (!copy) continue;
            
            // Add selection indicator
            if (line == box->choice) {
                copy[0] = '>';
                memcpy(copy + 1, str, MIN(28, strlen(str)));
            } else {
                memcpy(copy, str, MIN(29, strlen(str)));
            }

            Color text_color = (line == box->choice) ? BLACK : BLACK;
            
            DrawText(
                copy,
                inside.x + TEXTBOX_BORDER + TEXTBOX_TEXT_HEIGHT,
                inside.y + TEXTBOX_BORDER + TEXTBOX_TEXT_HEIGHT * line,
                TEXTBOX_TEXT_HEIGHT,
                text_color
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
    box->done = false;
}

void show_choose(TextBox *box, char **choices, int len, ChooseCall func) {
    box->pc = 0;
    box->waiting = 0;
    box->first_line = 0;
    box->choices = len;
    box->choice = 0;
    box->done = false;
    box->cb = func;

    // Allocate and copy new choices
    box->text = malloc(len * sizeof(char*));
    if (box->text) {
        for (int i = 0; i < len; i++) {
            if (choices[i]) {
                box->text[i] = strdup(choices[i]);
            } else {
                box->text[i] = NULL;
            }
        }
    }
}