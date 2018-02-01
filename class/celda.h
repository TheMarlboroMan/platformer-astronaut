#ifndef CELDA_H
#define CELDA_H

#include "representable.h"
#include "espaciable.h"

class Celda:public Representable, public Espaciable
{
	///////////////////////////
	// Definiciones

	public:

	static const int DIM_CELDA=16; //Dimensiones de una celda en el mundo.

	enum flags {
		F_NADA=0,
		F_SOLIDA_SOLO_ARRIBA=1, //Identifica a las celdas que pueden "traspasarse" y sólo se tienen en cuenta cuando estás por encima de ellas.
		F_RAMPA_CANTO_PASABLE=2, //Identifica a las rampas cuyos cantos (laterales) son pasables. Ahora mismo no caigo en porque, pero en su momento sería una buena idea para el diseño de niveles.
		F_RAMPA_PICO_SOLIDO=4, //Identifica a la rampa que "inicia" una rampa, es decir, aquella que puedes chocar con el pico si estuviera a la altura adecuada. Por defecto no lo tienen para permitir chocar con el pico de las rampas si hay varias en fila,
		F_RAMPA_TECHO_SIN_TECHO=8 //Identifica a la rampa en el techo cuya parte superior no es sólida. Tiene sentido sólo cuando haces una rampa que sube y la adornas por debajo con rampas.
	};
	enum tipos {
		T_NADA=0, 
		T_SOLIDA=1, 
		T_RAMPA_ARRIBA=2, 	

		//Rampas que suben id...
		T_RAMPA_SUBE_ID_MIN=10,
			T_RAMPA_SUBE_ID, T_RAMPA_SUBE_ID_23_INI, T_RAMPA_SUBE_ID_23_FIN,
		T_RAMPA_SUBE_ID_MAX,

		//Rampas que suben di
		T_RAMPA_SUBE_DI_MIN=30,
			T_RAMPA_SUBE_DI, T_RAMPA_SUBE_DI_23_INI, T_RAMPA_SUBE_DI_23_FIN,
		T_RAMPA_SUBE_DI_MAX,

		//Rampas en techo...
		T_RAMPAS_TECHO_MIN=50,
			T_RAMPA_TECHO_ID, T_RAMPA_TECHO_DI,
		T_RAMPAS_TECHO_MAX,

		//Escaleras
		T_ESCALERA_MIN=60,
			T_ESCALERA_NORMAL,
			T_ESCALERA_SOLIDA,
		T_ESCALERA_MAX
	};

	private:

	struct Estructura_rampa
	{
		unsigned int direccion_arriba;
		unsigned int direccion_bajando_rampa;
		unsigned int direccion_abajo;
		unsigned int direccion_subiendo_rampa;

		Estructura_rampa(unsigned int da, unsigned int db, unsigned int dc, unsigned int dd)
			:direccion_arriba(da), direccion_bajando_rampa(db), direccion_abajo(dc),
			direccion_subiendo_rampa(dd)
		{}
	};

	static const Estructura_rampa er_sube_id;
	static const Estructura_rampa er_sube_di;
	static const Estructura_rampa er_techo_id;
	static const Estructura_rampa er_techo_di;

	//////////////
	// Propiedades

	private:

	unsigned int x; //X e Y en función de rejilla.
	unsigned int y;
	unsigned int tipo;
	unsigned int flags;

	mutable bool debug_considerada_colision;

	//////////////////////////////
	// Métodos internos...

	unsigned int obtener_x_mundo() const {return x*DIM_CELDA;}
	unsigned int obtener_xw_mundo() const {return (x*DIM_CELDA)+DIM_CELDA;}
	unsigned int obtener_y_mundo() const {return y*DIM_CELDA;}
	unsigned int obtener_yh_mundo() const {return (y*DIM_CELDA)+DIM_CELDA;}

	Info_colision obtener_info_colision_rampa_techo(const Espaciable&, unsigned int) const;
	Info_colision obtener_info_colision_rampa_ascendente(const Espaciable&, unsigned int) const;
	Info_colision calcular_correccion_rampa(const Espaciable&, unsigned int) const;
	float static calcular_y_rampa_ascendente(const Celda& c, float centro, bool recortar=true);

	bool es_rampa_id() const {return (tipo > T_RAMPA_SUBE_ID_MIN && tipo < T_RAMPA_SUBE_ID_MAX) || tipo==T_RAMPA_TECHO_ID;}
	bool es_rampa_di() const {return (tipo > T_RAMPA_SUBE_DI_MIN && tipo < T_RAMPA_SUBE_DI_MAX) || tipo==T_RAMPA_TECHO_DI;}

	bool es_rampa_sube_id() const {return tipo > T_RAMPA_SUBE_ID_MIN && tipo < T_RAMPA_SUBE_ID_MAX;}
	bool es_rampa_sube_di() const {return tipo > T_RAMPA_SUBE_DI_MIN && tipo < T_RAMPA_SUBE_DI_MAX;}

	const Estructura_rampa obtener_estructura_rampa() const
	{
		if(es_rampa_sube_id()) return er_sube_id;
		else if(es_rampa_sube_di()) return er_sube_di;
		else if(tipo==T_RAMPA_TECHO_ID) return er_techo_id;
		else if(tipo==T_RAMPA_TECHO_DI) return er_techo_di;
		return Estructura_rampa(0,0,0,0);
	};

	////////////////////
	// Implementando Representable...

	public:

	virtual unsigned short int obtener_profundidad_ordenacion() const;
	virtual void transformar_bloque(Bloque_transformacion_representable &b) const;
	virtual std::string a_cadena() const {return "CELDA EN "+DLibH::Herramientas::entero_a_cadena(x)+","+DLibH::Herramientas::entero_a_cadena(y)+" DEL TIPO "+DLibH::Herramientas::entero_a_cadena(tipo);}

	////////////////////
	// Implementando Colisionable / Espaciable...

	virtual Info_colision obtener_info_colision(const Espaciable&, unsigned int)const;

	virtual std::string espaciable_info() const {return a_cadena();}
	virtual Espaciable::t_caja copia_caja() const {return Espaciable::t_caja(obtener_x_mundo(), obtener_y_mundo(), DIM_CELDA, DIM_CELDA);}
	virtual void mut_x_caja(float) {}	//Prácticamente un hack.
	virtual void mut_y_caja(float) {}
	virtual void mut_w_caja(unsigned int) {}
	virtual void mut_h_caja(unsigned int) {}
	virtual void desplazar_caja(float, float) {}

	////////////////////
	// Interface pública

	public:

	bool comprobar_colision_con_caja_jugador(const Espaciable::t_caja&) const;
	bool colisiona_con_caja_segun_forma(const Espaciable::t_caja&) const;

	unsigned int acc_x() const {return x;}
	unsigned int acc_y() const {return y;}
	unsigned int acc_tipo() const {return tipo;}
	bool es_solida_solo_arriba() const {return flags & F_SOLIDA_SOLO_ARRIBA;}
	bool es_rampa_canto_pasable() const {return flags & F_RAMPA_CANTO_PASABLE;}
	bool es_rampa_pico_solido() const {return flags & F_RAMPA_PICO_SOLIDO;}
	bool es_rampa_techo_sin_techo() const {return flags & F_RAMPA_TECHO_SIN_TECHO;}

	bool es_tipo_nada() const {return tipo==T_NADA;}
	bool es_tipo_solido() const {return tipo==T_SOLIDA || es_tipo_indicador_rampa();}
	bool es_tipo_rampa() const {return es_rampa_sube_id() || es_rampa_sube_di();}
	bool es_tipo_escalera() const {return tipo > T_ESCALERA_MIN && tipo < T_ESCALERA_MAX;}
	bool es_tipo_escalera_solida() const {return tipo==T_ESCALERA_SOLIDA;}
	bool es_tipo_escalera_no_solida() const {return tipo==T_ESCALERA_NORMAL;}
	bool es_tipo_rampa_techo() const {return tipo > T_RAMPAS_TECHO_MIN && tipo < T_RAMPAS_TECHO_MAX;}
	bool es_tipo_indicador_rampa() const {return tipo >=T_RAMPA_ARRIBA && tipo <=T_RAMPA_ARRIBA;}
	bool es_tipo_snap() const {return es_tipo_rampa() || es_tipo_indicador_rampa();}
	bool es_traspasable_por_proyectiles() const {return es_tipo_escalera() || es_solida_solo_arriba();}
	
	void mut_x(unsigned int v) {x=v;}
	void mut_y(unsigned int v) {y=v;}
	void mut_tipo(unsigned int v) {tipo=v;}
	void mut_flags(unsigned int v) {flags=v;}

	float obtener_snap_y_para_x(float x) const;
	
	Celda(unsigned int, unsigned int, unsigned int, unsigned int);
	Celda(const Celda&);
	Celda& operator=(const Celda&);
	~Celda();

	void debug_establecer_considerada_colision(bool v) const {debug_considerada_colision=v;} //LOOOL.

	static void purgar_vector_de_indicadores_rampas(std::vector<const Celda *>& celdas);
	static void purgar_vector_de_celdas_solo_solidas_por_arriba_segun_caja(std::vector<const Celda *>& celdas, const Espaciable::t_caja& caja);
};

#endif
