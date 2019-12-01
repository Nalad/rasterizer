#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <Eigen/core>
#include <Eigen/LU>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Transform.h"

#include "SDL.h"

class Camera {
    public:
        Camera() = delete;
        Camera(Eigen::Vector3f pos, Eigen::Vector3f rot);

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

        Eigen::Matrix4f get_trans_mtx() const {
            return transform.rot_mtx().inverse() * transform.pos_mtx().inverse();
        }

        void Camera::handleEvent(SDL_Event &e);

    private:
        Transform transform;
};

#endif
