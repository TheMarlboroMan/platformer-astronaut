#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "configuracion.h"
#include "input.h"
#include "audio.h"
#include "localizador.h"
#include "cargador_recursos.h"

#include "base_proyecto/tabla_sprites.h"

#include "representador.h"
#include "representable.h"
#include "celda.h"
#include "nivel.h"
#include "actor.h"
#include "jugador.h"
#include "proyectil.h"
#include "origen_proyectil.h"
#include "obstaculo_generico.h"
#include "plataforma_movil.h"

class Controlador
{
	///////////////////
	// Propiedades

	private:

	DLibH::Controlador_argumentos& controlador_argumentos;
	DLibH::Controlador_fps_SDL controlador_fps;
	Configuracion configuracion;
	DLibV::Pantalla pantalla;
	Localizador localizador;
	Input input;

	DLibV::Camara camara;
	Representador representador;

	Tabla_sprites tabla_sprites_jugador;
	Nivel nivel;
	Jugador jugador;
	const Plataforma_movil * plataforma_union;
	std::vector<Obstaculo_generico * > obstaculos;	//TODO: Destruir al salir.
	std::vector<Plataforma_movil * > plataformas;	//TODO: Destruir al salir.
	std::vector<Proyectil * > proyectiles;

	unsigned int id_idioma;
	bool pausado;
	bool frame_a_frame;

	///////////////////////////
	// Internos...

	private:

	void inicializar_entorno_grafico(DLibV::Pantalla& pantalla, unsigned int w, unsigned int h);
	void configurar_entorno_grafico(DLibV::Pantalla& pantalla, unsigned int w, unsigned int h);

	void inicializar_entorno_audio();
	void configurar_entorno_audio();

	void cargar_nivel_desde_fichero();

	void obtener_plataformas_moviles(const Espaciable::t_caja& caja, std::vector<const Espaciable *>& destino);

	void controlar_colisiones_jugador(unsigned int, const Espaciable::t_caja& caja_original);
	void controlar_colisiones_jugador_escalera();

	void controlar_fin_movimiento_horizontal_jugador();
	void controlar_fin_movimiento_vertical_jugador();
	void controlar_subida_escalera(short int);
	void controlar_bajada_escalera(short int);

	void generar_disparo_jugador();
	void procesar_turno_proyectiles(float);

	void dibujar();
	Input_usuario obtener_input_usuario();

//	void procesar_borrado_actores_con_cantidad(std::vector<Actor *>&, unsigned int);

	/////////////////////
	// Interfaz pública

	public:

	void inicializar();
	bool loop();
	Controlador(DLibH::Controlador_argumentos&);
};

#endif
