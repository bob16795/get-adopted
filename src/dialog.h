#ifndef MYHEADER_H
#define MYHEADER_H

struct dialogLine {
    int identifier;
    char* dialog;
};

int getID(const char* line);
char* getDialog(const char* line);
int init_dialog();

#endif
