#ifndef CANVAS_H
#define CANVAS_H

#include <string>
#include <vector>

#include <Eigen/Core>

#include "SDL.h"

class Canvas {
    public:
        Canvas() = delete;
        Canvas(int, int, float, float, float);
        Canvas(const Canvas&);
        Canvas &operator=(const Canvas&);
        ~Canvas();

        void put_pixel(Eigen::Vector2i, Eigen::Vector3i);
        float get_depth_buffer(Eigen::Vector2i);
        void set_depth_buffer(Eigen::Vector2i, float z);
        void display(SDL_Surface *surface);
        void display(const std::string &);
        void clear();
        Eigen::Vector2i project_vertex(Eigen::Vector3f);

        float get_viewport_width() {
            return viewport_width;
        }

        float get_viewport_height() {
            return viewport_height;
        }

        float get_viewport_distance() {
            return viewport_distance;
        }

        std::size_t idx(int x, int y) const {
            return (screen_width - x - 1) * screen_height + y;
        }


    private:
        int screen_width;
        int screen_height;
        float viewport_width;
        float viewport_height;
        float viewport_distance;
        Eigen::Vector3i *cvs;
        std::vector<float> depth_buffer;

        Eigen::Vector2f viewport_to_canvas(Eigen::Vector2f);
};

#endif
