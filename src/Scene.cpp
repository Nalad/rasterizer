#include "Scene.h"
#include "Camera.h"

#include <memory>

Scene::Scene(std::shared_ptr<Camera> c) : camera(c) {
}
