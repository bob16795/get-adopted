#include "textbox.h"
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

TextBox init_textbox(Rectangle pos) {
    return (TextBox) {
        pos,
        LoadTexture("down.png"),
        "",
        0,
        0,
        0,
        0
    };
}

void update_textbox(TextBox *box, float dt) {
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

    float remaining = box->pc;

    int len = strlen(box->text);
    char *copy = malloc(len + 1);

    strcpy(copy, box->text);

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

    if (remaining > 0) {
        box->waiting = 1;
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
}

void show_message(TextBox *box, char* text) {
    box->pc = 0;
    box->waiting = 0;
    box->first_line = 0;
    box->text = text;
}