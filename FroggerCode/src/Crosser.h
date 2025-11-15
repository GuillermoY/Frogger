#ifndef CROSSER_H
#define CROSSER_H

#include "SceneObject.h"
#include "vector2D.h"
#include "game.h"

/**
 * Clase para objetos que cruzan la escena horizontalmente.
 * Incluye plataformas y vehículos.
 */
class Crosser : public SceneObject
{
protected:
    Vector2D<> velocity;        // Velocidad de movimiento
    float backJump;           // Desplazamiento horizontal para reaparecer

public:
    Crosser(Game* game, Texture* texture, Point2D pos, int width, int height,
        Vector2D<> vel, float wrapOffset);

    // Implementa el comportamiento de cruzar la escena
    virtual void update() override;
};

#endif // CROSSER_H
