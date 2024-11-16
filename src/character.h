#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include <stddef.h>

//Structure to hold ID-path pairs
struct FrameData {
    int id;
    Texture tex;
};

//Global array to store frame data
static struct FrameData* frameArray;
static int frameCount;

int get_texture(int target_id, Texture* tex);
void cleanup_frame_data(void);

#endif