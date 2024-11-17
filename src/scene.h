#ifndef SCENE_H
#define SCENE_H

#include "raylib.h"

// Structure to hold ID-texture pairs
struct sceneData {
    int id;
    Texture2D tex;
};

// Global array to store scene data
extern struct sceneData* sceneArray;
extern int sceneCount;

// Function declarations
int get_scene_texture(int target_id, Texture2D* tex);
void cleanup_scene_data(void);

#endif