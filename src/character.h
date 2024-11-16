#ifndef CHARACTER_H
#define CHARACTER_H

//Structure to hold ID-path pairs
struct FrameData {
    int id;
    char* path;
};

char* getCharacterPath(int target_id);
void cleanup_frame_data(void);

#endif