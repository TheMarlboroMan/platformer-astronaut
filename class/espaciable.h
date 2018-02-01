#ifndef ESPACIABLE_H
#define ESPACIABLE_H

/*Interfaz que exige los métodos para definir una caja en 2d y a partir de ahí,
monta el resto. Que defina una caja 2d no significa que realmente haya una
caja por ahí, por ejemplo, las celdas de un nivel no tienen cajas reales, aunque
devuelven una caja calculada. 

Está implícito también que implementa muchas funciones para temas de colisión,
como por ejemplo, saber si es sólido por todos lados o no, obtener información
de colisión y demás. Intentamos separarlo en dos interfaces pero teníamos tanto
el problema del diamante como que son casi la misma cosa. Se quedan, por tanto,
como están.
*/

#include "../libdan2/libDan2.h"

class Espaciable;

struct Info_colision
{
	const Espaciable * espaciable;

	float x;	//Posición final.
	float y;
	unsigned int dir_ajuste_x; //Dirección en la que ajustar (a la izquierda o derecha, arriba o abajo) la posición final.
	unsigned int dir_ajuste_y;
	bool ajuste_x;	//Indica si debe o no realizarse un ajuste en este sentido.
	bool ajuste_y;

	void establecer_x(float v, unsigned int d) {x=v; dir_ajuste_x=d; ajuste_x=true;}				
	void establecer_y(float v, unsigned int d) {y=v; dir_ajuste_y=d; ajuste_y=true;}

	Info_colision(const Espaciable& e):
		espaciable(&e), x(0.0), y(0.0), dir_ajuste_x(0), dir_ajuste_y(0), ajuste_x(false), ajuste_y(false) 
	{}

	Info_colision(const Info_colision& o):
		espaciable(o.espaciable), x(o.x), y(o.y),
		dir_ajuste_x(o.dir_ajuste_x), dir_ajuste_y(o.dir_ajuste_y), ajuste_x(o.ajuste_x), ajuste_y(o.ajuste_y) 
	{}

	const Info_colision& operator=(const Info_colision& o)
	{
		espaciable=o.espaciable;
		x=o.x;
		y=o.y;
		dir_ajuste_x=o.dir_ajuste_x;
		dir_ajuste_y=o.dir_ajuste_y;
		ajuste_x=o.ajuste_x;
		ajuste_y=o.ajuste_y;

		return *this;
	}

/*
La función de comparación debe ser:

"Binary function that accepts two elements in the range as arguments, and 
returns a value convertible to bool. The value returned indicates whether 
the element passed as first argument is considered to go before the second 
in the specific strict weak ordering it defines."

Un detalle a cómo se hace esto: consideramos que si NO hay ajuste en el eje que
estamos buscando, el elemento es menor. El tipo de ordenación ascendente o 
descendente sólo cambia con el valor del ajuste.
*/

	static bool comparar_asc(const Info_colision&a, const Info_colision& b, float Info_colision::*p_posicion, bool Info_colision::*p_ajuste)
	{
		if(a.*p_ajuste!=b.*p_ajuste) return a.*p_ajuste < b.*p_ajuste;
		else return a.*p_posicion < b.*p_posicion;
	}

	static bool comparar_desc(const Info_colision&a, const Info_colision& b, float Info_colision::*p_posicion, bool Info_colision::*p_ajuste)
	{
		if(a.*p_ajuste!=b.*p_ajuste) return a.*p_ajuste < b.*p_ajuste;
		else return a.*p_posicion > b.*p_posicion;
	}
};

class Plataforma_movil; //F-f-f-f-forward!!!.

struct Datos_colision
{
	const Plataforma_movil * const plataforma_movil;

	Datos_colision(const Plataforma_movil * const p):plataforma_movil(p)
	{}

	Datos_colision():plataforma_movil(NULL)
	{}
};

class Ordenador_info_colision
{
	private:

	bool ascendente;
	float Info_colision::*p_posicion;
	bool Info_colision::*p_ajuste;

	public:

	enum {OX=1, OY=2};

	Ordenador_info_colision(bool a, unsigned int t)
		:ascendente(a), p_posicion(NULL), p_ajuste(NULL)
	{
		if(t==OX)
		{
			p_posicion=&Info_colision::x;
			p_ajuste=&Info_colision::ajuste_x;
		}
		else
		{
			p_posicion=&Info_colision::y;
			p_ajuste=&Info_colision::ajuste_y;
		}
	}

	bool operator()(const Info_colision& a, const Info_colision& b) const
	{
		bool resultado=ascendente ? Info_colision::comparar_asc(a, b, p_posicion, p_ajuste) :
					Info_colision::comparar_desc(a, b, p_posicion, p_ajuste);
		return resultado;
	}
};

class Espaciable
{
	///////////////////////////////
	// Definiciones

	public:

	typedef DLibH::Caja<float, unsigned int> t_caja;
	enum t{T_X=1, T_Y=2};
	enum direcciones{D_ARRIBA=1, D_DERECHA=2, D_ABAJO=4, D_IZQUIERDA=8};

	///////////////////////////////
	// Propiedades e internas...

	protected:

	//////////////////////////////
	// Interfaz pública.

	public:

	Espaciable();
	~Espaciable();

	float acc_espaciable_x() const {return copia_caja().origen.x;}
	float acc_espaciable_y() const {return copia_caja().origen.y;}
	float acc_espaciable_fx() const {return copia_caja().origen.x+copia_caja().w;}
	float acc_espaciable_fy() const {return copia_caja().origen.y+copia_caja().h;}
	unsigned int acc_espaciable_w() const {return copia_caja().w;}
	unsigned int acc_espaciable_h() const {return copia_caja().h;}
	DLibH::Punto_2d<float> acc_espaciable_posicion() const {return copia_caja().origen;}

	bool en_colision_con(const Espaciable&, bool=false) const;

	//Estos son siempre según la geometría en la que 0.0 es el punto superior
	//a la izquierda.

	bool es_sobre(const Espaciable& otra) const {return es_sobre(copia_caja(), otra.copia_caja());}
	bool es_bajo(const Espaciable& otra) const {return es_bajo(copia_caja(), otra.copia_caja());}
	bool es_a_la_izquierda_de(const Espaciable& otra) const {return es_a_la_izquierda_de(copia_caja(), otra.copia_caja());}
	bool es_a_la_derecha_de(const Espaciable& otra) const {return es_a_la_derecha_de(copia_caja(), otra.copia_caja());}

	static bool es_sobre(const t_caja& una, const t_caja& otra) {return una.origen.y+una.h <= otra.origen.y;}
	static bool es_bajo(const t_caja& una, const t_caja& otra) {return una.origen.y >= otra.origen.y+otra.h;}
	static bool es_a_la_izquierda_de(const t_caja& una, const t_caja& otra) {return una.origen.x+una.w <= otra.origen.x;}
	static bool es_a_la_derecha_de(const t_caja& una, const t_caja& otra) {return una.origen.x >= otra.origen.x+otra.w;}

	//Al principio era protected, ahora la estamos llamando desde el controlador.
	Info_colision obtener_info_colision_caja_solida(unsigned int) const;

	////////////////////////////
	// A implementar

	virtual std::string espaciable_info() const=0;
	virtual t_caja copia_caja() const=0;	//Debe devolver una COPIA de la caja.
	virtual Info_colision obtener_info_colision(const Espaciable&, unsigned int)const=0;

	//Si hay algún tipo de efecto específico lo registramos aquí. Por defecto no 
	//registramos nada, of course.
	virtual Datos_colision obtener_datos_colision() const {return Datos_colision();}
	
	virtual void mut_x_caja(float)=0;	//Estos no tienen porqué hacer nada si no hay una caja real.
	virtual void mut_y_caja(float)=0;
	virtual void desplazar_caja(float, float)=0;	//Este desplazaría x e y unidades.
	virtual void mut_w_caja(unsigned int)=0;
	virtual void mut_h_caja(unsigned int)=0;
};

#endif
