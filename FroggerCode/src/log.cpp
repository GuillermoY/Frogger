#include "Log.h"
#include <iostream>

/**
 * Constructor de Log
 */
Log::Log(Game* game, Texture* texture, Point2D pos, Vector2D<> vel)
    : Platform(game, texture, pos,
        texture->getFrameWidth(),
        texture->getFrameHeight(),
        vel, LOG_MARGIN)
{
    // Ajusta los factores de colisión para los troncos
    collisionWidthFactor = 1.0f / 1.3f;  // Más ajustado en ancho
    collisionHeightFactor = 0.5f;
}

/**
 * Carga un Log desde el archivo
 * Formato: L x y velocidad tipoTextura
 */
Log* Log::load(Game* game, Texture* texture, std::istream& in)
{
    float x, y, velocidad;
    int tipoTextura;

    in >> x >> y >> velocidad >> tipoTextura;

    return new Log(game, texture, Point2D(x, y), Vector2D<>(velocidad, 0));
}