#ifndef UI_H
#define UI_H

typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_PAUSE,
} State;

static const char* APP_NAME = "Dad Picker";
static const int WIDTH = 640;
static const int HEIGHT = 480;

static State state;
static int ui_dialog;

int init_ui();
void loop_ui();
void ui_play();
void deinit_ui();

#endif
