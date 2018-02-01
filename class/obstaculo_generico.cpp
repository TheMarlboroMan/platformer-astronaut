#include "obstaculo_generico.h"

Obstaculo_generico::Obstaculo_generico(float x, float y)
	:Actor(x, y, W, H)
{

}

Obstaculo_generico::~Obstaculo_generico()
{

}

void Obstaculo_generico::transformar_bloque(Bloque_transformacion_representable &b) const
{
	b.establecer_tipo(Bloque_transformacion_representable::TR_BITMAP);
	b.establecer_alpha(255);
	b.establecer_recurso(Recursos_graficos::RT_BLOQUES);
	b.establecer_recorte(25, 0, W, H);
	b.establecer_posicion(acc_espaciable_x(), acc_espaciable_y(), W, H);
}

Info_colision Obstaculo_generico::obtener_info_colision(const Espaciable& e, unsigned int dir) const
{
	return obtener_info_colision_caja_solida(dir);
}
