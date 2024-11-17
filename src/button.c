#include "button.h"

Button init_button(Rectangle pos, char* text, ButtonClickFn onClick, void* data) {
    return (Button){
        pos,
        text,
        onClick,
        data,
        0,
        LoadTexture("others/play.png")
    };
}

void update_button(Button *btn, Vector2 mouse_pos, int click) {
    btn->hover = CheckCollisionPointRec(
        mouse_pos,
        btn->pos
    );

    if (click && btn->hover)
        btn->onClick(btn->data);
}

void draw_button(Button *btn) {
    float rotation = (!btn->hover) ? 0.0 : 0.25; 

    DrawTexturePro(
        btn->tex,
        (Rectangle){0, 0, btn->tex.width, btn->tex.height},
        (Rectangle){
            btn->pos.x + btn->pos.width / 2,
            btn->pos.y + btn->pos.height / 2,
            btn->pos.width,
            btn->pos.height
        },
        (Vector2){
            btn->pos.width / 2,
            btn->pos.height / 2
        },        
        rotation,
        WHITE
    );
}