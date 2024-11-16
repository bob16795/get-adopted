#include "button.h"
#include "textbox.h"
#include "dialog.h"

#ifndef UI_H
#define UI_H

typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_PAUSE,
} State;

typedef struct {
    struct {
        Button play_button;
    } menu;

    struct {
        int dialog;
        TextBox dialog_box;
        int happynessPoint;
        int currChar;
    } game;

    Dialog *dia;

    State state;

} GameUI;

static const char* APP_NAME = "Dad Picker";
static const int WIDTH = 1280;
static const int HEIGHT = 960;

static int ui_dialog;

int init_ui(GameUI *ui, Dialog *dia);
void loop_ui(GameUI *ui);
void ui_play(void *data);
void deinit_ui(GameUI *ui);

#endif
