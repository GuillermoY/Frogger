#ifndef GAMEERROR_H
#define GAMEERROR_H

#include <stdexcept>
#include <string>
#include <SDL3/SDL.h>

/**
 * Excepción base para todos los errores del juego
 */
class GameError : public std::logic_error
{
public:
    explicit GameError(const std::string& message)
        : std::logic_error(message) {}
};

/**
 * Excepción para errores de SDL
 */
class SDLError : public GameError
{
public:
    explicit SDLError(const std::string& message)
        : GameError(message + ": " + SDL_GetError()) {}
};

/**
 * Excepción para archivos no encontrados
 */
class FileNotFoundError : public GameError
{
public:
    explicit FileNotFoundError(const std::string& filename)
        : GameError("Archivo no encontrado: " + filename) {}
};

/**
 * Excepción para errores de formato en archivos
 */
class FileFormatError : public GameError
{
private:
    std::string filename;
    int lineNumber;

public:
    FileFormatError(const std::string& filename, int lineNumber, const std::string& message)
        : GameError("Error de formato en " + filename +
            " (línea " + std::to_string(lineNumber) + "): " + message),
        filename(filename),
        lineNumber(lineNumber)
    {}

    const std::string& getFilename() const { return filename; }
    int getLineNumber() const { return lineNumber; }
};

#endif // GAMEERROR_H
