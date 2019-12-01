#include "Renderer.h"

#include <iostream>
#include <memory>
#include <random>

#include "Canvas.h"
#include "Scene.h"
#include "Primitives.h"
#include "Face.h"
#include "Texture.h"

#include <Eigen/core>
#include <Eigen/dense>

Renderer::Renderer(Canvas &canvas) : cvs(canvas), pmts(canvas) {
}

void Renderer::render_scene(Scene &scene) {
    const auto main_camera = scene.get_camera();
    const Eigen::Matrix4f camera_transform = main_camera->get_trans_mtx();

    for (auto &i : scene.get_instances()) {
        const Eigen::Matrix4f transform = camera_transform * i->get_trans_mtx();
        const auto to_render = transform_and_clip(i, transform);
        render_instance(to_render, main_camera);
    }
}

void Renderer::render_triangle_with_texture(const Eigen::Vector3i &v_idx,
        std::vector<Eigen::Vector2i> &projected, std::vector<float> &zs,
        const Eigen::Vector2f &tex0, const Eigen::Vector2f &tex1, const Eigen::Vector2f &tex2, Texture texture) {
    pmts.draw_texture(
            projected[v_idx[0]], 1.0f / zs[v_idx[0]], tex0,
            projected[v_idx[1]], 1.0f / zs[v_idx[1]], tex1,
            projected[v_idx[2]], 1.0f / zs[v_idx[2]], tex2,
            texture
    );
}

void Renderer::render_instance(const Model &inst, const std::shared_ptr<Camera> camera) {
    std::vector<Eigen::Vector2i> projected;
    std::vector<float> zs;

    for (const auto &v : inst.vertexes) {
        projected.push_back(cvs.project_vertex({v.x(), v.y(), v.z()}));
        zs.push_back(v.z());
    }

    for (const auto &face : inst.faces) {
        const Eigen::Vector3i &v_idx = face.vertex_idx;
        const Eigen::Vector3i &t_idx = face.texcoord_idx;
        // backface culling
        Eigen::Vector3f v01 = inst.vertexes[v_idx[1]] - inst.vertexes[v_idx[0]];
        Eigen::Vector3f v02 = inst.vertexes[v_idx[2]] - inst.vertexes[v_idx[0]];
        Eigen::Vector3f normal = v01.cross(v02);
        Eigen::Vector3f centre = 1.0f/3.0f *
            (inst.vertexes[v_idx[0]] + inst.vertexes[v_idx[1]] + inst.vertexes[v_idx[2]]);
        Eigen::Vector3f v = -1.0f * centre;

        if (normal.dot(v) < 0) {
            continue;
        }

        //render_triangle(v_idx, projected, zs);
        render_triangle_with_texture(v_idx, projected, zs,
                inst.texels[t_idx[0]], 
                inst.texels[t_idx[1]],
                inst.texels[t_idx[2]],
                inst.texture
                );
    }
}


Model Renderer::transform_and_clip(const std::shared_ptr<Model> m, const Eigen::Matrix4f transform) {
    std::vector<Eigen::Vector3f> vertexes;
    std::vector<Face> faces;

    for (const auto &v : m->vertexes) {
        Eigen::Vector4f v_extended{v.x(), v.y(), v.z(), 1};

        vertexes.push_back((transform * v_extended).head(3));
    }

    // clipping
    for (const auto &face : m->faces) {
        if (!((vertexes[face.vertex_idx(0)].z() < cvs.get_viewport_distance()) || 
              (vertexes[face.vertex_idx(1)].z() < cvs.get_viewport_distance()) ||
              (vertexes[face.vertex_idx(2)].z() < cvs.get_viewport_distance()))) {
            Face n_face;
            n_face.vertex_idx = Eigen::Vector3i(face.vertex_idx);
            n_face.texcoord_idx = Eigen::Vector3i(face.texcoord_idx);
            faces.push_back(n_face);
        }
    }

    return Model{vertexes, m->texels, faces, m->texture};
}

void Renderer::render_triangle(Eigen::Vector3i triangle,
        std::vector<Eigen::Vector2i> &projected, std::vector<float> &zs) {


    pmts.draw_filled_triangle(projected[triangle[0]],
                                 projected[triangle[1]],
                                 projected[triangle[2]],
                                 1 / zs[triangle[0]],
                                 1 / zs[triangle[1]],
                                 1 / zs[triangle[2]],
                                 {195, 195, 195}
    );
}
