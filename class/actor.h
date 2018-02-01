#ifndef ACTOR_H
#define ACTOR_H

#include "representable.h"
#include "espaciable.h"

/*El actor es algo que se representa (en un principio... Podemos hacer que no
maneje el bloque y todos tan tranquilos) y que existe en el espacio de juego.
Podemos encontrar con controladores que hacen cosas y que da igual su punto
en el espacio, pero bueno, no es importante.

Ojo, es espaciable, pero NO es Colisionable.
*/

class Actor:public Representable, public Espaciable
{
	////////////////////////
	// Propiedades.

	private:

	Espaciable::t_caja caja;
	bool para_borrar; //Aunque no lo vayamos a usar en todos.

	////////////////////////	
	// A implementar...

	virtual float obtener_peso() const=0;

	///////////////////////
	// Implementación de Representable.

	public:

	virtual unsigned short int obtener_profundidad_ordenacion() const=0;
	virtual void transformar_bloque(Bloque_transformacion_representable &b) const=0;
	virtual std::string a_cadena() const=0;

	///////////////////////
	// Implementación de espaciable

	virtual std::string espaciable_info() const=0;

	virtual Espaciable::t_caja copia_caja() const {return caja;}
	virtual void mut_x_caja(float v) {caja.origen.x=v;}
	virtual void mut_y_caja(float v) {caja.origen.y=v;}
	virtual void mut_w_caja(unsigned int v) {caja.w=v;}
	virtual void mut_h_caja(unsigned int v) {caja.h=v;}
	virtual void desplazar_caja(float x, float y) {caja.origen.x+=x; caja.origen.y+=y;}
	virtual void establecer_posicion(float x, float y) {caja.origen.x=x; caja.origen.y=y;}

	///////////////////////
	// Interfaz pública.

	public:

	Actor(float x, float y, unsigned int w, unsigned int h);
	~Actor();

	bool es_para_borrar() const {return para_borrar;}
	void marcar_para_borrar() {para_borrar=true;}
};

#endif
