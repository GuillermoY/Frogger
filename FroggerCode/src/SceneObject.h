#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "GameObject.h"
#include "texture.h"
#include "game.h"
#include "vector2D.h"
#include <SDL3/SDL.h>

class Texture;
class Game;

// Forward declaration de Collision
struct Collision
{
    enum Type
    {
        NONE,
        ENEMY,
        PLATFORM,
        NEST
    };

    Type tipo = Type::NONE;
    Vector2D<> vel = Vector2D<>(0, 0);
};

/**
 * Subclase de GameObject para elementos de la escena.
 * Mantiene posición, dimensiones y textura.
 */
class SceneObject : public GameObject
{
protected:
    Point2D position;
    int width, height;
    Texture* texture;

    // Método protegido para obtener la caja de colisión
    SDL_FRect getBoundingBox() const;

public:
    SceneObject(Game* game, Texture* texture, Point2D pos, int width, int height);
    virtual ~SceneObject() = default;

    // Render por defecto (puede ser sobrescrito)
    virtual void render() const override;

    // Update debe ser implementado por subclases
    virtual void update() override = 0;

    // Método virtual para checkear colisiones
    virtual Collision checkCollision(const SDL_FRect& rect);

    Point2D getPosition() const { return position; }
};

#endif // SCENEOBJECT_H