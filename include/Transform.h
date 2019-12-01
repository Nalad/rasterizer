#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <Eigen/core>

class Transform {
    public:
        Transform();
        Transform(Eigen::Vector3f pos, Eigen::Vector3f rot, Eigen::Vector3f sc);

        Eigen::Vector3f position;
        Eigen::Vector3f rotation;
        Eigen::Vector3f scale;

        Eigen::Matrix4f pos_mtx() const;
        Eigen::Matrix4f rot_mtx() const;
        Eigen::Matrix4f sc_mtx() const;
};

#endif
