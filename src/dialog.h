// dialog.h
#ifndef DIALOG_H
#define DIALOG_H

struct dialogLine {
    int identifier;
    int characterFrameID;
    char* dialog;
};

extern int g_totalLines;
extern struct dialogLine* g_allText;

// Function prototypes
int getID(const char* line);
int getFrameID(const char* line);
char* getDialog(const char* line);
int init_dialog(void);  // Changed return type to int for success/failure
void cleanup_dialog(void);  // Added cleanup function
char** parseOptions(struct dialogLine* allDialog, int* options, int numOptions);
void freeOptions(char** options, int numOptions);  // Added helper to free options

#endif