#include "Platform.h"
#include <SDL3/SDL.h>

/**
 * Comprueba colisión con la plataforma
 * Devuelve PLATFORM si hay colisión
 */
Collision Platform::checkCollision(const SDL_FRect& rect)
{
    Collision col;
    col.tipo = Collision::NONE;

    // Caja de colisión ajustada
    SDL_FRect platformRect = {
        position.getX(),
        position.getY(),
        width,
        height
    };

    if (SDL_HasRectIntersectionFloat(&platformRect, &rect))
    {
        col.tipo = Collision::PLATFORM;
        col.vel = velocity;
    }

    return col;
}