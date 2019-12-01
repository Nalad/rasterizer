#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.h"
#include "Model.h"
#include "Primitives.h"
#include "Camera.h"

#include <memory>
#include "Texture.h"

#include <Eigen/core>

class Renderer {
    public:
        Renderer(Canvas &);

        void render_scene(Scene &);

    private:
        Canvas &cvs;
        Primitives pmts;

        void render_instance(const Model &, std::shared_ptr<Camera> camera);
        void render_triangle(Eigen::Vector3i, std::vector<Eigen::Vector2i> &,
                std::vector<float> &zs);
        Model transform_and_clip(const std::shared_ptr<Model> m,
                const Eigen::Matrix4f transform);
        void render_triangle_with_texture(const Eigen::Vector3i &v_idx,
                std::vector<Eigen::Vector2i> &projected, std::vector<float> &zs,
                const Eigen::Vector2f &tex0,
                const Eigen::Vector2f &tex1,
                const Eigen::Vector2f &tex2,
                Texture texture);
};
#endif
