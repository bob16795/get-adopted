#include "button.h"
#include "textbox.h"
#include "dialog.h"

#ifndef UI_H
#define UI_H

typedef enum {
    STATE_MENU,
    STATE_GAME,
} State;

typedef struct {
    struct {
        Button play_button;

        Texture title[2];
    } menu;

    struct {
        int dialog;
        TextBox dialog_box;
        int happynessPoint;
        int currChar;
    } game;
    
    float timer;

    Dialog *dia;

    State state;
    Sound click1;
    Sound click2;
    Sound click3;
    Sound creepy1;
    Sound creepy2;
    Sound creepy3;
    Sound gibberish1;
    Sound gibberish2;
    Sound gibberish3;
    Sound meow1;
    Sound meow2;
    Sound meow3;
    Sound news1;
    Sound sim1;
    Sound sim2;
    Sound sim3;
    Sound snarky1;
    Sound snarky2;
    Sound snarky3;
    Sound dadSound;
    Sound youwin;
} GameUI;

static const char* APP_NAME = "Dad Picker";
static const int WIDTH = 640;
static const int HEIGHT = 480;

static int ui_dialog;

int init_ui(GameUI *ui, Dialog *dia);
void loop_ui(GameUI *ui);
void choose_ui(int choices);
void ui_play(void *data);
void deinit_ui(GameUI *ui);


#endif
