#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include <stddef.h>

//Structure to hold ID-path pairs
struct FrameData {
    int id;
    Texture2D tex;  //Changed from Texture to Texture2D
};

//Global array to store frame data
//Remove static and use extern for global variables in header
extern struct FrameData* frameArray;
extern int frameCount;

int get_character_texture(int target_id, Texture2D* tex);  //Changed from Texture* to Texture2D*
void cleanup_frame_data(void);

#endif