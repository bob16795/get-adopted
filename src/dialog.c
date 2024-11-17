//dialog.c
#include "dialog.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int init_dialog(Dialog *dia) {
    FILE* fptr;
    char line[1042];
    
    memset(dia, 0, sizeof(Dialog));
    
    fptr = fopen(dialog_file, "r");
    if (fptr == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    
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
        dia->lines[dia->line_count - 1].pointGain = get_points(line);
        dia->lines[dia->line_count - 1].dialog = get_dialog(line);
        dia->lines[dia->line_count - 1].next_count = 0;

        /*
        Cruz: 1
        Jack: 2
        Lester: 3
        Sensei: 4
        Travis: 5
        Player/Tabitha: 6
        */




        
        if (dia->lines[dia->line_count - 1].dialog == NULL) {
            // printf("Failed to allocate dialog for line %d\n", dia->line_count);
            // deinit_dialog(dia);
            // fclose(fptr);
            // return 1;
            // HACK: lazy
            dia->lines[dia->line_count - 1].dialog = "";
        }

        if (dia->lines[dia->line_count - 1].characterFrameID < 4) {
            dia->lines[dia->line_count - 1].characterID = 1;
        }
        else if (dia->lines[dia->line_count - 1].characterFrameID < 7) {
            dia->lines[dia->line_count - 1].characterID = 2;
        }
        else if (dia->lines[dia->line_count - 1].characterFrameID < 10) {
            dia->lines[dia->line_count - 1].characterID = 3;
        }
        else if (dia->lines[dia->line_count - 1].characterFrameID < 14) {
            dia->lines[dia->line_count - 1].characterID = 4;
        }
        else if (dia->lines[dia->line_count - 1].characterFrameID < 18 && dia->lines[dia->line_count - 1].characterFrameID > 15) {
            dia->lines[dia->line_count - 1].characterID = 5;
        } else {
            dia->lines[dia->line_count - 1].characterID = 0;
        }
        printf("Line %d (Frame %d) (Scene %d) (Points %d): %s\n", 
               dia->lines[dia->line_count - 1].identifier,
               dia->lines[dia->line_count - 1].characterFrameID,
               dia->lines[dia->line_count - 1].sceneID,
               dia->lines[dia->line_count - 1].pointGain,
               dia->lines[dia->line_count - 1].dialog);
    }
    
    fclose(fptr);
    
    fptr = fopen(graph_file, "r");
    while (fgets(line, sizeof(line), fptr)) {

        if (line[0] == '#') continue;

        int count, idx;
        for (count = 0, idx = 0; line[idx]; idx ++)
            count += (line[idx] == ',');

        char *pch;
        pch = strtok(line, ",");

        int base_idx = atoi(pch);
        
        dia->lines[base_idx].next = malloc((count) * sizeof(int));
        dia->lines[base_idx].next_count = count;

        int next_idx = 0;

        pch = strtok(NULL, ",");
        while (pch) {
            dia->lines[base_idx].next[next_idx++] = atoi(pch);

            pch = strtok(NULL, ",");
        }
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
    if (options == NULL || numOptions <= 0) {
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

//Get the third ID (SceneID) from format <XXXX><YYYY><ZZZZ><+AA>
int get_points(const char* line) {
    // Find first '>'
    const char* first = strchr(line, '>');
    if (first == NULL) return 0;
    
    //Find second '>'
    const char* second = strchr(first + 1, '>');
    if (second == NULL) return 0;
    
    //Find start of third tag
    const char* third = strchr(second + 1, '<');
    if (third == NULL) return 0;

    //Find start of fourth tag
    const char* point_start = strchr(third + 1, '<');
    if (point_start == NULL) return 0;
    
    //Skip the '<'
    point_start++;
    
    char scene_str[4] = {0};
    int i = 0;
    
    while (point_start[i] != '>' && i < 4) {
        scene_str[i] = point_start[i];
        i++;
    }

    char* returnStr = calloc(2, sizeof(char));
    returnStr[0] = point_start[1];
    returnStr[1] = point_start[2];
    if (scene_str[0] == '-') {
        return -1 * atoi(returnStr);
    } 
    
    return atoi(scene_str);
}

char* get_dialog(const char* line) {
    //Find the second '>' character
    const char* first = strchr(line, '>');
    if (first == NULL) return 0;
    
    //Find second '>'
    const char* second = strchr(first + 1, '>');
    if (second == NULL) return 0;
    
    //Find start of third tag
    const char* third = strchr(second + 1, '<');
    if (third == NULL) return 0;

    //Find start of fourth tag
    const char* fourth = strchr(third + 1, '<');
    if (fourth == NULL) return 0;

    const char* text_start = strchr(fourth + 1, '>');
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