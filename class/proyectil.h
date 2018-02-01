#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "actor.h"
#include "movil.h"
#include "origen_proyectil.h"

class Proyectil:public Actor, public Movil
{
	//////////////////////////////////
	// Definiciones

	public:

	private:

	static const unsigned int W=4;
	static const unsigned int H=4;

	//////////////////////////////////
	// Propiedades

	private:

	float velocidad;

	//////////////////////////////////
	// Métodos internos

	private:

	/////////////////////////
	// Implementando Actor...

	private:
	virtual float obtener_peso() const {return 0.0;}

	///////////////////////////
	// Implementando Espaciable

	public:

	virtual Info_colision obtener_info_colision(const Espaciable&, unsigned int) const {return Info_colision(*this);}
	virtual std::string espaciable_info() const {return a_cadena();}

	///////////////////////
	// Implementación de Representable.

	public:

	virtual unsigned short int obtener_profundidad_ordenacion() const {return 15;}
	virtual void transformar_bloque(Bloque_transformacion_representable &b) const;
	virtual std::string a_cadena() const {return "PROYECTIL "+DLibH::Herramientas::float_a_cadena(acc_espaciable_x())+","+DLibH::Herramientas::float_a_cadena(acc_espaciable_y());}

	//////////////////////////////////
	// Interface pública

	public:

	Proyectil();
	~Proyectil();

	void configurar(float px, float py, float vx, float vy, float pvel);
	void turno(float delta);

};

#endif
