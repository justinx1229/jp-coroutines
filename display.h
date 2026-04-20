
#pragma once

#include "consts.h"

bool init_display(SDL_Window *window);
void render_display(const uint32_t frame_buffer[HEIGHT][WIDTH]);
void destroy_display();
