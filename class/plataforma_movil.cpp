#include "plataforma_movil.h"

Plataforma_movil::Plataforma_movil(float px, float py, unsigned int pt)
	:Actor(px, py, W, H), Movil(), tipo(pt), distancia_recorrida(0.0)
{
	establecer_vector(40.0, tipo==0 ? Movil::V_X : Movil::V_Y);
}

Plataforma_movil::~Plataforma_movil()
{

}

void Plataforma_movil::turno(float p_delta)
{
	float d=0.0;

	if(tipo==0)
	{
		d=acc_vector().x * p_delta;
		desplazar_caja(d, 0.0);
	}
	else  if(tipo==1)
	{
		d=acc_vector().y * p_delta;
		desplazar_caja(0.0, d);
	}

	distancia_recorrida+=fabs(d);

	if(distancia_recorrida >= 128.0)
	{
		distancia_recorrida=0.0;

		if(tipo==0) establecer_vector(-(acc_vector().x), Movil::V_X);
		else establecer_vector(-(acc_vector().y), Movil::V_Y);
	}
}

void Plataforma_movil::transformar_bloque(Bloque_transformacion_representable &b) const
{
	b.establecer_tipo(Bloque_transformacion_representable::TR_BITMAP);
	b.establecer_alpha(255);
	b.establecer_recurso(Recursos_graficos::RT_BLOQUES);
	b.establecer_recorte(20, 32, W, H);
	b.establecer_posicion(acc_espaciable_x(), acc_espaciable_y(), W, H);
}

Info_colision Plataforma_movil::obtener_info_colision(const Espaciable& e, unsigned int direccion) const
{
	return obtener_info_colision_caja_solida(direccion);
}
