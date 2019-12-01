#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <vector>
#include <Eigen/Core>
#include "Texture.h"
#include "SDL.h"

class Canvas;

class Primitives {
    public:
        Primitives(Canvas &);

        void draw_line(Eigen::Vector2i, Eigen::Vector2i, Eigen::Vector3i);
        void draw_wireframe_triangle(Eigen::Vector2i, Eigen::Vector2i, Eigen::Vector2i,
                Eigen::Vector3i);
        void draw_filled_triangle(Eigen::Vector2i, Eigen::Vector2i, Eigen::Vector2i,
                float z0, float z1, float z2,
                Eigen::Vector3i);
        void Primitives::draw_texture(Eigen::Vector2i p0, float z0, Eigen::Vector2f t0, 
                Eigen::Vector2i p1, float z1, Eigen::Vector2f t1,
                Eigen::Vector2i p2, float z2, Eigen::Vector2f t2, Texture texture);

    private:
        Canvas &cvs;

        std::vector<int> interpolate(int, int, int, int);
        std::vector<float> interpolate(int, float, int, float);
        Uint32 get_pixel(std::shared_ptr<SDL_Surface> surface, int x, int y);
};

#endif
