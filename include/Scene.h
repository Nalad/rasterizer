#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "Model.h"
#include "Camera.h"

class Scene {
    public:
        Scene() = delete;
        Scene(std::shared_ptr<Camera> c);

        void add_instance(std::shared_ptr<Model> m) {
            instances.push_back(m);
        }

        std::vector<std::shared_ptr<Model>> &get_instances() {
            return instances;
        }

        std::shared_ptr<Camera> get_camera() {
            return camera;
        }


    private:
        std::shared_ptr<Camera> camera;
        std::vector<std::shared_ptr<Model>> instances;
};

#endif
