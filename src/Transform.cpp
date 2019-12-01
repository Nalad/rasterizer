#include "Transform.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <Eigen/core>

Transform::Transform() : position({0, 0, 0}), rotation({0, 0, 0}), 
    scale({1.0f, 1.0f, 1.0f}) {
}

Transform::Transform(Eigen::Vector3f pos, Eigen::Vector3f rot, Eigen::Vector3f sc) : 
    position(pos), rotation(rot), scale(sc) {
}

Eigen::Matrix4f Transform::pos_mtx() const {
    Eigen::Matrix4f pos;
    pos <<
        1.0, 0.0, 0.0, position.x(),
        0.0, 1.0, 0.0, position.y(),
        0.0, 0.0, 1.0, position.z(),
        0.0, 0.0, 0.0, 1.0;

    return pos;
}

Eigen::Matrix4f Transform::rot_mtx() const {
    Eigen::Vector3f rot = rotation * M_PI / 180.0;

    Eigen::Matrix4f x_rot;
    x_rot << 
        1.0, 0.0, 0.0, 0.0,
        0, cos(rot.x()), -1 * sin(rot.x()), 0,
        0, sin(rot.x()), cos(rot.x()), 0,
        0.0, 0.0, 0.0, 1.0;

    Eigen::Matrix4f y_rot;
    y_rot << 
        cos(rot.y()), 0, sin(rot.y()), 0,
        0, 1, 0, 0,
        -1 * sin(rot.y()), 0, cos(rot.y()), 0,
        0, 0, 0, 1;

    Eigen::Matrix4f z_rot;
    z_rot <<
        cos(rot.z()), -1 * sin(rot.z()), 0, 0,
        sin(rot.z()), cos(rot.z()), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;

    return x_rot * y_rot * z_rot;
}

Eigen::Matrix4f Transform::sc_mtx() const {
    Eigen::Matrix4f sc;
    sc <<
        scale.x(), 0, 0, 0,
        0, scale.y(), 0, 0,
        0, 0, scale.z(), 0,
        0, 0, 0, 1;

    return sc;
}
