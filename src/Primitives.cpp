#include "Primitives.h"

#include <vector>
#include <iostream>
#include <Eigen/Core>

#include "Canvas.h"
#include "Texture.h"
#include "SDL.h"

Primitives::Primitives(Canvas &canvas) : cvs(canvas) {
}

void Primitives::draw_line(Eigen::Vector2i p0, Eigen::Vector2i p1, Eigen::Vector3i color) {
    using std::swap;

    if (std::abs(p1.x() - p0.x()) > std::abs(p1.y() - p0.y())) {
        if (p0.x() > p1.x()) {
            swap(p0, p1);
        }

        auto ys = interpolate(p0.x(), p0.y(), p1.x(), p1.y());
        for (auto x = p0.x(); x <= p1.x(); ++x) {
            cvs.put_pixel({x, ys[x - p0.x()]}, color); 
        }
    } else {
        if (p0.y() > p1.y()) {
            swap(p0, p1);
        }

        auto xs = interpolate(p0.y(), p0.x(), p1.y(), p1.x());
        for (auto y = p0.y(); y <= p1.y(); ++y) {
            cvs.put_pixel({xs[y - p0.y()], y}, color); 
        }
    }
}

Uint32 Primitives::get_pixel(std::shared_ptr<SDL_Surface> surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void Primitives::draw_wireframe_triangle(Eigen::Vector2i p0, Eigen::Vector2i p1,
        Eigen::Vector2i p2, Eigen::Vector3i color) {
    draw_line(p0, p1, color);
    draw_line(p1, p2, color);
    draw_line(p2, p0, color);
}


void Primitives::draw_texture(Eigen::Vector2i p0, float z0, Eigen::Vector2f t0, 
        Eigen::Vector2i p1, float z1, Eigen::Vector2f t1,
        Eigen::Vector2i p2, float z2, Eigen::Vector2f t2, Texture texture) {
    using std::swap;
    if (p1.y() < p0.y()) {
        swap(p1, p0);
        swap(z1, z0);
        swap(t1, t0);
    }
    if (p2.y() < p0.y()) {
        swap(p2, p0);
        swap(z2, z0);
        swap(t2, t0);
    }
    if (p2.y() < p1.y()) {
        swap(p2, p1);
        swap(z2, z1);
        swap(t2, t1);
    }

    
    auto x01 = interpolate(p0.y(), p0.x(), p1.y(), p1.x());
    auto z01 = interpolate(p0.y(), z0, p1.y(), z1);
    auto t010 = interpolate(p0.y(), t0(0) * z0, p1.y(), t1(0) * z1);
    auto t011 = interpolate(p0.y(), t0(1) * z0, p1.y(), t1(1) * z1);

    auto x12 = interpolate(p1.y(), p1.x(), p2.y(), p2.x());
    auto z12 = interpolate(p1.y(), z1, p2.y(), z2);
    auto t120 = interpolate(p1.y(), t1(0) * z1, p2.y(), t2(0) * z2);
    auto t121 = interpolate(p1.y(), t1(1) * z1, p2.y(), t2(1) * z2);

    auto x02 = interpolate(p0.y(), p0.x(), p2.y(), p2.x());
    auto z02 = interpolate(p0.y(), z0, p2.y(), z2);
    auto t020 = interpolate(p0.y(), t0(0) * z0, p2.y(), t2(0) * z2);
    auto t021 = interpolate(p0.y(), t0(1) * z0, p2.y(), t2(1) * z2);

    std::vector<int> x012 = x01;
    x012.insert(x012.end(), x12.cbegin(), x12.cend());
    std::vector<float> z012 = z01;
    z012.insert(z012.end(), z12.cbegin(), z12.cend());
    std::vector<float> t0120 = t010;
    t0120.insert(t0120.end(), t120.cbegin(), t120.cend());
    std::vector<float> t0121 = t011;
    t0121.insert(t0121.end(), t121.cbegin(), t121.cend());
    
    auto x_left = x02;
    auto z_left = z02;
    auto t0_left = t020;
    auto t1_left = t021;
    auto x_right = x012;
    auto z_right = z012;
    auto t0_right = t0120;
    auto t1_right = t0121;

    auto m = x02.size() / 2;
    if (x012[m] < x02[m]) {
        std::swap(x_left, x_right);
        std::swap(z_left, z_right);
        std::swap(t0_left, t0_right);
        std::swap(t1_left, t1_right);
    }

    auto w = texture.texture->w;
    auto h = texture.texture->h;
    for (auto y = p0.y(); y < p2.y(); ++y) {
        auto x_1 = x_left[y - p0.y()];
        auto x_r = x_right[y - p0.y()];

        auto zs = interpolate(x_1, z_left[y - p0.y()], x_r, z_right[y - p0.y()]);
        auto t0s = interpolate(x_1, t0_left[y - p0.y()], x_r, t0_right[y - p0.y()]);
        auto t1s = interpolate(x_1, t1_left[y - p0.y()], x_r, t1_right[y - p0.y()]);
        for (auto x = x_1; x < x_r; ++x) {
            auto z = zs[x - x_1];
            auto u = t0s[x - x_1];
            auto v = t1s[x - x_1];

            SDL_LockSurface(texture.texture.get());
            // mini hack because the orientation is wrong
            Uint32 pixel = get_pixel(texture.texture, static_cast<int>(u / z * (w - 1)), static_cast<int>((h - 1) - v / z * (h - 1)));
            SDL_UnlockSurface(texture.texture.get());

            if (z > cvs.get_depth_buffer({x, y})) {
                cvs.put_pixel({x, y}, {
                        (pixel >> 16) & 0xFF,
                        (pixel >> 8) & 0xFF,
                        (pixel >> 0) & 0xFF
                        
                });
                cvs.set_depth_buffer({x, y}, z);
            }
         }
     }
}


void Primitives::draw_filled_triangle(Eigen::Vector2i p0, Eigen::Vector2i p1,
        Eigen::Vector2i p2, float z0, float z1, float z2, Eigen::Vector3i color) {
    using std::swap;
    if (p1.y() < p0.y()) {
        swap(p1, p0);
        swap(z1, z0);
    }
    if (p2.y() < p0.y()) {
        swap(p2, p0);
        swap(z2, z0);
    }
    if (p2.y() < p1.y()) {
        swap(p2, p1);
        swap(z2, z1);
    }

    auto x01 = interpolate(p0.y(), p0.x(), p1.y(), p1.x());
    auto z01 = interpolate(p0.y(), z0, p1.y(), z1);
    auto x12 = interpolate(p1.y(), p1.x(), p2.y(), p2.x());
    auto z12 = interpolate(p1.y(), z1, p2.y(), z2);
    auto x02 = interpolate(p0.y(), p0.x(), p2.y(), p2.x());
    auto z02 = interpolate(p0.y(), z0, p2.y(), z2);

    std::vector<int> x012 = x01;
    x012.insert(x012.end(), x12.cbegin(), x12.cend());
    std::vector<float> z012 = z01;
    z012.insert(z012.end(), z12.cbegin(), z12.cend());

    auto x_left = x02;
    auto z_left = z02;
    auto x_right = x012;
    auto z_right = z012;
    auto m = x02.size() / 2;
    if (x012[m] < x02[m]) {
        std::swap(x_left, x_right);
        std::swap(z_left, z_right);
    }

    for (auto y = p0.y(); y < p2.y(); ++y) {
        auto x_1 = x_left[y - p0.y()];
        auto x_r = x_right[y - p0.y()];

        auto zs = interpolate(x_1, z_left[y - p0.y()], x_r, z_right[y - p0.y()]);
        for (auto x = x_1; x < x_r; ++x) {
            auto z = zs[x - x_1];

            if (z > cvs.get_depth_buffer({x, y})) {
                cvs.put_pixel({x, y}, color);
                cvs.set_depth_buffer({x, y}, z);
            }
         }
    }
}

std::vector<int> Primitives::interpolate(int i0, int d0, int i1, int d1) {
    if (i0 == i1) {
        return { d0 };
    }

    std::vector<int> values;

    auto a = static_cast<float>(d1 - d0) / (i1 - i0);
    float d = static_cast<float>(d0);
    for (auto i = i0; i <= i1; ++i) {
        values.push_back(static_cast<int>(d));
        d = d + a;
    }

    return values;
}


std::vector<float> Primitives::interpolate(int i0, float d0, int i1, float d1) {
    if (i0 == i1) {
        return { d0 };
    }

    std::vector<float> values;

    auto a = (d1 - d0) / (i1 - i0);
    auto d = d0;
    for (auto i = i0; i <= i1; ++i) {
        values.push_back(d);
        d = d + a;
    }

    return values;
}
