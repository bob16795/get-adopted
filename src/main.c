#include "dialog.h"
#include "character.h"
#include "ui.h"
#include <stdio.h>

int main(void) {
    // setup
    if (init_dialog()) return 1;
    if (init_ui()) return 1;


    // loop
    loop_ui();

    // cleanup
	deinit_ui();
    deinit_dialog();

    return 0;
}   

