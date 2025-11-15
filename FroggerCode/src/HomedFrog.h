// HomedFrog.h
#ifndef HOMEDFROG_H
#define HOMEDFROG_H

#include "SceneObject.h"
#include "game.h"
#include "texture.h"

/**
 * Representa un nido donde la rana puede llegar.
 * Cambia de comportamiento cuando está ocupado.
 */
class HomedFrog : public SceneObject
{
private:
    bool isActive = false;  // Si está ocupado por una rana
    bool hasWasp = false; // Si está ocupado por una wasp
public:
    HomedFrog(Game* game, Texture* texture, Point2D pos);

    // Sobrescribe render para mostrar solo si está activo
    virtual void render() const override;

    // Update no hace nada (es estático)
    virtual void update() override {}

    // Sobrescribe checkCollision
    virtual Collision checkCollision(const SDL_FRect& rect) override;

    // Getters y setters
    void setActive() { isActive = true; }
    bool getActive() const { return isActive; }
    void alterWasp() { if (hasWasp)hasWasp = false; else { hasWasp = true; } }
};

#endif // HOMEDFROG_H