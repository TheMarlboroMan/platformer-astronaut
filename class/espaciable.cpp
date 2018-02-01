#include "espaciable.h"

Espaciable::Espaciable()
{

}

Espaciable::~Espaciable()
{

}

bool Espaciable::en_colision_con(const Espaciable& e, bool roce_es_colision) const
{
	return copia_caja().es_en_colision_con(e.copia_caja(), roce_es_colision);
}
/*
bool Espaciable::es_solido_por(unsigned int lado) const
{
	return lado & obtener_solidez();
}
*/
/*En este método generamos un objeto del tipo Info_colisión a partir del 
espaciable que ha colisionado con la instancia y la dirección en la que se 
está movimiendo cuando lo ha hecho (contraria al lado con el que choca, vaya).
Cuando llegamos aquí podemos asumir que alguna parte de las dos cajas 
está en colisión y debemos asumir que la posición anterior era válida. Un 
objeto MUY pequeño que se mueva a una velocidad muy grande puede terminar en
posiciones ilegales si lo sometemos a esto.*/

Info_colision Espaciable::obtener_info_colision_caja_solida(unsigned int dir) const
{
	Info_colision resultado(*this);
	
	switch(dir)
	{
		//Colisión contra superior al ir hacia abajo... Ajustaría al actor móvil por abajo con el borde superior del obstáculo.
		case D_ABAJO: resultado.establecer_y(acc_espaciable_y(), D_ABAJO); break;

		case D_ARRIBA: resultado.establecer_y(acc_espaciable_fy(), D_ARRIBA); break;

		case D_IZQUIERDA: resultado.establecer_x(acc_espaciable_fx(), D_IZQUIERDA); break;

		//Colisión contra lado izquierdo al ir hacia la derecha... Ajustaría al actor móvil a la derecha con el borde izquierdo del obstáculo.
		case D_DERECHA: resultado.establecer_x(acc_espaciable_x(), D_DERECHA); break;
	}

	return resultado;
}
