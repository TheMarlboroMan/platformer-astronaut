#include "proyectil.h"

Proyectil::Proyectil()
	:Actor(0, 0, W, H), Movil(), velocidad(0.0)
{

}

Proyectil::~Proyectil()
{

}

void Proyectil::configurar(float px, float py, float pvx, float pvy, float pvel)
{
	establecer_posicion(px, py);


	velocidad=pvel;
	establecer_vector(pvx * velocidad, V_X);
	establecer_vector(pvy * velocidad, V_Y);
}

void Proyectil::turno(float delta)
{
	const DLibH::Vector_2d& v=acc_vector();
	desplazar_caja(v.x * delta, v.y * delta);
}

void Proyectil::transformar_bloque(Bloque_transformacion_representable &b) const
{
	b.establecer_tipo(Bloque_transformacion_representable::TR_BITMAP);
	b.establecer_alpha(192);
	b.establecer_recurso(Recursos_graficos::RT_SPRITES);
//	b.rotar(25.5);

	b.establecer_recorte(0, 0, W, H);
	b.establecer_posicion(acc_espaciable_x(), acc_espaciable_y(), W, H);
}
