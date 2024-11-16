#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure to hold ID-path pairs
struct FrameData {
    int id;
    char* path;
};

//Global array to store frame data
static struct FrameData* frameArray = NULL;
static int frameCount = 0;

//Compare function for binary search
static int compare_frames(const void* a, const void* b) {
    return ((struct FrameData*)a)->id - ((struct FrameData*)b)->id;
}

//Initialize the frame data array
static int init_frame_array(FILE* fptr) {
    char line[69];
    frameCount = 0;
    
    //First count valid lines
    while (fgets(line, sizeof(line), fptr)) {
        if (line[0] == '<' && !(line[0] == '/' && line[1] == '/')) {
            frameCount++;
        }
    }
    
    //Allocate array
    frameArray = malloc(frameCount * sizeof(struct FrameData));
    if (!frameArray) return 1;
    
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
        frameArray[index].id = atoi(id_str);
        frameArray[index].path = path;
        index++;
    }
    
    //Sort array by ID for binary search
    qsort(frameArray, frameCount, sizeof(struct FrameData), compare_frames);
    return 0;
}

char* getPath(int target_id) {
    static int initialized = 0;
    
    //Initialize on first call
    if (!initialized) {
        FILE* fptr = fopen("../ass/frameIDLookup.txt", "r");
        if (fptr == NULL) {
            printf("Error opening file\n");
            return NULL;
        }
        
        if (init_frame_array(fptr)) {
            fclose(fptr);
            return NULL;
        }
        
        fclose(fptr);
        initialized = 1;
    }
    
    //Perform binary search
    struct FrameData key = { .id = target_id };
    struct FrameData* result = bsearch(&key, frameArray, frameCount, sizeof(struct FrameData), compare_frames);
    
    if (result) {
        return strdup(result->path);  //Return copy of path
    }
    
    return NULL;
}

//Don't forget to add cleanup function
void cleanup_frame_data(void) {
    if (frameArray) {
        for (int i = 0; i < frameCount; i++) {
            free(frameArray[i].path);
        }
        free(frameArray);
        frameArray = NULL;
        frameCount = 0;
    }
}