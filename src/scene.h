#ifndef SCENE_H
#define SCENE_H

//Structure to hold ID-path pairs
struct sceneData {
    int id;
    char* path;
};

char* getScenePath(int target_id);
void cleanup_scene_data(void);

#endif