#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"
#include <memory>
#include <string>

class Texture {
    public:
        std::shared_ptr<SDL_Surface> texture; 

        Texture();
        Texture(const std::string &s);
};

#endif
