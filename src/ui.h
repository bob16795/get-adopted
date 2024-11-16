#ifndef UI_H
#define UI_H

static const char* APP_NAME = "Dad Picker";
static const int WIDTH = 640;
static const int HEIGHT = 480;

static int ui_dialog;

int init_ui();
void loop_ui();
void deinit_ui();

#endif
