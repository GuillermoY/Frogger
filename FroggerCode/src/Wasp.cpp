// Wasp.cpp
#include "Wasp.h"
#include <SDL3/SDL.h>

/**
 * Constructor
 */
Wasp::Wasp(Game* game, Texture* texture, Point2D pos, int lifespan, Anchor anchor)
    : SceneObject(game, texture, pos,
        texture->getFrameWidth(),
        texture->getFrameHeight()),
    lifespan(lifespan),
    currentLife(0),
    myAnchor(anchor)
{
}

/**
 * Renderiza solo si está viva
 */
void Wasp::render() const
{
    if (isAlive())
    {
        SDL_FRect destRect = {
            position.getX(),
            position.getY(),
            width,
            height
        };
        texture->render(destRect);
    }
}

/**
 * Actualiza el tiempo de vida y se elimina si expira
 */
void Wasp::update()
{
    currentLife++;

    // Si ha expirado, marca para eliminación
    if (!isAlive())
    {
        game->alterHomed();
        game->deleteAfter(myAnchor);
    }
}

/**
 * Comprueba si la avispa sigue viva
 */
bool Wasp::isAlive() const
{
    return currentLife < lifespan;
}

/**
 * Comprueba colisión - solo colisiona si está viva
 */
Collision Wasp::checkCollision(const SDL_FRect& rect)
{
    Collision col;
    col.tipo = Collision::NONE;

    if (!isAlive())
        return col;

    SDL_FRect waspRect = {
        position.getX(),
        position.getY(),
        static_cast<float>(width),
        static_cast<float>(height)
    };

    if (SDL_HasRectIntersectionFloat(&waspRect, &rect))
    {
        col.tipo = Collision::ENEMY;
    }

    return col;
}
