#include "SceneObject.h"

/**
 * Constructor de SceneObject
 */
SceneObject::SceneObject(Game* game, Texture* texture, Point2D pos, int width, int height)
    : GameObject(game), texture(texture), position(pos), width(width), height(height)
{
}

/**
 * Render por defecto - renderiza la textura completa
 */
void SceneObject::render() const
{
    SDL_FRect destRect = {
        position.getX(),
        position.getY(),
        static_cast<float>(width),
        static_cast<float>(height)
    };
    texture->render(destRect);
}

/**
 * Obtiene la caja de colisión del objeto
 */
SDL_FRect SceneObject::getBoundingBox() const
{
    return SDL_FRect{
        position.getX(),
        position.getY(),
        static_cast<float>(width),
        static_cast<float>(height)
    };
}

/**
 * Implementación por defecto de checkCollision
 */
Collision SceneObject::checkCollision(const SDL_FRect& rect)
{
    Collision col;
    col.tipo = Collision::NONE;
    return col;
}