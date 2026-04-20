#include "display.h"

SDL_Renderer *renderer = nullptr;
SDL_Texture *texture = nullptr;

bool init_display(SDL_Window *window) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer not there: " << SDL_GetError() << "\n";
        return false;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    if (!texture) {
        std::cerr << "Texture not there: " << SDL_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
        return false;
    }

    return true;
}

void render_display(const uint32_t frame_buffer[HEIGHT][WIDTH]) {
    if (!renderer || !texture) {
        return;
    }

    SDL_UpdateTexture(texture, nullptr, frame_buffer, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void destroy_display() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}
