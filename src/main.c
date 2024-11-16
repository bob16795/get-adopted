#include "dialog.h"
#include "character.h"
#include "ui.h"
#include <stdio.h>

int main(void) {
    GameUI ui;
    Dialog dialog;

    // setup
    if (init_dialog(&dialog)) return 1;
    if (init_ui(&ui, &dialog)) return 1;

    // loop
    loop_ui(&ui);

    // cleanup
	deinit_ui(&ui);
    deinit_dialog(&dialog);

    // audio 
    void InitAudioDevice(void);   

    return 0;
}   