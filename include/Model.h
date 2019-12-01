#ifndef MODEL_H
#define MODEL_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <vector>

#include <math.h>

#include <Eigen/core>

#include "Transform.h"
#include "Face.h"
#include "Texture.h"

class Model {
    public:
        const std::vector<Eigen::Vector3f> vertexes;
        const std::vector<Eigen::Vector3i> triangles;
        const std::vector<Eigen::Vector2f> texels;
        const std::vector<Face> faces;
        Texture texture;

        Model() = delete;
        Model(std::vector<Eigen::Vector3f> vertexes, std::vector<Eigen::Vector2f> texels,
                std::vector<Face> faces);
        Model(std::vector<Eigen::Vector3f> vertexes, std::vector<Eigen::Vector2f> texels,
                std::vector<Face> faces, Texture texture);

        Eigen::Vector3f get_position() const {
            return transform.position;
        }

        void set_position(Eigen::Vector3f pos) {
            transform.position = pos;
        }

        Eigen::Vector3f get_rotation() const {
            return transform.rotation;
        }

        void set_rotation(Eigen::Vector3f rot) {
            transform.rotation = rot;
        }

        Eigen::Vector3f get_scale() const {
            return transform.scale;
        }

        void set_scale(Eigen::Vector3f sc) {
            transform.scale = sc;
        }

        Eigen::Matrix4f get_trans_mtx() const {
            return transform.pos_mtx() * transform.rot_mtx() * transform.sc_mtx();
        }

        static Model load_obj_into_model(const std::string& filename);


    private:
        Transform transform;
};


#endif
