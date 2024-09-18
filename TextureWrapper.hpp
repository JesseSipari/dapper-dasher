// TextureWrapper.hpp
#ifndef TEXTUREWRAPPER_HPP
#define TEXTUREWRAPPER_HPP

#include "raylib.h"
#include <stdexcept>
#include <string>
#include <iostream>

class TextureWrapper {
public:
    // Constructor: Load the texture
    TextureWrapper(const std::string& filePath) {
        texture = LoadTexture(filePath.c_str());
        if (texture.id == 0) {
            throw std::runtime_error("Failed to load texture: " + filePath);
        }
        else {
            std::cout << "INFO: FILEIO: [" << filePath << "] File loaded successfully\n";
            std::cout << "INFO: IMAGE: Data loaded successfully ("
                << texture.width << "x" << texture.height
                << " | R" << ((texture.format >> 16) & 0xFF)
                << "G" << ((texture.format >> 8) & 0xFF)
                << "B" << (texture.format & 0xFF)
                << " | " << texture.mipmaps << " mipmaps)\n";
        }
    }

    // Destructor: Unload the texture
    ~TextureWrapper() {
        if (texture.id != 0) {
            UnloadTexture(texture);
        }
    }

    // Delete copy constructor and copy assignment to prevent copying
    TextureWrapper(const TextureWrapper&) = delete;
    TextureWrapper& operator=(const TextureWrapper&) = delete;

    // Allow move semantics
    TextureWrapper(TextureWrapper&& other) noexcept : texture(other.texture) {
        other.texture.id = 0; // Prevent double unloading
    }

    TextureWrapper& operator=(TextureWrapper&& other) noexcept {
        if (this != &other) {
            if (texture.id != 0) {
                UnloadTexture(texture);
            }
            texture = other.texture;
            other.texture.id = 0;
        }
        return *this;
    }

    // Accessor
    Texture2D get() const { return texture; }

private:
    Texture2D texture;
};

#endif // TEXTUREWRAPPER_HPP
