#include "dialog.h"

int main(void) {
    if (init_dialog() != 0) {
        return 1;
    }
    
    cleanup_dialog();
    return 0;
}
