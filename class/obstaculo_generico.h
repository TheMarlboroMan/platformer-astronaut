#ifndef OBSTACULO_GENERICO_H
#define OBSTACULO_GENERICO_H

#include "actor.h"

class Obstaculo_generico:public Actor
{
	/////////////////////
	// Definiciones

	public:

	static const unsigned int W=9;
	static const unsigned int H=9;

	/////////////////////
	// Propiedades

	private:

	/////////////////////////
	// Implementando Actor...

	private:

	virtual float obtener_peso() const {return 0.0;}

	///////////////////////////
	// Implementando espaciable / Colisionable

	public:

	virtual Info_colision obtener_info_colision(const Espaciable&, unsigned int) const;
	virtual std::string espaciable_info() const {return a_cadena();}

	///////////////////////
	// Implementación de Representable.

	public:

	virtual unsigned short int obtener_profundidad_ordenacion() const {return 10;}
	virtual void transformar_bloque(Bloque_transformacion_representable &b) const;
	virtual std::string a_cadena() const {return "OBSTACULO EN "+DLibH::Herramientas::float_a_cadena(acc_espaciable_x())+","+DLibH::Herramientas::float_a_cadena(acc_espaciable_y());}

	/////////////////////
	// Interfaz pública

	public:

	Obstaculo_generico(float, float);
	~Obstaculo_generico();

};

#endif
