// HomedFrog.cpp
#include "HomedFrog.h"
#include <SDL3/SDL.h>

/**
 * Constructor
 */
HomedFrog::HomedFrog(Game* game, Texture* texture, Point2D pos)
    : SceneObject(game, texture, pos,
        texture->getFrameWidth(),
        texture->getFrameHeight()),
    isActive(false)
{
}

/**
 * Renderiza solo si el nido está ocupado
 */
void HomedFrog::render() const
{
    if (isActive)
    {
        SDL_FRect destRect = {
            position.getX(),
            position.getY(),
            static_cast<float>(width),
            static_cast<float>(height)
        };
        texture->renderFrame(destRect, 0, 0, SDL_FLIP_VERTICAL);
    }
}

/**
 * Comprueba colisión con el nido
 * - Si está activo: devuelve ENEMY (no se puede entrar)
 * - Si está libre: devuelve NEST y se activa
 */
Collision HomedFrog::checkCollision(const SDL_FRect& rect)
{
    SDL_FRect nestRect = {
        position.getX(),
        position.getY(),
        width / 2.0f,
        height / 2.0f
    };

    Collision col;
    col.tipo = Collision::NONE;

    if (SDL_HasRectIntersectionFloat(&nestRect, &rect))
    {
        if (isActive || hasWasp)
        {
            col.tipo = Collision::ENEMY;
        }
        else
        {
            col.tipo = Collision::NEST;
            game->addHomedFrog();
            setActive();
        }
    }

    return col;
}