// dialog.h
#ifndef DIALOG_H
#define DIALOG_H

typedef struct {
    int identifier;
    int characterFrameID;
    int sceneID;
    int pointGain;
    char* dialog;
    int next_count;
    int* next;
} DialogLine;

typedef struct {
    DialogLine *lines;
    int line_count;
} Dialog;

// some consts
static const char* dialog_file = "dialog.txt";
static const char* graph_file = "graph.csv";

// helpers
int get_id(const char* line);
int get_frame_id(const char* line);
int get_points(const char* line);
int get_scene_id(const char* line);
char* get_dialog(const char* line);

// Function prototypes
DialogLine get_line(Dialog *dia, int ID);

int init_dialog(Dialog *dia);  // Changed return type to int for success/failure
void deinit_dialog(Dialog *dia);  // Added cleanup function

char** parse_options(Dialog* dia, int* options, int numOptions);
void freeOptions(char** options, int numOptions);  // Added helper to free options

#endif