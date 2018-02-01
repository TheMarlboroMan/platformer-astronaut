#ifndef ORIGEN_PROYECTIL_H
#define ORIGEN_PROYECTIL_H

/*Define los parámetros necesarios para generar un proyectil. Sirve para que
lo puedan conocer clases que no tienen necesidad de conocer al proyectil en 
si (por ejemplo, el jugador).**/

struct Origen_proyectil
{
	float x, y, vx, vy, velocidad;
	unsigned int tipo;

	Origen_proyectil(float px, float py, float pvx, float pvy, float pvel, unsigned int pt)
		:x(px), y(py), vx(pvx), vy(pvy), velocidad(pvel), tipo(pt)
	{}
};

#endif
