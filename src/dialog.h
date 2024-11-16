// dialog.h
#ifndef DIALOG_H
#define DIALOG_H

struct dialogLine {
    int identifier;
    int characterFrameID;
    char* dialog;
};

static int g_totalLines;
static struct dialogLine* g_allText;

// some consts
static const char* dialog_file = "dialog.txt";

// Function prototypes
struct dialogLine getDialogLine(int ID);
int getID(const char* line);
int getFrameID(const char* line);
char* get_dialog(const char* line);

int init_dialog(void);  // Changed return type to int for success/failure
void deinit_dialog(void);  // Added cleanup function

char** parseOptions(struct dialogLine* allDialog, int* options, int numOptions);
void freeOptions(char** options, int numOptions);  // Added helper to free options

#endif