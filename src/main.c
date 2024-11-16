#include "dialog.h"
#include "ui.h"

int main(void) {
    if (init_dialog()) return 1;
    if (init_ui()) return 1;

    loop_ui();

	deinit_ui();
    deinit_dialog();
    return 0;
}   

