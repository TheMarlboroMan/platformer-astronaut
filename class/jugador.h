#ifndef JUGADOR_H
#define JUGADOR_H

#include "actor.h"
#include "movil.h"
#include "origen_proyectil.h"
#include "base_proyecto/tabla_sprites.h"

struct Input_usuario
{
	short int mov_horizontal;
	short int mov_vertical;
	bool accion_1;
	bool accion_2;

	Input_usuario():mov_horizontal(0), mov_vertical(0), accion_1(false), accion_2(false) {}
};

class Jugador:public Actor, public Movil
{
	////////////////////////
	// Definiciones...

	public:

	static const unsigned int W=8;
	static const unsigned int H=21;
	static const unsigned int DIST_MAXIMA_BAJAR_ESCALERA=8;
	static const unsigned int DIST_MAXIMA_SUBIR_ESCALERA=12;

	enum class estados {E_SUELO=0, E_AIRE=1, E_ESCALERA=2, E_ARMA=3, E_ARMA_RETROCESO=4};
	enum class direcciones {D_IZQUIERDA, D_DERECHA, D_ARRIBA, D_ARRIBA_IZQUIERDA, D_ARRIBA_DERECHA};

	private:

	static const float IMPULSO_SALTO;
	static const float MIN_VELOCIDAD_SUELO;		//Mínima velocidad en unidades por segundo.
	static const float MAX_VELOCIDAD_SUELO;		//Maxima velocidad en unidades por segundo.
	static const float FACTOR_ACELERACION_SUELO;	//Unidades de velocidad alcanzadas en un segundo.
	static const float FACTOR_DECELERACION_SUELO;	//Unidades de velocidad que se pierden en un segundo.
	static const float VELOCIDAD_ESCALERA;		//Velocidad fija en una escalera.

	/////////////////////////
	// Propiedades...

	private:

	Tabla_sprites * TABLA_SPRITES;
	estados estado;
	short int movimiento_horizontal;	//Movimiento horizontal que ha recibido en este tic.
	short int movimiento_vertical;
	direcciones direccion;
	direcciones direccion_arma;
	bool generar_proyectil;

	/////////////////////////
	// Implementando Actor...

	private:

	virtual float obtener_peso() const {return 150.0;}

	///////////////////////////
	// Implementando Espaciable

	public:

	virtual Info_colision obtener_info_colision(const Espaciable&, unsigned int) const {return Info_colision(*this);}
	virtual std::string espaciable_info() const {return a_cadena();}

	///////////////////////
	// Implementación de Representable.

	public:

	virtual unsigned short int obtener_profundidad_ordenacion() const {return 20;}
	virtual void transformar_bloque(Bloque_transformacion_representable &b) const;
	virtual std::string a_cadena() const {return "JUGADOR EN "+DLibH::Herramientas::float_a_cadena(acc_espaciable_x())+","+DLibH::Herramientas::float_a_cadena(acc_espaciable_y());}

	/////////////////////////
	// Métodos internos.
	
	bool con_movimiento_horizontal() const {return estado==estados::E_SUELO || estado==estados::E_AIRE;}
	bool con_movimiento_vertical() const {return estado==estados::E_AIRE || estado==estados::E_ESCALERA;}
	void procesar_movimiento_horizontal(float p_delta);	//Acelera o frena el vector horizontal antes de realizar el movimiento.
	void procesar_movimiento_vertical(float p_delta);	
	bool puede_saltar() const;
	bool puede_sacar_arma() const;

	////////////////////////
	// Interfaz pública...

	Jugador(float, float);
	~Jugador();

	bool es_en_aire() const {return estado==estados::E_AIRE;}
	bool es_en_suelo() const {return estado==estados::E_SUELO;}
	bool es_en_escalera() const {return estado==estados::E_ESCALERA;}
	bool es_con_arma() const {return estado==estados::E_ARMA;}
	bool es_generar_proyectil() const {return generar_proyectil;}

	//Todo: faltaría un método para generar el origen y dirección de un
	//proyectil.
	//direccion acc_direccion_arma() const {return direccion_arma;}

	void turno(float delta);
	void movimiento(unsigned int, float);
	void turno_arma();
	void ajustar_y_abajo(float pos);
	void ajustar_y_arriba(float pos);
	void ajustar_x_izquierda(float pos);
	void ajustar_x_derecha(float pos);
	void ajustar_y_abajo_sin_estado(float pos) {mut_y_caja(pos-H);}
	void ajustar_y_arriba_sin_estado(float pos) {mut_y_caja(pos);}
	void ajustar_x_izquierda_sin_estado(float pos) {mut_x_caja(pos);}
	void ajustar_x_derecha_sin_estado(float pos) {mut_x_caja(pos-W);}
	void saltar();

	void recibir_input(const Input_usuario&);
	void frenar_movimiento_horizontal(float p_delta);
	void revertir_a_posicion(const Espaciable::t_caja& caja);

	void establecer_en_suelo() {estado=estados::E_SUELO;}
	void establecer_en_aire() {estado=estados::E_AIRE;}
	void establecer_en_escalera();
	void establecer_en_arma();

	Espaciable::t_caja obtener_caja_suelo() const;
	Espaciable::t_caja obtener_caja_escalera() const;

	void recibir_vector_plataforma(const Movil& m, float delta);
	void centrar_en_escalera(const Espaciable&);

	Origen_proyectil generar_info_proyectil();
	void establecer_tabla_sprites(Tabla_sprites& t) {TABLA_SPRITES=&t;}
};

#endif
