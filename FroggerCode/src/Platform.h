#ifndef PLATFORM_H
#define PLATFORM_H

#include "Crosser.h"

/**
 * Clase base para todas las plataformas (troncos y tortugas).
 * Define el comportamiento de colisión de una plataforma.
 */
class Platform : public Crosser
{
protected:
    // Factor de ajuste para la caja de colisión
    float collisionWidthFactor = 1.0f;
    float collisionHeightFactor = 0.5f;

public:
    // Copia el constructor de Crosser con using
    using Crosser::Crosser;

    // Define checkCollision con la lógica de plataforma
    virtual Collision checkCollision(const SDL_FRect& rect) override;
};

#endif // PLATFORM_H