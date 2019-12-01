#include "Camera.h"

#include <Eigen/core>
#include <iostream>

#include "SDL.h"

Camera::Camera(Eigen::Vector3f pos, Eigen::Vector3f rot) : 
    transform(pos, rot, {1.0f, 1.0f, 1.0f}) {
}

void Camera::handleEvent(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                transform.position += Eigen::Vector3f{0, 0, 0.5f};
                break;
            case SDLK_s:
                transform.position -= Eigen::Vector3f{0, 0, 0.5f};
                break;
            case SDLK_d:
                transform.position += Eigen::Vector3f{0.5f, 0, 0};
                break;
            case SDLK_a:
                transform.position -= Eigen::Vector3f{0.5f, 0, 0};
                break;
            case SDLK_q:
                transform.position += Eigen::Vector3f{0, 0.5f, 0};
                break;
            case SDLK_e:
                transform.position -= Eigen::Vector3f{0, 0.5f, 0};
                break;
            case SDLK_UP:
                transform.rotation -= Eigen::Vector3f{0.5f, 0, 0};
                break;
            case SDLK_DOWN:
                transform.rotation += Eigen::Vector3f{0.5f, 0, 0};
                break;
            case SDLK_LEFT:
                transform.rotation -= Eigen::Vector3f{0, 0.5f, 0};
                break;
            case SDLK_RIGHT:
                transform.rotation += Eigen::Vector3f{0, 0.5f, 0};
                break;
        }
    }
}
