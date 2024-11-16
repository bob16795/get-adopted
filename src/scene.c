#include "raylib.h"
#include "scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Global array to store frame data
static struct sceneData* sceneArray = NULL;
static int sceneCount = 0;

//Compare function for binary search
int compare_scenes(const void* a, const void* b) {
    return ((struct sceneData*)a)->id - ((struct sceneData*)b)->id;
}

//Initialize the scene data array
int init_scene_array(FILE* fptr) {
    char line[69];
    sceneCount = 0;
    
    //First count valid lines
    while (fgets(line, sizeof(line), fptr)) {
        if (line[0] == '<' && !(line[0] == '/' && line[1] == '/')) {
            sceneCount++;
        }
    }
    
    //Allocate array
    sceneArray = malloc(sceneCount * sizeof(struct sceneData));
    if (!sceneArray) return 1;
    
    //Reset file pointer
    rewind(fptr);
    
    //Fill array
    int index = 0;
    while (fgets(line, sizeof(line), fptr)) {
        if (line[0] != '<' || (line[0] == '/' && line[1] == '/')) continue;
        
        //Get ID
        char id_str[5] = {0};
        int i = 1;
        while (line[i] != '>' && i < 5) {
            id_str[i-1] = line[i];
            i++;
        }
        
        //Get path
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
        
        //Store in array
        sceneArray[index].id = atoi(id_str);
        sceneArray[index].tex = LoadTexture(path);
        index++;

        free(path);
    }
    
    //Sort array by ID for binary search
    qsort(sceneArray, sceneCount, sizeof(struct sceneData), compare_scenes);
    return 0;
}

int get_scene_texture(int target_id, Texture* tex) {
    //Initialize on first call
    if (!sceneArray) {
        FILE* fptr = fopen("sceneIDLookup.txt", "r");
        if (fptr == NULL) {
            printf("Error opening file\n");
            return 0;
        }
        
        if (init_scene_array(fptr)) {
            fclose(fptr);
            return 0;
        }
        
        fclose(fptr);
    }
    
    //Perform binary search
    struct sceneData key = { .id = target_id };
    key = *((struct sceneData *)bsearch(&key, sceneArray, sceneCount, sizeof(struct sceneData), compare_scenes));
    *tex = key.tex;
    
    return 1;
}

//Don't forget to add cleanup function
void cleanup_scene_data(void) {
    if (sceneArray) {
        for (int i = 0; i < sceneCount; i++) {
            UnloadTexture(sceneArray[i].tex);
        }
        free(sceneArray);
        sceneArray = NULL;
        sceneCount = 0;
    }
}