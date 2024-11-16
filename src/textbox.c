#include "textbox.h"

TextBox init_textbox(Rectangle pos) {
    return (TextBox) {
        pos,
        0
    };
}