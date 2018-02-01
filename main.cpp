#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <exception>

#include "libdan2/libDan2.h"
#include "libdan2/defDanSDL.h"

//Clases del proyecto aquí.
#include "class/controlador.h"


/*
TODO: Grandes velocidades, se jode el invento de las rampas.

TODO: El tema de saltar en una rampa sigue haciendo cosas simpáticas. Quiźas podamos ignorar las rampas
que estén por debajo nuestra cuando el vector sube????
*/

//Declaración del log del motor en espacio global.
DLibH::Log_base LOG;

int main(int argc, char ** argv)
{
	if(DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
	{
		//Inicializar control de logs.
		LOG.inicializar("data/logs/info.log");
		LOG.activar();

		//Inicializar log del motor. Puede compilarse fuera con la flag -DLOG_BASE_SILENCIOSO=1
		DLibH::Log_motor::arrancar("data/logs/log_motor.log");

		//Inicializar control de argumentos.
		DLibH::Controlador_argumentos CARG(argc, argv);

		Controlador controlador(CARG);
		controlador.inicializar();
		
		while(controlador.loop())
		{

		};
	}
	
	DLibH::Log_motor::finalizar();
	DLibH::Herramientas_SDL::apagar_SDL();	

	return 0;
}
