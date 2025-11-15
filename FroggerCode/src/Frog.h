// Frog.h
#ifndef FROG_H
#define FROG_H

#include "SceneObject.h"
#include "vector2D.h"
#include "game.h"
#include "texture.h"

enum Direction
{
    QUIETO = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * Personaje jugable - la rana.
 * Hereda de SceneObject.
 */
class Frog : public SceneObject
{
private:
    Direction dir = Direction::QUIETO;      // Dirección actual
    Direction lastDir = Direction::QUIETO;  // Última dirección de movimiento
    Point2D iniPos;                         // Posición inicial
    int vidas;                              // Vidas restantes

    // Obtiene el frame actual según la dirección
    int getCurrentFrame() const;

public:
    Frog(Game* game, Texture* texture, Point2D pos, int vidas);

    // Sobrescribe render para manejar rotaciones
    virtual void render() const override;

    // Sobrescribe update para el movimiento
    virtual void update() override;

    // Maneja eventos de teclado
    void handleEvent(const SDL_Event& event);

    // Getters
    int getLives() const { return vidas; }
    Point2D getPos() const { return position; }

    // Métodos auxiliares
    void changePos(Point2D newPos);
    void changeDir(Vector2D<> dirN);
    void quitaVida();

    // Carga la rana desde archivo
    static Frog* load(Game* game, Texture* texture, std::istream& in);
};

#endif // FROG_H