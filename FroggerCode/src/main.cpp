//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		Game().run();
	}
	catch (...)  // Captura CUALQUIER excepción
	{
		cerr << "ERROR: El juego ha fallado" << endl;
		return 1;
	}

	return 0;
}