#include "Vehicle.h"
#include <SDL3/SDL.h>
#include <iostream>

/**
 * Constructor de Vehicle
 */
Vehicle::Vehicle(Game* game, Texture* texture, Point2D pos, Vector2D<> vel)
    : Crosser(game, texture, pos,
        texture->getFrameWidth(),
        texture->getFrameHeight(),
        vel, VEHICLE_MARGIN)
{
}

/**
 * Comprueba colisión con el vehículo
 * Devuelve ENEMY si hay colisión
 */
Collision Vehicle::checkCollision(const SDL_FRect& rect)
{
    Collision col;
    col.tipo = Collision::NONE;

    // Caja de colisión del vehículo (altura reducida)
    SDL_FRect vehicleRect = {
        position.getX(),
        position.getY(),
        static_cast<float>(width),
        height / 2.0f
    };

    if (SDL_HasRectIntersectionFloat(&vehicleRect, &rect))
    {
        col.tipo = Collision::ENEMY;
    }

    return col;
}

/**
 * Carga un Vehicle desde el archivo
 * Formato: V x y velocidad tipoTextura
 */
Vehicle* Vehicle::load(Game* game, Texture* texture, std::istream& in)
{
    float x, y, velocidad;
    int tipoTextura;

    in >> x >> y >> velocidad >> tipoTextura;

    return new Vehicle(game, texture, Point2D(x, y), Vector2D<>(velocidad, 0));
}