#include "button.h"

Button init_button(Rectangle pos, char* text, ButtonClickFn onClick) {
    return (Button){
        pos,
        text,
        onClick
    };
}

void update_button(Button *btn, Vector2 mouse_pos, int click) {
    btn->hover = CheckCollisionPointRec(
        mouse_pos,
        btn->pos
    );

    if (click && btn->hover)
        btn->onClick();
}

void draw_button(Button *btn) {
    DrawRectangleRec(
        btn->pos,
        WHITE
    );
    
    Rectangle inside = (Rectangle) {
        btn->pos.x + BTN_BORDER,
        btn->pos.y + BTN_BORDER,
        btn->pos.width - BTN_BORDER * 2,
        btn->pos.height - BTN_BORDER * 2,
    };

    if (!btn->hover)
        DrawRectangleRec(
            inside,
            BLACK
        );

    const int text_height = inside.height - BTN_BORDER * 2;

    const int width = MeasureText(btn->text, text_height);

    DrawText(
        btn->text,
        btn->pos.x + (btn->pos.width - width) / 2,
        inside.y + BTN_BORDER,
        text_height,
        btn->hover ? BLACK : WHITE
    );
}