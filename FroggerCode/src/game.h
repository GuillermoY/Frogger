// Game.h
#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <list>
#include <vector>
#include <random>
#include "vector2D.h"

// Forward declarations
class Texture;
class SceneObject;
class Frog;
class HomedFrog;
struct Collision;

// Alias para el iterador de la lista
using Anchor = std::list<SceneObject*>::iterator;

/**
 * Estructura de colisión
 */

/**
 * Clase principal del juego
 */
class Game
{
public:
    // Constantes del juego
    static constexpr int FRAME_RATE = 30;
    static constexpr int WINDOW_WIDTH = 448;
    static constexpr int WINDOW_HEIGHT = 484;
    static constexpr int CELL_SIZE = 32;
    static constexpr double REAL_FRAME_RATE = FRAME_RATE * 0.001;
    static constexpr int RIVER_LOW = 210;
    static constexpr int HOMED_FROGS_NUM = 5;
    static constexpr double HOMED_FROG_Y_POS = CELL_SIZE / 1.5;
    static constexpr double WASP_SPAWN_X = CELL_SIZE / 3.5;
    static constexpr int BORDER_AND_NEST_GAP = 96 / 8;
    static constexpr int NEST_GAP = 96;

    // Enumeración de texturas
    enum TextureName
    {
        FROG = 0,
        BACKGROUND,
        CAR1,
        CAR2,
        CAR3,
        CAR4,
        CAR5,
        LOG1,
        LOG2,
        WASP,
        TURTLE,
        NUM_TEXTURES
    };

private:
    // SDL
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Texturas
    std::array<Texture*, NUM_TEXTURES> textures;

    // Lista polimórfica de objetos de la escena
    std::list<SceneObject*> sceneObjects;

    std::vector<HomedFrog*> homedFrogs;

    // Iteradores marcados para eliminar
    std::vector<Anchor> objectsToDelete;

    // Rana
    Frog* frog;
    Point2D frogIniPos;

    // Control del juego
    bool exit;
    int actualFrogs;
    static constexpr int homedFrogsNum = 5;

    // Control de spawn de avispas
    int waspSpawnTimer;
    static constexpr int WASP_MIN_SPAWN_TIME = 250;
    static constexpr int WASP_MAX_SPAWN_TIME = 500;
    static constexpr int WASP_LIFESPAN = 250;
    int timeForSpawn;
    int lastNest = -1;
    std::vector<Point2D> spawnPoints = { Point2D(BORDER_AND_NEST_GAP + WASP_SPAWN_X, HOMED_FROG_Y_POS), Point2D(BORDER_AND_NEST_GAP + NEST_GAP + WASP_SPAWN_X, HOMED_FROG_Y_POS),
    Point2D(BORDER_AND_NEST_GAP + NEST_GAP * 2 + WASP_SPAWN_X, HOMED_FROG_Y_POS), Point2D(BORDER_AND_NEST_GAP + NEST_GAP * 3 + WASP_SPAWN_X, HOMED_FROG_Y_POS),
    Point2D(BORDER_AND_NEST_GAP + NEST_GAP * 4 + WASP_SPAWN_X, HOMED_FROG_Y_POS) }; // Contiene las posiciones de cada nido

    // Métodos privados
    void render() const;
    void update();
    void handleEvents();
    void loadMap(const char* mapFile);
    void clearGame();
    void trySpawnWasp();

public:
    Game();
    ~Game();

    // Ejecuta el bucle principal
    void run();

    // Comprueba colisiones
    Collision checkCollision(const SDL_FRect& rect) const;

    // Obtiene una textura
    Texture* getTexture(TextureName name) const;

    // Genera números aleatorios
    int getRandomRange(int min, int max);

    // Gestión de ranas en nidos
    void addHomedFrog();

    // Marca un objeto para eliminación
    void deleteAfter(Anchor anchor);

    // Reinicia el juego
    void restartGame();

    // Activa el último nido ocupado por una avispa
    void alterHomed();
};

inline Texture* Game::getTexture(TextureName name) const
{
    return textures[name];
}

#endif // GAME_H