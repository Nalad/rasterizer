#include "Texture.h"

#include "SDL.h"

Texture::Texture() : texture(nullptr){
}

Texture::Texture(const std::string &s) {
    auto text_ptr = SDL_LoadBMP(s.c_str());
    texture = std::shared_ptr<SDL_Surface>(text_ptr);
};
