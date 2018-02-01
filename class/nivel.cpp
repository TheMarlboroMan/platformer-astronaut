#include "nivel.h"

Nivel::Nivel()
	:w_en_celdas(100), h_en_celdas(100),
	celda_no_existe(0,0,0,false)
{

}

Nivel::~Nivel()
{

}

unsigned int Nivel::calcular_indice_celda(unsigned int px, unsigned int py) const {return (py * w_en_celdas) + px;}

void Nivel::actualizar_celda(unsigned int px, unsigned int py, unsigned int pt, unsigned int pf)
{
	unsigned int indice=calcular_indice_celda(px, py);

	if(rejilla.count(indice)) 
	{
		Celda& c=rejilla.at(indice);
		c.mut_tipo(pt);
		c.mut_flags(pf);
	}
	else
	{
		rejilla.insert(std::make_pair(indice, Celda(px, py, pt, pf) ));
	}
}

/*Esta es sólo para el mundo de fuera... No es const, devuelve la celda REAL.*/

Celda& Nivel::obtener_celda_en_rejilla(unsigned int px, unsigned int py)
{
	unsigned int indice=calcular_indice_celda(px, py);
	if(rejilla.count(indice)) 
	{
		return rejilla.at(indice);
	}
	else 
	{
		celda_no_existe.mut_x(px);
		celda_no_existe.mut_y(py);
		return celda_no_existe;
	}
}

Celda& Nivel::obtener_celda_en_coordenadas(float px, float py)
{
	DLibH::Punto_2d<int> p=rejilla_para_coordenadas(px, py);
	return obtener_celda_en_rejilla(p.x, p.y);
}

DLibH::Punto_2d<int> Nivel::rejilla_para_coordenadas(float px, float py)
{
	int x=floor(px / Celda::DIM_CELDA);
	int y=floor(py / Celda::DIM_CELDA);
	return DLibH::Punto_2d<int>(x, y);
}


void Nivel::obtener_celdas_representables(std::vector<Representable *>& v)
{
	auto insertar=[&v](Representable * b)
	{
		v.push_back(b);
	};

	for(std::pair<const unsigned int, Celda>& par : rejilla)
	{
		Representable * cp=&(par.second);
		insertar(cp);
	}
}

/*Especificando ceil hacemos que las colisiones por abajo y por la derecha sean
siempre muy exactas. Especificando floor conseguimos el caso contrario... El 
caso que no escojamos tendremos que tratarlo...*/

unsigned int Nivel::mundo_a_indice_celdas(float val) const
{
	int v=floor(val);
	return floor(v / Celda::DIM_CELDA); //Si W y H tuvieran valores distintos sería un tema.
}

/*Ojo del tipo const Espaciable *, no const Espaciable * const, porque los
vectores deben ser asignables. O algo así.*/

const std::vector<const Celda *> Nivel::celdas_para_caja(const Espaciable::t_caja& c) const
{
	int ini_x=mundo_a_indice_celdas(c.origen.x);
	int fin_x=mundo_a_indice_celdas(c.origen.x+c.w);
	int ini_y=0;
	int fin_y=mundo_a_indice_celdas(c.origen.y+c.h);

	if(ini_x < 0) ini_x=0;
	if(fin_x < 0) fin_x=0;
	if(ini_y < 0) ini_y=0;

	//TODO: Retirar cuando hayamos finalizado.
	for(const std::pair<const unsigned int, Celda>& par : rejilla) par.second.debug_establecer_considerada_colision(false);

	std::vector<const Celda *> celdas;
	do
	{
		ini_y=mundo_a_indice_celdas(c.origen.y);
		do
		{
			if(ini_x >= 0 && ini_y >= 0) 
			{			
				unsigned int indice=calcular_indice_celda(ini_x, ini_y);

				if(rejilla.count(indice)) 
				{
					const Celda& cel=rejilla.at(indice);

					if(cel.comprobar_colision_con_caja_jugador(c))
//					if(cel.colisiona_con_caja_segun_forma(c))
					{
						celdas.push_back(&cel);	
						//TODO TODO TODO
						rejilla.at(indice).debug_establecer_considerada_colision(true);
						//TODO TODO TODO
					}
				}
			}
			++ini_y;

		}while(ini_y <= fin_y);

		++ini_x;
	}while(ini_x <= fin_x);

	return celdas;
}
