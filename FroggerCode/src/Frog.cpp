#include "Frog.h"
#include <iostream>

/**
 * Constructor de Frog
 */
Frog::Frog(Game* game, Texture* texture, Point2D pos, int vidas)
    : SceneObject(game, texture, pos, texture->getFrameWidth(), texture->getFrameHeight()),
    iniPos(pos),
    vidas(vidas)
{
}

/**
 * Renderiza la rana con rotación según su última dirección
 */
void Frog::render() const
{
    SDL_FRect destRect = {
        position.getX(),
        position.getY(),
        width,
        height
    };

    int frame = getCurrentFrame();

    if (lastDir == LEFT)
    {
        texture->renderFrame(destRect, 0, frame, -90, nullptr, SDL_FLIP_NONE);
    }
    else if (lastDir == RIGHT)
    {
        texture->renderFrame(destRect, 0, frame, 90, nullptr, SDL_FLIP_NONE);
    }
    else if (lastDir == UP)
    {
        texture->renderFrame(destRect, 0, frame, SDL_FLIP_NONE);
    }
    else if (lastDir == DOWN)
    {
        texture->renderFrame(destRect, 0, frame, SDL_FLIP_VERTICAL);
    }
    else
    {
        texture->renderFrame(destRect, 0, frame);
    }
}

/**
 * Detecta si la rana se mueve para aplicar la animación
 */
int Frog::getCurrentFrame() const
{
    switch (dir)
    {
    case Direction::UP:
    case Direction::DOWN:
    case Direction::LEFT:
    case Direction::RIGHT:
        return 1;
    case Direction::QUIETO:
    default:
        return 0;
    }
}

/**
 * Actualiza la posición de la rana y maneja colisiones
 */
void Frog::update()
{
    // Obtiene el rectángulo de colisión de la rana
    SDL_FRect frogRect = {
        position.getX(),
        position.getY(),
        width / 2.0f,
        height / 2.0f
    };

    Collision col = game->checkCollision(frogRect);

    // Maneja colisiones
    if (col.tipo == Collision::NEST)
    {
        changePos(iniPos);
    }
    else if (col.tipo == Collision::PLATFORM && !(position.getX() > game->WINDOW_WIDTH - game->CELL_SIZE))
    {
        changeDir(col.vel);
    }
    else if ((position.getX() < 0 || position.getX() > game->WINDOW_WIDTH - game->CELL_SIZE) || (col.tipo == Collision::ENEMY)|| (col.tipo == Collision::NONE && (position.getY() < (game->WINDOW_HEIGHT / 2) - game->CELL_SIZE)))
    {
        changePos(iniPos);
        if (col.tipo == Collision::ENEMY)
        {
            std::cout << "¡COLISION CON ENEMIGO DETECTADA!" << std::endl;
        }
        quitaVida();
    }

    // Mueve la rana según la dirección
    switch (dir)
    {
    case Direction::UP:
        if (position.getY() > Game::CELL_SIZE)
        {
            position = position + Vector2D<>(0, -Game::CELL_SIZE);
            dir = Direction::QUIETO;
        }
        break;
    case Direction::DOWN:
        if (position.getY() < Game::WINDOW_HEIGHT - Game::CELL_SIZE * 3)
        {
            position = position + Vector2D<>(0, Game::CELL_SIZE);
            dir = Direction::QUIETO;
        }
        break;
    case Direction::LEFT:
        if (position.getX() > Game::CELL_SIZE)
        {
            position = position + Vector2D<>(-Game::CELL_SIZE, 0);
            dir = Direction::QUIETO;
        }
        break;
    case Direction::RIGHT:
        if (position.getX() < Game::WINDOW_WIDTH - Game::CELL_SIZE * 2)
        {
            position = position + Vector2D<>(Game::CELL_SIZE, 0);
            dir = Direction::QUIETO;
        }
        break;
    case Direction::QUIETO:
        break;
    }
}

/**
 * Maneja eventos del teclado
 */
void Frog::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        switch (event.key.key)
        {
        case SDLK_UP:
        case SDLK_W:
            dir = Direction::UP;
            break;
        case SDLK_DOWN:
        case SDLK_S:
            dir = Direction::DOWN;
            break;
        case SDLK_LEFT:
        case SDLK_A:
            dir = Direction::LEFT;
            break;
        case SDLK_RIGHT:
        case SDLK_D:
            dir = Direction::RIGHT;
            break;
        }
        lastDir = dir;
    }
}

/**
 * Quita una vida a la rana
 */
void Frog::quitaVida()
{
    vidas--;
    //std::cout << vidas << std::endl;
}

/**
 * Cambia la posición de la rana
 */
void Frog::changePos(Point2D newPos)
{
    position = newPos;
}

/**
 * Cambia la dirección de la rana (para movimiento en plataformas)
 */
void Frog::changeDir(Vector2D<> dirN)
{
    position = position + dirN * game->REAL_FRAME_RATE;
}

/**
 * Carga una rana desde el archivo
 * Formato: F x y vidas
 */
Frog* Frog::load(Game* game, Texture* texture, std::istream& in)
{
    float x, y;
    int vidas;

    in >> x >> y >> vidas;

    return new Frog(game, texture, Point2D(x, y), vidas);
}