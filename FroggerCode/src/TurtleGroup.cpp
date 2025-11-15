#include "TurtleGroup.h"
#include <iostream>

/**
 * Constructor de TurtleGroup
 */
TurtleGroup::TurtleGroup(Game* game, Texture* texture, Point2D pos, Vector2D<> vel,
    int numTurtles, bool canSubmerge)
    : Platform(game, texture, pos,
        texture->getFrameWidth(),
        texture->getFrameHeight(),
        vel, TURTLE_MARGIN),
    numTurtles(numTurtles),
    canSubmerge(canSubmerge),
    isSubmerged(false),
    animationTimer(0),
    currentFrame(0)
{
    // Ajusta el factor de colisión según el número de tortugas
    collisionWidthFactor = 1.0f;
    collisionHeightFactor = 0.5f;
}

/**
 * Update: comportamiento de cruzar + hundimiento periódico
 */
void TurtleGroup::update()
{
    Crosser::update();

    animationTimer++;

    if (canSubmerge)
    {

        int animStep = (animationTimer / FRAMES_PER_SPRITE) % 8;  // 8 pasos totales

        switch (animStep)
        {
        case 0: currentFrame = 0; isSubmerged = false; break;
        case 1: currentFrame = 1; isSubmerged = false; break;
        case 2: currentFrame = 2; isSubmerged = false; break;
        case 3: currentFrame = 3; isSubmerged = false; break;
        case 4: currentFrame = 4; isSubmerged = false; break;
        case 5: currentFrame = 5; isSubmerged = true;  break;  // Sin colisión
        case 6: currentFrame = 6; isSubmerged = false; break;
        case 7: currentFrame = 3; isSubmerged = false; break;
        }
    }
    else
    {
        // No se hunde: solo anima frames 0-2
        if (animationTimer % ANIMATION_SPEED == 0)
        {
            currentFrame = (currentFrame + 1) % 3;
        }
    }
}

/**
 * Render: no dibuja si está hundida
 */
void TurtleGroup::render() const
{
    for (int i = 0; i < numTurtles; i++)
    {
        SDL_FRect destRect = {
            position.getX() + i * width,
            position.getY(),
            static_cast<float>(width),
            static_cast<float>(height)
        };

        texture->renderFrame(destRect, 0, currentFrame);
    }
}

/**
 * CheckCollision: no colisiona cuando está hundida
 */
Collision TurtleGroup::checkCollision(const SDL_FRect& rect)
{
    Collision col;
    col.tipo = Collision::NONE;

    // Si está sumergido, no hay colisión
    if (isSubmerged)
    {
        return col;
    }

    // Caja de colisión del grupo completo de tortugas
    SDL_FRect platformRect = {
        position.getX(),
        position.getY(),
        width * numTurtles,  // Ancho total del grupo
        height * collisionHeightFactor
    };

    if (SDL_HasRectIntersectionFloat(&platformRect, &rect))
    {
        col.tipo = Collision::PLATFORM;
        col.vel = velocity;  // velocity viene de Crosser
    }

    return col;
}

/**
 * Carga un TurtleGroup desde el archivo
 */
TurtleGroup* TurtleGroup::load(Game* game, Texture* texture, std::istream& in)
{
    float x, y, vx;
    int n, h;

    in >> x >> y >> vx >> n >> h;

    return new TurtleGroup(game, texture, Point2D(x, y),
        Vector2D<>(vx, 0), n, h != 0);
}