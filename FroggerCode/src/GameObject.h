#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Forward declaration
class Game;

/**
 * Clase abstracta base para todos los objetos del juego.
 * Define la interfaz común con métodos virtuales puros.
 */
class GameObject
{
protected:
    Game* game; // Puntero al juego

    // Constructor protegido - solo accesible por subclases
    GameObject(Game* game) : game(game) {}

public:
    // Métodos virtuales puros
    virtual void render() const = 0;
    virtual void update() = 0;

    // Destructor virtual
    virtual ~GameObject() = default;
};

#endif // GAMEOBJECT_H