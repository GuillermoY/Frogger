#ifndef LOG_H
#define LOG_H

#include "Platform.h"
#include "game.h"

/**
 * Tronco que flota en el río.
 * Es una plataforma que cruza la escena.
 */
class Log : public Platform
{
private:
    static constexpr float LOG_MARGIN = 150.0f;

public:
    Log(Game* game, Texture* texture, Point2D pos, Vector2D<> vel);

    // Carga un Log desde archivo
    static Log* load(Game* game, Texture* texture, std::istream& in);
};

#endif // LOG_H