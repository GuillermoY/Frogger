#ifndef TURTLEGROUP_H
#define TURTLEGROUP_H

#include "Platform.h"
#include "game.h"
#include "texture.h"

/**
 * Grupo de tortugas que remonta el río.
 * Puede hundirse periódicamente.
 */
class TurtleGroup : public Platform
{
private:
    int numTurtles;           // Número de tortugas en el grupo
    bool canSubmerge;         // Si el grupo se hunde o no
    bool isSubmerged;         // Estado actual (hundido o no)

    // Control de animación
    int animationTimer;          // Contador de frames para animar
    int currentFrame;            // Frame actual (0-6)


    static constexpr int ANIMATION_SPEED = 10;      // Frames entre cambios de sprite
    static constexpr int FRAMES_PER_SPRITE = 8;  // Velocidad de animación

    static constexpr float TURTLE_MARGIN = 150.0f;

public:
    TurtleGroup(Game* game, Texture* texture, Point2D pos, Vector2D<> vel,
        int numTurtles, bool canSubmerge);

    // Sobrescribe update para manejar el hundimiento
    virtual void update() override;

    // Sobrescribe render para no dibujar cuando está hundida
    virtual void render() const override;

    // Sobrescribe checkCollision - no colisiona cuando está hundida
    virtual Collision checkCollision(const SDL_FRect& rect) override;

    // Lee los datos desde archivo
    static TurtleGroup* load(Game* game, Texture* texture, std::istream& in);
};

#endif // TURTLEGROUP_H
