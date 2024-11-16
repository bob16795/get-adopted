#include "raylib.h"

#ifndef BUTTON_H
#define BUTTON_H

static const int BTN_BORDER = 5;

typedef void (*ButtonClickFn)(void*);

typedef struct {
    Rectangle pos;
    char* text;

    ButtonClickFn onClick;
    void* data;

    int hover;
} Button;

Button init_button(Rectangle pos, char* text, ButtonClickFn onClick, void *data);
void update_button(Button *btn, Vector2 mouse_pos, int click);
void draw_button(Button *btn);

#endif
