#ifndef VEHICLE_H
#define VEHICLE_H

#include "Crosser.h"
#include "game.h"

/**
 * Vehículo que cruza la carretera.
 * Es un enemigo que causa colisión.
 */
class Vehicle : public Crosser
{
private:
    static constexpr float VEHICLE_MARGIN = 150.0f;

public:
    Vehicle(Game* game, Texture* texture, Point2D pos, Vector2D<> vel);

    // Sobrescribe checkCollision para comportamiento de enemigo
    virtual Collision checkCollision(const SDL_FRect& rect) override;

    // Carga un Vehicle desde archivo
    static Vehicle* load(Game* game, Texture* texture, std::istream& in);
};

#endif // VEHICLE_H