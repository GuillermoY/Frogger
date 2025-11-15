// Wasp.h
#ifndef WASP_H
#define WASP_H

#include "SceneObject.h"
#include "game.h"
#include "texture.h"

/**
 * Avispa que aparece sobre los nidos vacíos.
 * Tiene un tiempo de vida limitado.
 */
class Wasp : public SceneObject
{
private:
    int lifespan;        // Tiempo de vida total en frames
    int currentLife;     // Tiempo de vida actual
    Anchor myAnchor;     // Iterador en la lista de objetos

public:
    Wasp(Game* game, Texture* texture, Point2D pos, int lifespan, Anchor anchor);

    // Sobrescribe render
    virtual void render() const override;

    // Sobrescribe update para controlar el tiempo de vida
    virtual void update() override;

    // Sobrescribe checkCollision
    virtual Collision checkCollision(const SDL_FRect& rect) override;

    // Comprueba si sigue viva
    bool isAlive() const;
};

#endif // WASP_H