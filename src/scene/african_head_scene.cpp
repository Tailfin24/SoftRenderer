#include "scene_list.h"

AfricanHeadScene::AfricanHeadScene(int width, int height) : Scene(width, height) {
    // 添加模型
    models.push_back(new Model("obj\\african_head\\african_head.obj"));
}
