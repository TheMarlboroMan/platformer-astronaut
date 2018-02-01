#ifndef NIVEL_H
#define NIVEL_H

#include <map>
#include "celda.h"

/*

*/

class Nivel
{
	///////////////////////////
	// Definiciones

	public:

	enum flags_obtener_celdas {F_CON_SOLO_SOLIDAS_ARRIBA=1};

	///////////////////////////
	// Propiedades

	private:

	unsigned int w_en_celdas; //Alto y ancho en celdas, no en 
	unsigned int h_en_celdas; //dimensiones del mundo.

	std::map<unsigned int, Celda> rejilla; //La rejilla se almacena como un mapa, no como un vector.
	Celda celda_no_existe; //Esta es la que devolvemos si pedimos una celda que no existe en el mapa.

	///////////////////////////
	// Internas.

	unsigned int calcular_indice_celda(unsigned int, unsigned int) const;
	unsigned int mundo_a_indice_celdas(float) const;

	///////////////////////////
	// Interface pública

	public:

	void actualizar_celda(unsigned int, unsigned int, unsigned int, unsigned int);
	Celda& obtener_celda_en_rejilla(unsigned int, unsigned int);
	Celda& obtener_celda_en_coordenadas(float, float);
	static DLibH::Punto_2d<int> rejilla_para_coordenadas(float, float);

	void obtener_celdas_representables(std::vector<Representable *>&);
	const std::vector<const Celda *> celdas_para_caja(const Espaciable::t_caja& caja) const;

	Nivel();
	~Nivel();
};

#endif
