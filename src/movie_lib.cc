// NOTE: This module is completely standalone. It does not have external
// dependencies and uses __cdecl calling convention, which probably means it
// was implemented as a separate library and linked statically.

#include "movie_lib.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "audio_engine.h"
#include "platform_compat.h"

namespace fallout {

// Creates a movie surface whose pitch exactly equals its row width. The MVE
// decoder addresses pixels as `row * _mveBW`, so any pitch padding added by
// SDL (which can happen on some ports, including PS4) corrupts the frame data
// into blocky garbage. We allocate the backing buffer ourselves so the row
// stride is always exact and the decoder can safely read contiguous rows.
static SDL_Surface* movieCreatePackedSurface(int width, int height, int depth, int rmask, int gmask, int bmask)
{
    const int bytesPerPixel = depth / 8;
    const int pitch = width * bytesPerPixel;
    void* pixels = calloc((size_t)pitch * height + 64, 1);
    if (pixels == NULL) {
        return NULL;
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch, rmask, gmask, bmask, 0);
    if (surface == NULL) {
        free(pixels);
        return NULL;
    }

    // SDL may otherwise re-align or pad the pitch on some platforms. Since the
    // MVE decoder assumes contiguous rows, force the exact packed stride we
    // allocated for.
    surface->pitch = pitch;
    surface->userdata = pixels;
    return surface;
}

static void movieFreePackedSurface(SDL_Surface* surface)
{
    if (surface != NULL) {
        void* pixels = surface->userdata;
        SDL_FreeSurface(surface);
        free(pixels);
    }
}
