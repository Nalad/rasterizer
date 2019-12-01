#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>

#include <Eigen/Core>
#include "SDL.h"

#include "Canvas.h"
#include "Primitives.h"
#include "Renderer.h"
#include "Face.h"
#include "Model.h"

#define screen_width 1024
#define screen_height 768

int main(int argc, char *args[]) {

    SDL_Window *window = nullptr;
    SDL_Surface *screen_surface = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError();
    } else {
        window = SDL_CreateWindow("rasterizer", SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError();
        } else {
            screen_surface = SDL_GetWindowSurface(window);
        }
    }

    Canvas canvas(screen_width, screen_height, 8.0f, 6.0f, 10.0f);

    auto camera = std::make_shared<Camera>(
        Eigen::Vector3f{0, 0, 0},
        Eigen::Vector3f{0, 0, 0}
    );
    Scene scene{camera};

    auto obj = std::make_shared<Model>(Model::load_obj_into_model("watch_tower.obj"));
    Texture texture("watch_tower_texture.bmp");
    obj->texture = texture;
    obj->set_position({0, -3, 20});

    scene.add_instance(obj);

    Renderer renderer{canvas};
    Eigen::Vector3f current_rot{-30.0f, 180.0f, 0};
    auto quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            camera->handleEvent(e);
        }

        canvas.clear();

        current_rot += Eigen::Vector3f{0, 1.0f, 0};
        obj->set_rotation(current_rot);

        renderer.render_scene(scene);
        canvas.display(screen_surface);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
