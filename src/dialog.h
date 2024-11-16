#ifndef DIALOG_H
#define DIALOG_H

struct dialogLine {
    int identifier;
    char* dialog;
};

int getID(const char* line);
char* getDialog(const char* line);
int init_dialog();

#endif
