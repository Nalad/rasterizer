#include "Canvas.h"

#include <fstream>
#include <iostream>
#include <new>
#include <string>
#include <vector>

#include <Eigen/Core>

Canvas::Canvas(int x, int y, float vp_w, float vp_h, float vp_d) : screen_width(x),
    screen_height(y), cvs(new Eigen::Vector3i[x * y]),
    depth_buffer(std::vector<float>(x * y, 0)),
    viewport_width(vp_w), viewport_height(vp_h), viewport_distance(vp_d) {
}

Canvas::Canvas(const Canvas &orig) : screen_width(orig.screen_width),
    screen_height(orig.screen_height),
    cvs(new Eigen::Vector3i[screen_width * screen_height]) {
    for (int i = 0; i < screen_width; i++) {
        for (int j = 0; j < screen_height; j++) {
            cvs[idx(i, j)] = orig.cvs[idx(i, j)];
        }
    }
}

Canvas& Canvas::operator=(const Canvas &rhs) {
    auto new_cvs = new Eigen::Vector3i[screen_width * screen_height];
    for (int i = 0; i < screen_width; i++) {
        for (int j = 0; j < screen_height; j++) {
            new_cvs[idx(i, j)] = rhs.cvs[idx(i, j)];
        }
    }
    delete [] cvs;

    cvs = new_cvs;
    screen_width = rhs.screen_width;
    screen_height = rhs.screen_height;

    return *this;
}

Canvas::~Canvas() {
    delete [] cvs;
}

void Canvas::put_pixel(Eigen::Vector2i point, Eigen::Vector3i color) {
    if ((-(screen_width / 2) <= point.x() && point.x() < (screen_width / 2)) &&
        (-(screen_height / 2) <= point.y() && point.y() < (screen_height / 2))) {
        cvs[idx(point.x() + (screen_width / 2), point.y() + (screen_height / 2))] = color;
    }
}

void Canvas::set_depth_buffer(Eigen::Vector2i point, float z) {
    if ((-(screen_width / 2) <= point.x() && point.x() < (screen_width / 2)) &&
        (-(screen_height / 2) <= point.y() && point.y() < (screen_height / 2))) {
        depth_buffer[idx(point.x() + (screen_width / 2), point.y() + (screen_height / 2))] = z;
    }
}

float Canvas::get_depth_buffer(Eigen::Vector2i point) {
    if ((-(screen_width / 2) <= point.x() && point.x() < (screen_width / 2)) &&
        (-(screen_height / 2) <= point.y() && point.y() < (screen_height / 2))) {
        return depth_buffer[idx(
                point.x() + (screen_width / 2),
                point.y() + (screen_height / 2))];
    }
    return 0;
}

void Canvas::display(const std::string &filename) {
    std::ofstream img(filename, std::ofstream::binary);

    img << "P3\n" << screen_width << " " << screen_height << "\n255\n";
    for (auto j = screen_height - 1; j >= 0; j--) {
        for (auto i = 0; i < screen_width; i++) {
            auto color = cvs[idx(i, j)];
            img << color(0) << " " << color(1) << " " << color(2) << "\n";
        }
    }

    img.close();
}

void Canvas::display(SDL_Surface *surface) {
    // DO NOT TOUCH IT
    for (auto y = screen_height - 1; y >= 0; --y) {
        for (auto x = screen_width - 1; x >= 0; --x) { 
            auto color = cvs[idx(x, screen_height - 1 - y)];

            Uint8 *pixel = (Uint8 *) surface->pixels;
            pixel += (y * surface->pitch) + (x * sizeof(Uint32));
            *((Uint32 *) pixel) = (color(0) << 16) | (color(1) << 8) | (color(2));
        }
    }
}

Eigen::Vector2i Canvas::project_vertex(Eigen::Vector3f v) {
    auto vtc = viewport_to_canvas({
            v.x() * viewport_distance / v.z(), 
            v.y() * viewport_distance / v.z()
            });
    return {static_cast<int>(vtc.x()), static_cast<int>(vtc.y())};
}

Eigen::Vector2f Canvas::viewport_to_canvas(Eigen::Vector2f v) {
    auto width_ratio = screen_width / viewport_width;
    auto height_ratio = screen_height / viewport_height;
    return {v.x() * width_ratio, v.y() * height_ratio};
}

void Canvas::clear() {
    for (auto i = -screen_width / 2; i < screen_width / 2; ++i) {
        for (auto j = -screen_height / 2; j < screen_height / 2; ++j) {
            put_pixel({i, j}, {255, 255, 255});
        }
    }
    depth_buffer = std::vector<float>(screen_width * screen_height, 0);
}
