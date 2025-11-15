#include "Game.h"
#include "GameError.h"
#include "texture.h"
#include "Frog.h"
#include "Vehicle.h"
#include "Log.h"
#include "Wasp.h"
#include "TurtleGroup.h"
#include "HomedFrog.h"
#include "SceneObject.h"
#include <SDL3_image/SDL_image.h>
#include <fstream>
#include <limits>
#include <string>

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Frogger 2.0";
constexpr const char* const MAP_FILE = "../assets/maps/turtles.txt";

// Especificación de texturas
struct TextureSpec
{
    const char* name;
    int nrows = 1;
    int ncols = 1;
};

constexpr const char* const imgBase = "../assets/images/";

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
    TextureSpec{"frog.png", 1, 2},
    {"background.png"},
    {"car1.png"},
    {"car2.png"},
    {"car3.png"},
    {"car4.png"},
    {"car5.png"},
    {"log1.png"},
    {"log2.png"},
    {"wasp.png"},
    {"turtle.png", 1, 7}
};

/**
 * Constructor - inicializa SDL y carga recursos
 */
Game::Game()
    : exit(false), frog(nullptr), actualFrogs(0), waspSpawnTimer(0)
{
    // Inicializa SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw SDLError("Error al inicializar SDL");

    // Crea la ventana
    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == nullptr)
        throw SDLError("Error al crear la ventana");

    // Crea el renderer
    renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr)
        throw SDLError("Error al crear el renderer");

    // Carga las texturas
    for (size_t i = 0; i < textures.size(); i++)
    {
        auto [name, nrows, ncols] = textureList[i];
        try
        {
            textures[i] = new Texture(renderer,
                (string(imgBase) + name).c_str(),
                nrows, ncols);
        }
        catch (const exception& e)
        {
            throw GameError("Error al cargar textura: "s + name);
        }
    }
    timeForSpawn = getRandomRange(WASP_MIN_SPAWN_TIME, WASP_MAX_SPAWN_TIME);
    for (int i = 0; i < HOMED_FROGS_NUM; ++i)
    {
        HomedFrog* newHome = new HomedFrog(this, getTexture(FROG), Point2D(BORDER_AND_NEST_GAP + NEST_GAP * i, HOMED_FROG_Y_POS));
        sceneObjects.push_back(newHome);
        homedFrogs.push_back(newHome);
    }
    // Carga el mapa
    loadMap(MAP_FILE);
}

/**
 * Destructor - libera recursos
 */
Game::~Game()
{
    clearGame();

    // Libera texturas
    for (auto texture : textures)
    {
        delete texture;
    }

    // Libera SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/**
 * Limpia todos los objetos del juego
 */
void Game::clearGame()
{
    // Elimina la rana
    delete frog;
    frog = nullptr;

    // Elimina todos los objetos de la escena
    for (auto obj : sceneObjects)
    {
        delete obj;
    }
    sceneObjects.clear();
    objectsToDelete.clear();
}

/**
 * Renderiza todos los elementos del juego
 */
void Game::render() const
{
    SDL_RenderClear(renderer);

    // Renderiza el fondo
    textures[BACKGROUND]->render();

    // Renderiza todos los objetos de la escena polimórficamente
    for (const auto* obj : sceneObjects)
    {
        obj->render();
    }

    // Renderiza la rana
    if (frog)
        frog->render();

    SDL_RenderPresent(renderer);
}

/**
 * Actualiza todos los elementos del juego
 */
void Game::update()
{
    // Actualiza todos los objetos polimórficamente
    for (auto* obj : sceneObjects)
    {
        obj->update();
    }

    // Actualiza la rana
    if (frog)
        frog->update();

    trySpawnWasp();

    // Elimina objetos marcados para eliminación
    for (auto anchor : objectsToDelete)
    {
        delete* anchor;
        sceneObjects.erase(anchor);
    }
    objectsToDelete.clear();
}

/**
 * Bucle principal del juego
 */
void Game::run()
{
    while (!exit && frog && frog->getLives() > 0 && actualFrogs < homedFrogsNum)
    {
        handleEvents();
        render();
        update();
        SDL_Delay(FRAME_RATE);
    }
}

/**
 * Maneja eventos
 */
void Game::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            exit = true;
        }
        else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_0)
        {
            // Reiniciar el juego con Ctrl o Shift presionado
            SDL_Keymod mods = SDL_GetModState();
            if (mods & (SDL_KMOD_CTRL | SDL_KMOD_SHIFT))
            {
                restartGame();
            }
        }
        else if (frog)
        {
            frog->handleEvent(event);
        }
    }
}

/**
 * Comprueba colisiones con todos los objetos
 */
Collision Game::checkCollision(const SDL_FRect& rect) const
{
    Collision result;
    result.tipo = Collision::NONE;

    Collision nestCollision;
    nestCollision.tipo = Collision::NONE;

    for (const auto* obj : sceneObjects)
    {
        Collision col = const_cast<SceneObject*>(obj)->checkCollision(rect);

        if (col.tipo != Collision::NONE)
        {
            // ENEMY tiene máxima prioridad - devuelve inmediatamente
            if (col.tipo == Collision::ENEMY)
            {
                return col;
            }
            
            // NEST se guarda pero seguimos buscando por si hay ENEMY
            if (col.tipo == Collision::NEST)
            {
                nestCollision = col;
            }

            // PLATFORM se guarda pero tiene menos prioridad
            if (col.tipo == Collision::PLATFORM)
            {
                result = col;
            }
        }
    }
    if (nestCollision.tipo != Collision::NONE)
    {
        return nestCollision;
    }
    else
    {
        return result;
    }
}

void Game::alterHomed()
{
    homedFrogs[lastNest]->alterWasp();
}

void Game::trySpawnWasp()
{
    waspSpawnTimer++;

    if (waspSpawnTimer >= timeForSpawn)
    {
        waspSpawnTimer = 0;
        lastNest = getRandomRange(0, homedFrogsNum - 1);
        
        while (homedFrogs[lastNest]->getActive())
        {
            lastNest = getRandomRange(0, homedFrogsNum - 1);
        }

        sceneObjects.push_back(nullptr);
        Anchor anchor = --sceneObjects.end();
        Wasp* wasp = new Wasp(this, getTexture(WASP), spawnPoints[lastNest], WASP_LIFESPAN, anchor);
        homedFrogs[lastNest]->alterWasp();
        *anchor = wasp;

        timeForSpawn = getRandomRange(WASP_MIN_SPAWN_TIME, WASP_MAX_SPAWN_TIME);
    }
}


/**
 * Carga el mapa desde un archivo
 */
void Game::loadMap(const char* mapFile)
{
    ifstream entrada(mapFile);
    if (!entrada)
        throw FileNotFoundError(mapFile);

    char tipo;
    int lineNumber = 0;

    while (entrada >> tipo)
    {
        lineNumber++;

        try
        {
            if (tipo == '#')
            {
                // Comentario - ignora la línea
                entrada.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else if (tipo == 'F')
            {
                // Rana
                frog = Frog::load(this, getTexture(FROG), entrada);
                frogIniPos = frog->getPos();
            }
            else if (tipo == 'V')
            {
                // Vehículo
                float x, y, vel;
                int texType;
                entrada >> x >> y >> vel >> texType;

                TextureName texName = static_cast<TextureName>(CAR1 + texType - 1);
                Vehicle* vehicle = new Vehicle(this, getTexture(texName),
                    Point2D(x, y), Vector2D<>(vel, 0));
                sceneObjects.push_back(vehicle);
            }
            else if (tipo == 'L')
            {
                // Tronco
                float x, y, vel;
                int texType;
                entrada >> x >> y >> vel >> texType;

                TextureName texName = (texType == 0) ? LOG1 : LOG2;
                Log* log = new Log(this, getTexture(texName),
                    Point2D(x, y), Vector2D<>(vel, 0));
                sceneObjects.push_back(log);
            }
            else if (tipo == 'T')
            {
                // Grupo de tortugas
                TurtleGroup* turtles = TurtleGroup::load(this, getTexture(TURTLE), entrada);
                sceneObjects.push_back(turtles);
            }
            else
            {
                throw FileFormatError(mapFile, lineNumber,
                    string("Tipo desconocido: ") + tipo);
            }
        }
        catch (const exception& e)
        {
            throw FileFormatError(mapFile, lineNumber, e.what());
        }
    }

    entrada.close();
}

/**
 * Reinicia el juego
 */
void Game::restartGame()
{
    clearGame();
    actualFrogs = 0;
    loadMap(MAP_FILE);
}

/**
 * Genera un número aleatorio en el rango [min, max]
 */
int Game::getRandomRange(int min, int max)
{
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

/**
 * Añade una rana al contador de ranas en nidos
 */
void Game::addHomedFrog()
{
    actualFrogs++;
    //cout << "NEW HOMED " << actualFrogs << endl;
}

/**
 * Marca un objeto para eliminación al final del update
 */
void Game::deleteAfter(Anchor anchor)
{
    objectsToDelete.push_back(anchor);
}