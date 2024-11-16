#include "dialog.h"
#include "ui.h"

int main(void) {
    if (init_dialog() != 0) {
        return 1;
    }
    
	init_ui();

    loop_ui();

	deinit_ui();
    deinit_dialog();
    return 0;
}   

