//dialog.c
#include "dialog.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int init_dialog(Dialog *dia) {
    memset(dia, 0, sizeof(Dialog));

    FILE* fptr = fopen(dialog_file, "r");
    if (fptr == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    
    char line[1042];
    while (fgets(line, sizeof(line), fptr)) {
        if (line[0] != '<' || (line[0] == '/' && line[1] == '/')) continue; //Skip invalid lines
        
        dia->line_count++;
        DialogLine* temp = realloc(dia->lines, dia->line_count * sizeof(DialogLine));
        if (temp == NULL) {
            printf("Memory allocation failed\n");
            deinit_dialog(dia);
            fclose(fptr);
            return 1;
        }
        dia->lines = temp;
        
        //Parse the line
        dia->lines[dia->line_count - 1].identifier = get_id(line);
        dia->lines[dia->line_count - 1].characterFrameID = get_frame_id(line);
        dia->lines[dia->line_count - 1].sceneID = get_scene_id(line);
        dia->lines[dia->line_count - 1].dialog = get_dialog(line);
        
        if (dia->lines[dia->line_count - 1].dialog == NULL) {
            printf("Failed to allocate dialog for line %d\n", dia->line_count);
            deinit_dialog(dia);
            fclose(fptr);
            return 1;
        }
        
        printf("Line %d (Frame %d) (Scene %d): %s\n", 
               dia->lines[dia->line_count - 1].identifier,
               dia->lines[dia->line_count - 1].characterFrameID,
               dia->lines[dia->line_count - 1].sceneID,
               dia->lines[dia->line_count - 1].dialog);
    }
    
    fclose(fptr);
    return 0;
}

DialogLine get_line(Dialog *dia, int ID) {
    return dia->lines[ID];
}

void deinit_dialog(Dialog *dia) {
    if (dia->lines != NULL) {
        for (int i = 0; i < dia->line_count; i++) {
            free(dia->lines[i].dialog);
        }
        free(dia->lines);
        dia->lines = NULL;
    }
    dia->line_count = 0;
}

char** parse_options(Dialog* dia, int* options, int numOptions) {
    if (dia->line_count == NULL || options == NULL || numOptions <= 0) {
        return NULL;
    }
    
    //Allocate array of char pointers
    char** optionsOutput = calloc(numOptions, sizeof(char*));
    if (optionsOutput == NULL) return NULL;
    
    //For each option, directly get the dialog using the ID as index
    for (int i = 0; i < numOptions; i++) {
        int id = options[i];
        if (id >= 0 && id < dia->line_count) {  //Check bounds
            optionsOutput[i] = strdup(dia->lines[id].dialog);
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

//Get the first ID (LineID) from format <XXXX><YYYY><ZZZZ>
int get_id(const char* line) {
    char id_str[5] = {0}; //Only take 4 digits
    //Skip the '<' and copy until '>'
    int i = 1; //Start after '<'
    while (line[i] != '>' && i < 5) {
        id_str[i-1] = line[i];
        i++;
    }
    return atoi(id_str);
}

//Get the second ID (FrameID) from format <XXXX><YYYY><ZZZZ>
int get_frame_id(const char* line) {
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

//Get the third ID (SceneID) from format <XXXX><YYYY><ZZZZ>
int get_scene_id(const char* line) {
    // Find first '>'
    const char* first = strchr(line, '>');
    if (first == NULL) return 0;
    
    //Find second '>'
    const char* second = strchr(first + 1, '>');
    if (second == NULL) return 0;
    
    //Find start of third tag
    const char* scene_start = strchr(second + 1, '<');
    if (scene_start == NULL) return 0;
    
    //Skip the '<'
    scene_start++;
    
    char scene_str[5] = {0};
    int i = 0;
    
    while (scene_start[i] != '>' && i < 4) {
        scene_str[i] = scene_start[i];
        i++;
    }
    
    return atoi(scene_str);
}

char* get_dialog(const char* line) {
    //Find the second '>' character
    const char* first_bracket = strchr(line, '>');
    if (first_bracket == NULL) return NULL;
    
    const char* second_bracket = strchr(first_bracket + 1, '>');
    if (second_bracket == NULL) return NULL;

    const char* text_start = strchr(second_bracket + 1, '>');
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