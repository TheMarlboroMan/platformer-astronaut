#include "jugador.h"

const float Jugador::IMPULSO_SALTO=-250.0;
const float Jugador::MIN_VELOCIDAD_SUELO=40.0;
const float Jugador::MAX_VELOCIDAD_SUELO=180.0;
const float Jugador::FACTOR_ACELERACION_SUELO=240.0;
const float Jugador::FACTOR_DECELERACION_SUELO=480.0;
const float Jugador::VELOCIDAD_ESCALERA=100.0;

Jugador::Jugador(float x, float y)
	:Actor(x, y, W, H), Movil(), estado(estados::E_AIRE), 
	movimiento_horizontal(0), direccion(direcciones::D_DERECHA), 
	direccion_arma(direcciones::D_DERECHA), generar_proyectil(false)
{

}

Jugador::~Jugador()
{

}

void Jugador::transformar_bloque(Bloque_transformacion_representable &b) const
{
	b.establecer_tipo(Bloque_transformacion_representable::TR_BITMAP);
	b.establecer_alpha(255);
	b.establecer_recurso(Recursos_graficos::RT_JUGADOR);
//	b.rotar(25.5);

	unsigned int indice_sprite=0;
	bool invertir_si_izquierda=true;

	switch(estado)
	{
		case estados::E_SUELO: indice_sprite=0; break;
		case estados::E_AIRE: indice_sprite=acc_vector().y < 0 ? 10 : 11; break;
		case estados::E_ESCALERA:
			indice_sprite=20;
			invertir_si_izquierda=false;
		break;
		case estados::E_ARMA:
			switch(direccion_arma)
			{
				case direcciones::D_DERECHA:
				case direcciones::D_IZQUIERDA:
					indice_sprite=30;
				break;

				break;
				case direcciones::D_ARRIBA:
					indice_sprite=31;
				break;
				case direcciones::D_ARRIBA_IZQUIERDA:
				case direcciones::D_ARRIBA_DERECHA:
					indice_sprite=32;
				break;
			}
		break;
		case estados::E_ARMA_RETROCESO:
		break;
	}

	Frame_sprites f=TABLA_SPRITES->obtener(indice_sprite);

	if(f)
	{
		b.establecer_recorte(f.x, f.y, f.w, f.h);
		if(invertir_si_izquierda && direccion==direcciones::D_IZQUIERDA) b.invertir_horizontal(true); 
		b.establecer_posicion(acc_espaciable_x()+f.desp_x, acc_espaciable_y()+f.desp_y, f.w, f.h);
	}
}

/*Método de turno genérico: recibe la cantidad de tiempo que ha pasado y la
suma a los contadores internos, si es necesario. Controlaría cosas de tiempo,
como el retroceso del arma.*/

void Jugador::turno(float delta)
{
	//TODO TODO TODO TODO
}

/*Método que hace el turno de movimiento para el actor. El controlador limita
los estados en los que es posible realizar la llamada.*/

void Jugador::movimiento(unsigned int t, float delta)
{
	switch(t)
	{
		case Espaciable::T_X:	
			if(con_movimiento_horizontal())
			{
				procesar_movimiento_horizontal(delta);
				if(acc_vector().x) desplazar_caja(acc_vector().x * delta, 0.0); 
			}
		break;
		case Espaciable::T_Y: 	
			if(con_movimiento_vertical())
			{
				//Sólo si está en el aire integramos la gravedad.
				if(estado==estados::E_AIRE) integrar_vector(delta, ref_vector_y(), obtener_peso() * 4.0);
				procesar_movimiento_vertical(delta);
				desplazar_caja(0.0, acc_vector().y * delta); 
			}
		break;
		default: break;
	}
}

/*En función de los inputs pulsados, se establecer una dirección para el 
arma o no. La dirección del arma afecta también a la dirección del jugador
para cuando salga de este estado.*/
	
void Jugador::turno_arma()
{
	if(movimiento_horizontal > 0 && !movimiento_vertical) 
	{
		direccion_arma=direcciones::D_DERECHA;
		direccion=direccion_arma;
	}
	else if(movimiento_horizontal < 0 && !movimiento_vertical) 
	{
		direccion_arma=direcciones::D_IZQUIERDA;
		direccion=direccion_arma;
	}
	else if(movimiento_vertical < 0 && !movimiento_horizontal) 
	{
		direccion_arma=direcciones::D_ARRIBA;
	}
	else if(movimiento_horizontal > 0 && movimiento_vertical < 0) 
	{
		direccion_arma=direcciones::D_ARRIBA_DERECHA;
		direccion=direcciones::D_DERECHA;
	}
	else if(movimiento_horizontal < 0 && movimiento_vertical < 0) 
	{
		direccion_arma=direcciones::D_ARRIBA_IZQUIERDA;	
		direccion=direcciones::D_IZQUIERDA;
	}
}

void Jugador::saltar()
{
	establecer_vector(IMPULSO_SALTO, Movil::V_Y);
	estado=estados::E_AIRE;
}

void Jugador::ajustar_y_abajo(float pos) 
{
	mut_y_caja(pos-H); 
	establecer_vector(0.0, Movil::V_Y);
	estado=estados::E_SUELO;
}

void Jugador::ajustar_y_arriba(float pos) 
{
	mut_y_caja(pos); 

	//Esto impide que si estamos moviendonos hacia la derecha en una rampa 
	//de techo nos quedemos "pegados" al mismo.
	if(acc_vector().y < 0.0) 
	{
		establecer_vector(0.0, Movil::V_Y);
	}
}

void Jugador::ajustar_x_izquierda(float pos) 
{
	mut_x_caja(pos);
	establecer_vector(0.0, Movil::V_X);
}

void Jugador::ajustar_x_derecha(float pos) 
{
	mut_x_caja(pos-W);
	establecer_vector(0.0, Movil::V_X);
}



void Jugador::procesar_movimiento_horizontal(float p_delta)
{
	float vx=acc_vector().x;
	
	auto controlar_horizontal=[this, vx, p_delta]() -> float
	{
		float mov=movimiento_horizontal * (p_delta * FACTOR_ACELERACION_SUELO);
		mov+=vx;

		if(movimiento_horizontal > 0)
		{
			if(mov > MAX_VELOCIDAD_SUELO) mov=MAX_VELOCIDAD_SUELO;
			else if(mov < MIN_VELOCIDAD_SUELO) mov=MIN_VELOCIDAD_SUELO;
		}
		else
		{
			if(mov < -MAX_VELOCIDAD_SUELO) mov=-MAX_VELOCIDAD_SUELO;
			if(mov > -MIN_VELOCIDAD_SUELO) mov=-MIN_VELOCIDAD_SUELO;
		}

		return mov;
	};

	if(estado==estados::E_SUELO)
	{	
		if(!movimiento_horizontal)
		{
			float v=abs(vx);
			v-=p_delta * FACTOR_DECELERACION_SUELO;
			if(v < 0.0) v=0.0;
			establecer_vector(vx >= 0 ? v : -v, Movil::V_X);
		}
		else
		{
			float mov=controlar_horizontal();
			establecer_vector(mov, Movil::V_X);
		}
	}
	else if(estado==estados::E_AIRE)
	{
		if(movimiento_horizontal)
		{
			float mov=controlar_horizontal();
			establecer_vector(mov, Movil::V_X);
		}
	}

	float vf=acc_vector().x;
	if(vf > 0.0) direccion=direcciones::D_DERECHA;
	else if(vf < 0.0) direccion=direcciones::D_IZQUIERDA;
}

void Jugador::procesar_movimiento_vertical(float delta)
{
	if(estado==estados::E_ESCALERA && movimiento_vertical)
	{
		if(movimiento_vertical > 0) establecer_vector(VELOCIDAD_ESCALERA, Movil::V_Y);
		else establecer_vector(-VELOCIDAD_ESCALERA, Movil::V_Y);
	}
}

Espaciable::t_caja Jugador::obtener_caja_suelo() const
{
	Espaciable::t_caja resultado=copia_caja();
	resultado.origen.y+=resultado.h-1;
	resultado.h=1;
	return resultado;
}

Espaciable::t_caja Jugador::obtener_caja_escalera() const
{
	Espaciable::t_caja resultado=copia_caja();
	resultado.h=resultado.h / 2;
	return resultado;
}

/*Cuando el jugador está sobre una plataforma recibe parte del movimiento
de la misma... */

void Jugador::recibir_vector_plataforma(const Movil& m, float delta)
{
	float dist=m.acc_vector().x * delta;
	desplazar_caja(dist, 0.0);
	//TODO: Otra forma de hacer esto sería tener un vector de fuerzas externas...
//		sumar_vector(, Movil::V_X);
}

/*Cuando se agarra una escalera se detiene todo vector de movimiento.*/

void Jugador::establecer_en_escalera()
{
	estado=estados::E_ESCALERA;
	establecer_vector(0, Movil::V_X);
	establecer_vector(0, Movil::V_Y);
}

bool Jugador::puede_saltar() const 
{
	if(estado==estados::E_SUELO) return true;
	else if(estado==estados::E_ESCALERA) return movimiento_horizontal; //En una escalera sólo puedes saltar hacia los lados.
	else return false;
}

bool Jugador::puede_sacar_arma() const
{
	return estado==estados::E_SUELO;
}

void Jugador::recibir_input(const Input_usuario& IU)
{
	movimiento_horizontal=IU.mov_horizontal; 
	movimiento_vertical=IU.mov_vertical;
	if(IU.accion_2)
	{
		if(estado==estados::E_ARMA) establecer_en_suelo();
		else if(puede_sacar_arma()) establecer_en_arma();
	}
	else if(IU.accion_1)
	{
		//Realmente levanta la bandera "voy a disparar".
		if(estado==estados::E_ARMA) generar_proyectil=true;
		else if(puede_saltar()) saltar();
	}
}

/*Cuando el jugador se agarra a una escalera cambia su posición X.*/

void Jugador::centrar_en_escalera(const Espaciable& e)
{
	float pos=(e.acc_espaciable_w() / 2) - acc_espaciable_w()/2;
	mut_x_caja(e.acc_espaciable_x()+pos);
}

void Jugador::establecer_en_arma()
{
	estado=estados::E_ARMA;
	direccion_arma=direccion;
	establecer_vector(0, Movil::V_X);
	establecer_vector(0, Movil::V_Y);
}

/*Se llamaría desde el controlador cuando tenemos la certeza de que el jugador
va a disparar. Genera la estructura para saber dónde colocar el proyectil y 
además, baja la bandera que hace que se llame a este método en primer lugar.*/

Origen_proyectil Jugador::generar_info_proyectil()
{
	float x=acc_espaciable_x(), y=acc_espaciable_y(), vx=0.0, vy=0.0, vel=100.0;

	switch(direccion_arma)
	{
		case direcciones::D_DERECHA: vx=1.0; break;
		case direcciones::D_IZQUIERDA: vx=-1.0; break;
		case direcciones::D_ARRIBA: vy=-1.0; break;
		case direcciones::D_ARRIBA_IZQUIERDA: vy=-1.0; vx=-1.0; break;
		case direcciones::D_ARRIBA_DERECHA: vy=-1.0; vx=1.0; break;
	}		

	generar_proyectil=false;
	return Origen_proyectil(x, y, vx, vy, vel, 1);
}
