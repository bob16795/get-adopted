#include "character.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "raylib.h"

// Define the global variables - remove static since they're extern in the header
struct FrameData* frameArray = NULL;
int frameCount = 0;

static int compare_frames(const void* a, const void* b) {
    return ((struct FrameData*)a)->id - ((struct FrameData*)b)->id;
}

static int init_frame_array(FILE* fptr) {
    char line[256];
    frameCount = 0;

    // First count valid lines
    while (fgets(line, sizeof(line), fptr)) {
        if (line[0] == '<' && !(line[0] == '/' && line[1] == '/')) {
            frameCount++;
        }
    }

    // Allocate array
    frameArray = malloc(frameCount * sizeof(struct FrameData));
    if (!frameArray) return 1;

    // Reset file pointer
    rewind(fptr);

    // Fill array
    int index = 0;
    while (fgets(line, sizeof(line), fptr)) {
        if (line[0] != '<' || (line[0] == '/' && line[1] == '/')) continue;

        // Get ID
        char id_str[5] = {0};
        int i = 1;
        while (line[i] != '>' && i < 5) {
            id_str[i-1] = line[i];
            i++;
        }

        // Get path
        const char* text_start = strchr(line, '>');
        if (text_start == NULL) continue;
        text_start++;

        size_t len = strlen(text_start);
        char* path = malloc(len + 1);
        if (path == NULL) continue;
        
        strcpy(path, text_start);
        if (len > 0 && path[len-1] == '\n') {
            path[len-1] = '\0';
        }

        // Store in array
        frameArray[index].id = atoi(id_str);
        
        // Load texture and check for errors
        frameArray[index].tex = LoadTexture(path);
        if (frameArray[index].tex.id == 0) {
            printf("Failed to load texture: %s\n", path);
            free(path);
            return 1;
        }
        
        printf("Loaded texture ID %d: %s\n", frameArray[index].id, path);
        free(path);
        index++;
    }

    // Sort array by ID for binary search
    qsort(frameArray, frameCount, sizeof(struct FrameData), compare_frames);
    return 0;
}

int get_character_texture(int target_id, Texture2D* tex) {
    // Initialize on first call
    if (!frameArray) {
        FILE* fptr = fopen("./frameIDLookup.txt", "r");
        if (fptr == NULL) {
            printf("Error opening file\n");
            return 1;
        }
        if (init_frame_array(fptr)) {
            fclose(fptr);
            return 1;
        }
        fclose(fptr);
    }

    // Perform binary search
    struct FrameData key = { .id = target_id };
    struct FrameData* result = bsearch(&key, frameArray, frameCount, 
                                     sizeof(struct FrameData), compare_frames);
    
    if (result == NULL) {
        printf("Texture not found for ID: %d\n", target_id);
        return 0;
    }
    
    *tex = result->tex;
    return 1;
}

void cleanup_frame_data(void) {
    if (frameArray) {
        for (int i = 0; i < frameCount; i++) {
            UnloadTexture(frameArray[i].tex);
        }
        free(frameArray);
        frameArray = NULL;
        frameCount = 0;
    }
}