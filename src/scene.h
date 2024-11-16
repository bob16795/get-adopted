#ifndef SCENE_H
#define SCENE_H

//Structure to hold ID-path pairs
struct sceneData {
    int id;
    Texture tex;
};

int get_scene_texture(int target_id, Texture* tex);
void cleanup_scene_data(void);

#endif