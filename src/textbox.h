#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "raylib.h"

#ifndef TEXTBOX_H
#define TEXTBOX_H

static const int TEXTBOX_BORDER = 20;
static const int TEXTBOX_PADDING = 25;
static const int TEXTBOX_HEIGHT = 200;
static const int TEXTBOX_LINES = 5;

static const int TEXTBOX_TEXT_HEIGHT = (
    TEXTBOX_HEIGHT - TEXTBOX_BORDER * 4
) / TEXTBOX_LINES;

typedef void (*ChooseCall)(void *, int);

typedef struct {
    Rectangle pos;
    Texture down_texture;

    int first_line;
    int waiting;
    int wait_line;
    int done;
    int choices;
    char **text;
    ChooseCall cb;

    float pc;

    int choice;

    void* data;

    Texture bg;
} TextBox;

TextBox init_textbox(Rectangle pos, void *data);
void update_textbox(TextBox *box, float dt);
void draw_textbox(TextBox *box);
void show_message(TextBox *box, char *text);
void show_choose(TextBox *box, char **choices, int len, ChooseCall func);
void cleanup_textbox(TextBox *box);

#endif
