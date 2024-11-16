//dialog.c
#include "dialog.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Global variables
static struct dialogLine* g_allText = NULL;
static int g_totalLines = 0;

int init_dialog(void) {
    FILE* fptr = fopen(dialog_file, "r");
    if (fptr == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    
    char line[1035];
    while (fgets(line, sizeof(line), fptr)) {
        if (line[0] != '<' || (line[0] == '/' && line[1] == '/')) continue; //Skip invalid lines
        
        g_totalLines++;
        struct dialogLine* temp = realloc(g_allText, g_totalLines * sizeof(struct dialogLine));
        if (temp == NULL) {
            printf("Memory allocation failed\n");
            deinit_dialog();
            fclose(fptr);
            return 1;
        }
        g_allText = temp;
        
        //Parse the line
        g_allText[g_totalLines - 1].identifier = getID(line);
        g_allText[g_totalLines - 1].characterFrameID = getFrameID(line);
        g_allText[g_totalLines - 1].dialog = get_dialog(line);
        
        if (g_allText[g_totalLines - 1].dialog == NULL) {
            printf("Failed to allocate dialog for line %d\n", g_totalLines);
            deinit_dialog();
            fclose(fptr);
            return 1;
        }
        
        // printf("Line %d (Frame %d): %s\n", 
        //        g_allText[g_totalLines - 1].identifier,
        //        g_allText[g_totalLines - 1].characterFrameID,
        //        g_allText[g_totalLines - 1].dialog);
    }
    
    fclose(fptr);
    return 0;
}

struct dialogLine getDialogLine(int ID) {
    return g_allText[ID];
}

void deinit_dialog(void) {
    if (g_allText != NULL) {
        for (int i = 0; i < g_totalLines; i++) {
            free(g_allText[i].dialog);
        }
        free(g_allText);
        g_allText = NULL;
    }
    g_totalLines = 0;
}

char** parseOptions(struct dialogLine* allDialog, int* options, int numOptions) {
    if (allDialog == NULL || options == NULL || numOptions <= 0) {
        return NULL;
    }
    
    //Allocate array of char pointers
    char** optionsOutput = calloc(numOptions, sizeof(char*));
    if (optionsOutput == NULL) return NULL;
    
    //For each option, directly get the dialog using the ID as index
    for (int i = 0; i < numOptions; i++) {
        int id = options[i];
        if (id >= 0 && id < g_totalLines) {  //Check bounds
            optionsOutput[i] = strdup(allDialog[id].dialog);
            if (optionsOutput[i] == NULL) {
                freeOptions(optionsOutput, i);
                return NULL;
            }
        } else {
            optionsOutput[i] = strdup("Invalid option");
            if (optionsOutput[i] == NULL) {
                freeOptions(optionsOutput, i);
                return NULL;
            }
        }
    }
    
    return optionsOutput;
}

void freeOptions(char** options, int numOptions) {
    if (options != NULL) {
        for (int i = 0; i < numOptions; i++) {
            free(options[i]);
        }
        free(options);
    }
}

//Get the first ID (LineID) from format <XXXX><YYYY>
int getID(const char* line) {
    char id_str[5] = {0}; //Only take 4 digits
    //Skip the '<' and copy until '>'
    int i = 1; //Start after '<'
    while (line[i] != '>' && i < 5) {
        id_str[i-1] = line[i];
        i++;
    }
    return atoi(id_str);
}

//Get the second ID (FrameID) from format <XXXX><YYYY>
int getFrameID(const char* line) {
    const char* frame_start = strchr(line, '>');
    if (frame_start == NULL || frame_start[1] != '<') return 0;
    
    char frame_str[5] = {0};
    int i = 0;
    frame_start += 2; //Skip '><'
    
    while (frame_start[i] != '>' && i < 4) {
        frame_str[i] = frame_start[i];
        i++;
    }
    
    return atoi(frame_str);
}

char* get_dialog(const char* line) {
    //Find the second '>' character
    const char* first_bracket = strchr(line, '>');
    if (first_bracket == NULL) return NULL;
    
    const char* text_start = strchr(first_bracket + 1, '>');
    if (text_start == NULL) return NULL;
    
    text_start++; //Move past the second '>'
    size_t len = strlen(text_start);
    char* dialog = malloc(len + 1);
    if (dialog == NULL) return NULL;
    
    strcpy(dialog, text_start);
    //Remove trailing newline if present
    if (len > 0 && dialog[len-1] == '\n') {
        dialog[len-1] = '\0';
    }
    
    return dialog;
}