#include "Crosser.h"

/**
 * Constructor de Crosser
 */
Crosser::Crosser(Game* game, Texture* texture, Point2D pos, int width, int height,
    Vector2D<> vel, float backJump)
    : SceneObject(game, texture, pos, width, height),
    velocity(vel),
    backJump(backJump)
{
}

/**
 * Update: mueve el objeto y lo hace reaparecer por el otro lado
 */
void Crosser::update()
{
    // Mueve el objeto según su velocidad
    position = position + velocity * game->REAL_FRAME_RATE;

    // Comprueba si sale por la izquierda
    if (position.getX() < -backJump)
    {
        position = Point2D(Game::WINDOW_WIDTH + backJump, position.getY());
    }
    // Comprueba si sale por la derecha
    else if (position.getX() > Game::WINDOW_WIDTH + backJump)
    {
        position = Point2D(-backJump, position.getY());
    }
}