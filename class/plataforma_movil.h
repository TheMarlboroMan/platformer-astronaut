#ifndef PLATAFORMA_MOVIL_H
#define PLATAFORMA_MOVIL_H

#include "actor.h"
#include "movil.h"

class Plataforma_movil:public Actor, public Movil
{
	////////////////////////
	// Definiciones...

	public:

	static const unsigned int W=16;
	static const unsigned int H=6;

	private:

	/////////////////////////
	// Propiedades...

	private:

	unsigned int tipo;
	float distancia_recorrida;

	/////////////////////////
	// Métodos internos.
	
	/////////////////////////
	// Implementando Actor...

	private:

	virtual float obtener_peso() const {return 0.0;}

	///////////////////////////
	// Implementando Espaciable

	public:

	virtual Info_colision obtener_info_colision(const Espaciable&, unsigned int direccion) const;
	virtual std::string espaciable_info() const {return a_cadena();}
	virtual Datos_colision obtener_datos_colision() const {return Datos_colision(this);}

	///////////////////////
	// Implementación de Representable.

	public:

	virtual unsigned short int obtener_profundidad_ordenacion() const {return 20;}
	virtual void transformar_bloque(Bloque_transformacion_representable &b) const;
	virtual std::string a_cadena() const {return "PLATAFORMA MOVIL EN EN "+DLibH::Herramientas::float_a_cadena(acc_espaciable_x())+","+DLibH::Herramientas::float_a_cadena(acc_espaciable_y());}

	////////////////////////
	// Interfaz pública...

	Plataforma_movil(float px, float py, unsigned int);
	~Plataforma_movil();

	void turno(float p_delta); //TODO: El turno debería ser parte de una interface, no?...
};

#endif
