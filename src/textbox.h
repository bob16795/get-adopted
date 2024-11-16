#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "raylib.h"

#ifndef TEXTBOX_H
#define TEXTBOX_H

static const int TEXTBOX_BORDER = 5;
static const int TEXTBOX_PADDING = 25;
static const int TEXTBOX_HEIGHT = 200;
static const int TEXTBOX_LINES = 5;

static const int TEXTBOX_TEXT_HEIGHT = (
    TEXTBOX_HEIGHT - TEXTBOX_BORDER * 4
) / TEXTBOX_LINES;

typedef void (*ChooseCall)(void*);

typedef struct {
    Rectangle pos;
    Texture down_texture;
    char *text;

    int first_line;
    int waiting;
    int wait_line;
    int done;
    float pc;
} TextBox;

TextBox init_textbox(Rectangle pos);
void update_textbox(TextBox *box, float dt);
void draw_textbox(TextBox *box);
void show_message(TextBox *box, char *text);
void shop_choose(TextBox *box, char **choices, int len, ChooseCall func);

#endif
