
#include "celda.h"

//TODO TODO...
extern DLibH::Log_base LOG;
//TODO TODO

const Celda::Estructura_rampa Celda::er_sube_id=Estructura_rampa(Espaciable::D_ARRIBA, Espaciable::D_IZQUIERDA, Espaciable::D_ABAJO, Espaciable::D_DERECHA);
const Celda::Estructura_rampa Celda::er_sube_di=Estructura_rampa(Espaciable::D_ARRIBA, Espaciable::D_DERECHA, Espaciable::D_ABAJO, Espaciable::D_IZQUIERDA);
const Celda::Estructura_rampa Celda::er_techo_id=Estructura_rampa(Espaciable::D_ARRIBA, Espaciable::D_IZQUIERDA, Espaciable::D_ABAJO, Espaciable::D_DERECHA);
const Celda::Estructura_rampa Celda::er_techo_di=Estructura_rampa(Espaciable::D_ARRIBA, Espaciable::D_DERECHA, Espaciable::D_ABAJO, Espaciable::D_IZQUIERDA);
//const Celda::Estructura_rampa Celda::er_techo_id=Estructura_rampa(Espaciable::D_ABAJO, Espaciable::D_DERECHA, Espaciable::D_ARRIBA, Espaciable::D_IZQUIERDA);
//const Celda::Estructura_rampa Celda::er_techo_di=Estructura_rampa(Espaciable::D_ABAJO, Espaciable::D_IZQUIERDA, Espaciable::D_ARRIBA, Espaciable::D_DERECHA);

Celda::Celda(unsigned int px, unsigned int py, unsigned int pt, unsigned int flags)
	:Representable(),
	x(px), y(py), tipo(pt), flags(flags), 
	debug_considerada_colision(false)
{

}

Celda::Celda(const Celda& o)
	:Representable()
{
	x=o.x;
	y=o.y;
	tipo=o.tipo;
	flags=o.flags;
}

Celda& Celda::operator=(const Celda& o)
{
	Representable::operator=(o);

	x=o.x;
	y=o.y;
	tipo=o.tipo;
	flags=o.flags;
	return *this;
}

Celda::~Celda()
{

}

unsigned short int Celda::obtener_profundidad_ordenacion() const
{
	return 10;
}

void Celda::transformar_bloque(Bloque_transformacion_representable &b) const
{
	unsigned int rx=0;
	unsigned int ry=0;
	unsigned int alpha=127;

	switch(tipo)
	{
		default:
		case T_SOLIDA: rx=0; ry=0; break;
		case T_RAMPA_ARRIBA: rx=16; ry=0; break;

		case T_RAMPA_SUBE_ID: rx=0; ry=16; break;
		case T_RAMPA_SUBE_DI: rx=64; ry=16; break;
		case T_RAMPA_SUBE_ID_23_INI: rx=16; ry=16; break;
		case T_RAMPA_SUBE_ID_23_FIN: rx=32; ry=16; break;
		case T_RAMPA_SUBE_DI_23_INI: rx=96; ry=16; break;
		case T_RAMPA_SUBE_DI_23_FIN: rx=80; ry=16; break;
		case T_RAMPA_TECHO_ID: rx=48; ry=16; break;
		case T_RAMPA_TECHO_DI: rx=112; ry=16; break;
		case T_ESCALERA_NORMAL:
		case T_ESCALERA_SOLIDA: rx=80; ry=0; break;
	}

	if(debug_considerada_colision) alpha+=128;

	b.establecer_tipo(Bloque_transformacion_representable::TR_GRUPO);
	b.establecer_alpha(alpha);

	float px=x*DIM_CELDA, py=y*DIM_CELDA;
	
	DLibV::Representacion_bitmap_dinamica * rep=new DLibV::Representacion_bitmap_dinamica();

	rep->establecer_textura(DLibV::Gestor_texturas::obtener(Recursos_graficos::RT_BLOQUES));
	rep->establecer_recorte(rx, ry, DIM_CELDA, DIM_CELDA);
	rep->establecer_posicion(px, py, DIM_CELDA, DIM_CELDA);

	b.insertar_en_grupo(rep);

	//Y ahora las flags...
	if(es_solida_solo_arriba()) 
	{
		DLibV::Representacion_bitmap_dinamica * r=new DLibV::Representacion_bitmap_dinamica();
		r->establecer_textura(DLibV::Gestor_texturas::obtener(Recursos_graficos::RT_BLOQUES));
		r->establecer_recorte(32, 0, DIM_CELDA, DIM_CELDA);
		r->establecer_posicion(px, py, DIM_CELDA, DIM_CELDA);
		b.insertar_en_grupo(r);		
	}

	if(es_rampa_canto_pasable())
	{
		DLibV::Representacion_bitmap_dinamica * r=new DLibV::Representacion_bitmap_dinamica();
		r->establecer_textura(DLibV::Gestor_texturas::obtener(Recursos_graficos::RT_BLOQUES));
		r->establecer_recorte(48, 0, DIM_CELDA, DIM_CELDA);
		r->establecer_posicion(px, py, DIM_CELDA, DIM_CELDA);
		b.insertar_en_grupo(r);
	}

	if(es_rampa_pico_solido())
	{
		DLibV::Representacion_bitmap_dinamica * r=new DLibV::Representacion_bitmap_dinamica();
		r->establecer_textura(DLibV::Gestor_texturas::obtener(Recursos_graficos::RT_BLOQUES));
		r->establecer_recorte(64, 0, DIM_CELDA, DIM_CELDA);
		r->establecer_posicion(px, py, DIM_CELDA, DIM_CELDA);
		b.insertar_en_grupo(r);
	}

//	b.establecer_tipo(Bloque_transformacion_representable::TR_BITMAP);
//	b.establecer_alpha(alpha);
//	b.establecer_recurso(Recursos_graficos::RG_BLOQUES);
//	b.establecer_recorte(rx, ry, DIM_CELDA, DIM_CELDA);
//	b.establecer_posicion(x*DIM_CELDA, y*DIM_CELDA, DIM_CELDA, DIM_CELDA);
}

/*Indica si la caja está en colisión con esta celda y se usa dentro del Nivel
para meter o no meter esta celda en la lista de colisiones del jugador. Si la 
celda no tiene nada sabemos que no hay colisión. Si la celda es sólida podemos 
elegir entre tendremos que comprobar, puesto que el conjunto de celdas a obtener 
puede salirse un poco por la derecha y por abajo..
Si es una rampa, entramos en un mundo un poco más complejo: según el tipo de
rampa calcularemos si alguna parte del rectángulo está dentro de lo que sería
la parte sólida... 

La parte que corresponde a las rampas tiene también la idea de asumir que sólo 
puedes colisionar con una rampa a la vez (en la mayoría de los casos). Hay algún
caso borderline debido a que puedes colisionar con los cantos de rampas que no
son de 45 grados. Las podríamos dejar fuera, pero creo que lo podemos controlar
aunque no se le de uso real luego.

Básicamente es un caso muy especializado que usaremos sólo para el jugador por
la particularidad de no tener más de una rampa: a la hora de mirar las rampas
se considera el centro de la caja y no los cantos de las mismas, que sería
el modelo que se usa en "colisiona_con_caja_segun_forma".

//TODO: Documentar esto... Realmente se está usando con sólo una rampa a la 
vez???? Mirar el código del controlador.*/

bool Celda::comprobar_colision_con_caja_jugador(const Espaciable::t_caja& c) const
{
	if(!c.es_en_colision_con(copia_caja())) return false;

	float centro=c.origen.x+c.w / 2;
	float x_mundo=obtener_x_mundo();
	float fx_mundo=obtener_xw_mundo();

	bool dentro_de_segmento=centro >= x_mundo && centro < fx_mundo;
	bool dentro_por_derecha=c.origen.x <= fx_mundo && fx_mundo < c.origen.x+c.w;
	bool dentro_por_izquierda=c.origen.x <= x_mundo && x_mundo < c.origen.x+c.w;

	//TODO: Cada vez que veo un switch tipo aquí... bufff...

	switch(tipo)
	{
		default:
		case T_NADA: 
			return false; 
		break;
	
		case T_SOLIDA:  //Tipo sólido completo...
		case T_RAMPA_ARRIBA:
		case T_ESCALERA_NORMAL:
		case T_ESCALERA_SOLIDA:
			return true;
		break;

		//Hay que separar entre rampas con pico y sin pico. Si tienen 
		//pico son colisionables antes...
		case T_RAMPA_TECHO_DI: //Esto es correcto, DI con los ID, por forma el canto está igual.
		case T_RAMPA_SUBE_ID:
		case T_RAMPA_SUBE_ID_23_INI:
			if(es_rampa_pico_solido()) return true;
			else return dentro_de_segmento || dentro_por_derecha; 
		break;

		case T_RAMPA_SUBE_ID_23_FIN: //Puede tener dos cantos... 
			return dentro_de_segmento || dentro_por_derecha || dentro_por_izquierda;
		break;

		case T_RAMPA_TECHO_ID: //Esto es correcto. Ver arriba.
		case T_RAMPA_SUBE_DI:
		case T_RAMPA_SUBE_DI_23_INI:
			if(es_rampa_pico_solido()) return true;
			else return dentro_de_segmento || dentro_por_izquierda;
		break;

		case T_RAMPA_SUBE_DI_23_FIN: //Puede tener dos cantos...
			return dentro_de_segmento || dentro_por_izquierda || dentro_por_derecha;
		break;
	}
}

/*Con que cualquiera de las esquinas de la caja esté en colisión con la parte
sólida de la celda, devolveremos "verdadero".

TODO: Si disparas un proyectil en una rampa ascendente larga, muy cerca del 
borde, se puede o no joder, según entre o no parcialmente. 
*/

bool Celda::colisiona_con_caja_segun_forma(const Espaciable::t_caja& c) const
{
	if(!c.es_en_colision_con(copia_caja())) return false;

	//TODO: Cada vez que veo un switch tipo aquí... bufff...

	switch(tipo)
	{
		default:
		case T_NADA: 
			return false; 
		break;
	
		case T_SOLIDA:  //Tipo sólido completo...
		case T_RAMPA_ARRIBA:
		case T_ESCALERA_NORMAL:
		case T_ESCALERA_SOLIDA:
			return true;
		break;

		//Hay que separar entre rampas con pico y sin pico. Si tienen 
		//pico son colisionables antes...
		case T_RAMPA_SUBE_ID:
		case T_RAMPA_SUBE_ID_23_INI:
		case T_RAMPA_SUBE_ID_23_FIN:
		case T_RAMPA_SUBE_DI:
		case T_RAMPA_SUBE_DI_23_INI:
		case T_RAMPA_SUBE_DI_23_FIN:
		{
			//Los dos puntos inferiores de la caja contra la línea 
			//que generaría cada punto. Si está por debajo es que 
			//hay colisión.	
	
			float y_punto_izq=calcular_y_rampa_ascendente(*this, c.origen.x, false);
			float y_punto_der=calcular_y_rampa_ascendente(*this, c.origen.x+c.w, false);
			float fy=c.origen.y + c.h;

			return y_punto_izq < fy || y_punto_der < fy;
		}
		break;

		case T_RAMPA_TECHO_ID:
		case T_RAMPA_TECHO_DI:
		{
			//Los dos puntos superiores de la caja contra la línea.
			//Si está por encima es que hay colisión.

			float y_punto_izq=calcular_y_rampa_ascendente(*this, c.origen.x, false);
			float y_punto_der=calcular_y_rampa_ascendente(*this, c.origen.x+c.w, false);
			float fy=c.origen.y;

			return y_punto_izq > fy || y_punto_der > fy;
		}
		break;
	}
}

Info_colision Celda::obtener_info_colision(const Espaciable& e, unsigned int dir) const
{
	Info_colision resultado(*this);

	if(es_tipo_rampa()) 
	{
		resultado=obtener_info_colision_rampa_ascendente(e, dir);
	}
	else if(es_tipo_rampa_techo()) 
	{
		resultado=obtener_info_colision_rampa_techo(e, dir);
	}
	else switch(tipo)
	{
		default:
		case T_NADA: break;
		case T_SOLIDA: resultado=obtener_info_colision_caja_solida(dir); break;
		case T_RAMPA_ARRIBA: 
//TODO: Realmente no entiendo que hacía esto aquí...
			if(e.acc_espaciable_fy() < obtener_yh_mundo()) 
			{
				resultado=obtener_info_colision_caja_solida(dir); 
				//resultado=Info_colision(*this); //Saltando al borde de rampa con un poco de la caja dentro de esta celda...
			}
			else
			{
				resultado=obtener_info_colision_caja_solida(dir); 
			}
		break;
	}

	return resultado;
}

/*Este método se usa para calcular los ajustes al colisionar con una rampa. 
Asumimos que "e" es el objeto que ha colisionado con esta celda (el jugador,
mayormente) y que dir es la dirección en la que este objeto se está movimiendo.
Debemos asumir que la posición anterior era válida. En función de esta asunción
podemos determinar varios tipos de comportamiento: chocar con un canto, chocar
con el otro canto o chocar con la rampa (otro es "no chocar con nada", pero 
bueno)... 
Lo que hacemos aquí es "repartir" entre las diferentes respuestas según la
dirección en la que se mueve el espaciable. El cálculo de una corrección con
la rampa es sólo una de las posibles respuestas, que la calculamos en otro
método.*/

Info_colision Celda::obtener_info_colision_rampa_ascendente(const Espaciable& e, unsigned int dir) const
{
	const Espaciable::t_caja c=e.copia_caja();

	Estructura_rampa er=obtener_estructura_rampa();

	//Esto siempre comprobará si estás saltando encima de la rampa o si vienes por debajo de la misma.
	//Es importante no quitar el caso de "saltando al borde", cuando estás entre dos rampas esto hace
	//la diferencia. "Arriba" significa cosas distintas para rampas que suben y bajan (de hecho significa
	//"estar bajando").
	if(dir==er.direccion_arriba)
	{
		if(c.origen.y+c.h < obtener_yh_mundo()) 
		{
			return Info_colision(*this); //Saltando al borde de rampa cuando estás entre dos rampas o una rampa y rampa y fin rampa....
		}
		else 
		{
			return obtener_info_colision_caja_solida(dir); //Saltando bajo rampa, chocas contra el borde inferior.
		}
	}
	else if(dir==er.direccion_bajando_rampa)
	{
		//Chocar con el canto...
		auto es_con_canto=[](const Celda& c, float centro, float base) -> bool
		{
			if(c.es_rampa_canto_pasable()) return false;
			else switch(c.tipo)
			{
				case T_RAMPA_SUBE_ID: 
				case T_RAMPA_SUBE_ID_23_FIN:
					return c.obtener_xw_mundo() < centro;
				break;
				case T_RAMPA_SUBE_ID_23_INI: 
					return c.obtener_xw_mundo() < centro && base > c.obtener_y_mundo() + (DIM_CELDA / 2); 
				break;
				case T_RAMPA_SUBE_DI:
				case T_RAMPA_SUBE_DI_23_FIN:
					return c.obtener_x_mundo() >= centro; 
				break;

				case T_RAMPA_SUBE_DI_23_INI:
					return c.obtener_x_mundo() >= centro && base > c.obtener_y_mundo() + (DIM_CELDA / 2); 
				break;
				default: return true; break;
			}
		};

		//No tengo claro realmente de cómo ocurriría esto... Estoy 
		//moviéndome en la dirección en la que baja la rampa y doy con
		//el inicio de la misma...
		auto es_contra_inicio=[](const Celda& c, float centro, float base) -> bool
		{
			if(c.es_rampa_id()) return c.obtener_x_mundo() > centro;
			else if(c.es_rampa_di()) return c.obtener_xw_mundo() >= centro; 
			else return true;
		};

		float centro=c.origen.x+c.w / 2;
		float base=c.origen.y+c.h;

		if(es_con_canto(*this, centro, base)) 
		{
			return obtener_info_colision_caja_solida(dir);
		}
		else if(es_contra_inicio(*this, centro, base)) 
		{
			return Info_colision(*this);
		}
		else 
		{
			return calcular_correccion_rampa(e, dir);
		}
	}
	else if(dir==er.direccion_subiendo_rampa)
	{
		/*Este método controla(ba) la posibilidad de estar chocando 
		contra el canto de una rampa inacabada (rampas que tienen más
		de una celda) según subimos... Lo hemos declarado ilegal en el
		diseño y el comportamiento es indefinido.		

		auto es_con_canto_rampa_inacabada=[](const Celda& c, float centro, float base) -> bool
		{
			if(!c.es_rampa_canto_solido()) return false;
			else if(c.tipo==T_RAMPA_SUBE_ID_23_FIN || c.tipo==T_RAMPA_SUBE_DI_23_FIN)
			{
				float media_celda=(DIM_CELDA/2);
				float y_critica=c.obtener_y_mundo()+media_celda;

				switch(c.tipo)
				{
					case T_RAMPA_SUBE_ID_23_FIN:
						return c.obtener_x_mundo() > centro && base > y_critica;
					break;

					case T_RAMPA_SUBE_DI_23_FIN:
						return c.obtener_xw_mundo() < centro && base > y_critica;
					break;
					default: return false; break;
				}
			}
			else
			{
				return false;
			}
		};

		float centro=c.origen.x+c.w / 2;
		float base=c.origen.y+c.h;

		if(es_con_canto_rampa_inacabada(*this, centro, base)) return obtener_info_colision_caja_solida(dir);		
		else*/ 

	/*El tema de los picos sólidos de las rampas... Cuando hablamos de pico
	hablamos de, por ejemplo, en una rampa que sube ID, la esquina inferior
	izquierda. Cuando varias rampas están en fila, chocar con el pico de
	una rampa tiene como efecto subir arriba. Puede ocurrir, sin embargo,
	que te encuentres el pico enmedio del aire, en cuyo caso querrías que 
	fuera sólido. Eso es lo que controla "rampa_pico_solido".*/

		if(es_rampa_pico_solido())
		{
			float fy=obtener_yh_mundo();
			float y=c.origen.y+c.h;

			if(y <= fy) 
			{
				return calcular_correccion_rampa(e, dir);
			}
			else 
			{
				return obtener_info_colision_caja_solida(dir);
			}
		}
		else return calcular_correccion_rampa(e, dir);
	}
	else if(dir==er.direccion_abajo)
	{
		return calcular_correccion_rampa(e, dir);
	}
	else
	{
		return Info_colision(*this);
	}
}

/*Estas son las correcciones para las rampa del techo, que se comportan 
ligeramente distintas a las de suelo. Un detalle importante, la dirección sigue
siendo "dirección en la que se está moviendo el actor". Otro detalle que cobra
especial importancia es la flag que indica si el techo no es sólido... No tiene
más sentido que prevenir chocar con la rampa de techo si está justo debajo de 
una rampa ascendente formando una gran rampa.*/

Info_colision Celda::obtener_info_colision_rampa_techo(const Espaciable& e, unsigned int dir) const
{
	const Espaciable::t_caja c=e.copia_caja();
	Estructura_rampa er=obtener_estructura_rampa();

	if(dir==er.direccion_arriba)
	{
		return calcular_correccion_rampa(e, dir);
	}
	else if(dir==er.direccion_bajando_rampa) //Significa que vas en la dirección en la que el techo baja.
	{
		//La construcción de rampas inacabadas es ilegal. Comparar con las rampas ascendentes, cuyo código si lo permite.
		//En esta dirección controlaremos también los picos.

		if(es_rampa_techo_sin_techo())
		{
			return Info_colision(*this);
		}
		else if(es_rampa_pico_solido())
		{
			float fy=obtener_y_mundo();
			float y=c.origen.y;

			if(y > fy) 
			{
				return calcular_correccion_rampa(e, dir);
			}
			else 
			{
				return obtener_info_colision_caja_solida(dir);
			}
		}
		else
		{
			return calcular_correccion_rampa(e, dir);
		}
	}
	else if(dir==er.direccion_subiendo_rampa) //Significa que vas en la dirección en la que el techo sube.
	{
		//El canto es legal, pero si vamos a construir conjuntos de rampas
		//que por arriba son "suben" y por abajo son "techo" las de
		//techo no tendrían canto.

		auto es_con_canto=[](const Celda& c, float centro, float base) -> bool
		{
			if(c.es_rampa_canto_pasable()) return false;
			else switch(c.tipo)
			{
				case T_RAMPA_TECHO_ID: //Con forma de |7
					return c.obtener_x_mundo() > centro;
				break;
				case T_RAMPA_TECHO_DI: //Con forma de \|
					return c.obtener_xw_mundo() <= centro; 
				break;
				default: return true; break;
			}
		};

		auto es_contra_inicio=[](const Celda& c, float centro, float base) -> bool
		{
			if(c.es_rampa_id()) return c.obtener_x_mundo() > centro;
			else if(c.es_rampa_di()) return c.obtener_xw_mundo() >= centro; 
			else return true;
		};

		float centro=c.origen.x+c.w / 2;
		float base=c.origen.y+c.h;

		if(es_rampa_techo_sin_techo())
		{
			return Info_colision(*this);
		}
		else if(es_con_canto(*this, centro, base)) 
		{
			return obtener_info_colision_caja_solida(dir);
		}
		else if(es_contra_inicio(*this, centro, base)) 
		{
			return Info_colision(*this);
		}
		else 
		{
			return calcular_correccion_rampa(e, dir);
		}
	}
	else if(dir==er.direccion_abajo)
	{
		//El tema de rampa_techo_sin techo es útil cuando tienes una rampa
		//que sube y la adornas con una rampa de techo bajo cada uno de los puntos. 
		//Al caminar puedes chocar con el techo de la rampa de techo y terminar
		//dios sabe dónde.

		if(!es_rampa_techo_sin_techo() && c.origen.y < obtener_y_mundo()) 	//Caminando sobre rampa techo...
		{
			return obtener_info_colision_caja_solida(dir); //Saltando bajo rampa, chocas contra el borde inferior.
		}
		else
		{
			return Info_colision(*this);
		}
	}
	else
	{
		return Info_colision(*this);
	}
}

/*Habiendo llegado a este punto podemos asegurar que el centro del espaciable 
está dentro del segmento X de la celda de turno. Por tanto, podemos restar el 
fin del segmento al centro del espaciable para obtener la cantidad de subida 
para una función lineal.*/

Info_colision Celda::calcular_correccion_rampa(const Espaciable& e, unsigned int dir) const
{
	const Espaciable::t_caja c=e.copia_caja();
	float centro=c.origen.x+c.w / 2;
	float y=calcular_y_rampa_ascendente(*this, centro);

	Info_colision ic(*this);
	if(es_tipo_rampa() && y < c.origen.y+c.h) 
	{
		ic.establecer_y(y, Espaciable::D_ABAJO); //La corrección es en y, para snap abajo!.
	}
	else if(es_tipo_rampa_techo() && c.origen.y <= y) 
	{
		ic.establecer_y(y, Espaciable::D_ARRIBA);
	}

	return ic;
}

/*Recibe y devuelve valores absolutos en el mundo. El parámetro recortar 
implica, si es cierto, que los valores fuera del rango de la celda se truncarán
(por ejemplo, está muy fuera por la izquierda en una rampa que sube daría el 
valor mínimo para esa celda).*/

float Celda::calcular_y_rampa_ascendente(const Celda& c, float centro, bool recortar)
{
	centro=round(centro);
	float y=0.0, cantidad=0.0;
	
	if(c.es_rampa_id()) cantidad=centro-c.obtener_x_mundo();
	else if(c.es_rampa_di()) cantidad=c.obtener_xw_mundo()-centro;

	//TODO: Estudiar si asumir que recortamos siempre puede ayudarnos de 
	//alguna manera a arreglar el desastre que tenemos con las rampas.

	if(recortar)
	{
		if(cantidad < 0.0) cantidad=0.0;
		else if(cantidad > DIM_CELDA) cantidad=DIM_CELDA;
	}
	
	switch(c.tipo)
	{
		case T_RAMPA_SUBE_ID:
		case T_RAMPA_SUBE_DI:
		case T_RAMPA_TECHO_ID:
		case T_RAMPA_TECHO_DI:  
			y=c.obtener_yh_mundo()-cantidad; 		
		break;
		case T_RAMPA_SUBE_ID_23_INI: 
		case T_RAMPA_SUBE_DI_23_INI:
			y=c.obtener_yh_mundo()-(cantidad /2); 
		break;
		case T_RAMPA_SUBE_ID_23_FIN: 
		case T_RAMPA_SUBE_DI_23_FIN: 
			y=c.obtener_yh_mundo()-(cantidad /2)-(DIM_CELDA/2);
		break;
	}
	
//	LOG<<"Al calcular_y_rampa_ascendente se devuelve el valor floor("<<y<<")"<<" PARA CENTRO "<<centro<<", CANTIDAD "<<cantidad<<" Y EL TIPO DE RAMPA "<<c.tipo<<std::endl;

	return floor(y);
}

/*Devuelve la posición del mundo Y en la que haríamos snap. La posición es
absoluta. El jugador se encargará de ver si está por debajo o no de su posición
para aplicarla. La posición dada NO es con respecto a la parte baja del jugador.
Este cálculo lo haremos luego.*/

float Celda::obtener_snap_y_para_x(float x) const
{
	if(es_tipo_solido()) return obtener_y_mundo();
	if(es_tipo_rampa_techo()) return obtener_y_mundo();
	else if(es_tipo_rampa()) return calcular_y_rampa_ascendente(*this, x);
	else return 0.0;	//De dónde saldría este caso???
}

/*Recibe un vector de celdas y elimina todas las rampas que hubiera.*/

void Celda::purgar_vector_de_indicadores_rampas(std::vector<const Celda *>& celdas)
{
	bool hay_rampa=false;
	for(const Celda * c : celdas) 
	{
		if(c->es_tipo_rampa()) 
		{
			hay_rampa=true;
			break;
		}
	}
		
	if(hay_rampa)
	{
		auto es_indicador=[](const Celda* c) {return c->es_tipo_indicador_rampa();};
		std::vector<const Celda *>::iterator fin=std::remove_if(celdas.begin(), celdas.end(), es_indicador);
		celdas.erase(fin, celdas.end());
	}
}

/*Recibe un vector de celdas y una caja. Elimina aquellas celdas que sean 
sólidas sólo por arriba y que estén por debajo de la posición de la caja (que
normalemente sería la posición anterior del jugador.*/

void Celda::purgar_vector_de_celdas_solo_solidas_por_arriba_segun_caja(std::vector<const Celda *>& celdas, const Espaciable::t_caja& caja)
{
	auto comprobar_pasables_por_arriba=[&caja](const Celda * cel) -> bool
	{
		if(cel->es_solida_solo_arriba())
		{
			if(Espaciable::es_sobre(caja, cel->copia_caja()) )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else return false;
	};

	std::vector<const Celda *>::iterator fin=std::remove_if(celdas.begin(), celdas.end(), comprobar_pasables_por_arriba);
	celdas.erase(fin, celdas.end());
}
