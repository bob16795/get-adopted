#include "raylib.h"

#ifndef TEXTBOX_H
#define TEXTBOX_H

static const int TEXTBOX_BORDER = 5;

typedef struct {
    Rectangle pos;

    float pc;
} TextBox;

TextBox init_textbox(Rectangle pos);
void update_textbox(TextBox *box, float dt);
void draw_textbox(TextBox *box);

#endif
