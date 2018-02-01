#include "controlador.h"

//TODO TODO
extern DLibH::Log_base LOG;
//TODO TODO

Controlador::Controlador(DLibH::Controlador_argumentos& carg)
	:
	controlador_argumentos(carg),
	controlador_fps(60),
	pantalla(640, 480, 0),
	jugador(19.0, 309.0),
	plataforma_union(NULL),
	pausado(false),
	frame_a_frame(false)
{
	cargar_nivel_desde_fichero();
//	obstaculos.push_back(new Obstaculo_generico(672.0, 180.0));
//	obstaculos.push_back(new Obstaculo_generico(544.0, 262.0));
//	plataformas.push_back(new Plataforma_movil(242.0, 176.0, 0));
//	plataformas.push_back(new Plataforma_movil(832.0, 16.0, 1));
}

void Controlador::cargar_nivel_desde_fichero()
{
	std::ifstream fichero("test_nivel.txt", std::ios::in);
	std::string linea=DLibH::Herramientas::obtener_siguiente_linea_archivo(fichero, '*');

	unsigned int y=0;	

	while(linea.size())
	{
		unsigned int t=linea.size();
		unsigned int x=0;

		while(x <= t)
		{
			const char c=linea[x];

			switch(c)
			{
				case '.':
				case ' ': break;
				case 'x': nivel.actualizar_celda(x, y, Celda::T_SOLIDA, 0); break;
				case '#': nivel.actualizar_celda(x, y, Celda::T_SOLIDA, 0); break;
				case '=': nivel.actualizar_celda(x, y, Celda::T_RAMPA_ARRIBA, 0); break;
				case '-': nivel.actualizar_celda(x, y, Celda::T_SOLIDA, Celda::F_SOLIDA_SOLO_ARRIBA); break;
				case '/': nivel.actualizar_celda(x, y, Celda::T_RAMPA_SUBE_ID, Celda::F_NADA); break;
				case 'Z': nivel.actualizar_celda(x, y, Celda::T_RAMPA_SUBE_ID, Celda::F_RAMPA_PICO_SOLIDO); break;

				case '\\': nivel.actualizar_celda(x, y, Celda::T_RAMPA_SUBE_DI, Celda::F_NADA); break;
				case 'S': nivel.actualizar_celda(x, y, Celda::T_RAMPA_SUBE_DI, Celda::F_RAMPA_PICO_SOLIDO); break;

				case '{': nivel.actualizar_celda(x, y, Celda::T_RAMPA_SUBE_ID_23_INI, Celda::F_NADA); break;
				case '<': nivel.actualizar_celda(x, y, Celda::T_RAMPA_SUBE_ID_23_FIN, Celda::F_NADA); break;

				case '}': nivel.actualizar_celda(x, y, Celda::T_RAMPA_SUBE_DI_23_INI, Celda::F_NADA); break;
				case '>': nivel.actualizar_celda(x, y, Celda::T_RAMPA_SUBE_DI_23_FIN, Celda::F_NADA); break;

				case '7': nivel.actualizar_celda(x, y, Celda::T_RAMPA_TECHO_ID, Celda::F_NADA | Celda::F_RAMPA_TECHO_SIN_TECHO); break;
				case 'P': nivel.actualizar_celda(x, y, Celda::T_RAMPA_TECHO_ID, Celda::F_RAMPA_PICO_SOLIDO); break;
				case '9': nivel.actualizar_celda(x, y, Celda::T_RAMPA_TECHO_DI, Celda::F_NADA | Celda::F_RAMPA_TECHO_SIN_TECHO); break;
				case '5': nivel.actualizar_celda(x, y, Celda::T_RAMPA_TECHO_DI, Celda::F_RAMPA_PICO_SOLIDO); break;

				case 'H': nivel.actualizar_celda(x, y, Celda::T_ESCALERA_NORMAL, Celda::F_NADA); break;
				case 'M': nivel.actualizar_celda(x, y, Celda::T_ESCALERA_SOLIDA, Celda::F_NADA); break;

				case 'J': jugador.ajustar_x_izquierda(x*Celda::DIM_CELDA); jugador.ajustar_y_abajo(y*Celda::DIM_CELDA); break;
			}

			++x;
		}

		++y;
		linea=DLibH::Herramientas::obtener_siguiente_linea_archivo(fichero, '*');
	}	
}

void Controlador::inicializar()
{
	//Cargar configuración.
	configuracion.iniciar();

	//Inicializar información y sistemas según configuración.
	id_idioma=configuracion.acc_idioma();

	inicializar_entorno_grafico(pantalla, 600, 400);
	inicializar_entorno_audio();

	//Inicializar recursos.
	Cargador_recursos cargador_recursos(pantalla);
	cargador_recursos.generar_recursos_texturas();
	cargador_recursos.generar_recursos_superficies();
	cargador_recursos.generar_recursos_audio();
	
	//Inicializar controles.
	input.configurar(); 

	//Inicialización controlador tiempo.
	controlador_fps.inicializar();	

	//Inicializar tablas de sprites...
	tabla_sprites_jugador.cargar("data/recursos/ts_jugador.txt");
	jugador.establecer_tabla_sprites(tabla_sprites_jugador);
}

void Controlador::inicializar_entorno_grafico(DLibV::Pantalla& pantalla, unsigned int w, unsigned int h)
{
	configurar_entorno_grafico(pantalla, w, h);
	pantalla.establecer_titulo(" -- == MODELO APLICACION SDL == --");
//	DLibV::Utilidades_graficas_SDL::mostrar_ocultar_cursor(false);
	camara=DLibV::Camara(0, 0, w, h);
}

void Controlador::configurar_entorno_grafico(DLibV::Pantalla& pantalla, unsigned int w, unsigned int h)
{
	pantalla.inicializar(w, h);
	pantalla.establecer_medidas_logicas(w, h);
	pantalla.establecer_modo_ventana(DLibV::Pantalla::M_VENTANA);
}

void Controlador::inicializar_entorno_audio()
{
	Audio::inicializar_entorno_audio(
		configuracion.acc_audio_ratio(), 
		configuracion.acc_audio_salidas(),
		configuracion.acc_audio_buffers(),
		configuracion.acc_audio_canales());

	configurar_entorno_audio();
}

void Controlador::configurar_entorno_audio()
{
	Audio::establecer_volumen(configuracion.acc_volumen_audio());
	Audio::establecer_volumen_musica(configuracion.acc_volumen_musica());
}

Input_usuario Controlador::obtener_input_usuario()
{
	Input_usuario resultado;

	if(input.es_input_pulsado(Input::I_DERECHA)) resultado.mov_horizontal=1;
	else if(input.es_input_pulsado(Input::I_IZQUIERDA)) resultado.mov_horizontal=-1;

	if(input.es_input_pulsado(Input::I_ARRIBA)) resultado.mov_vertical=-1;
	else if(input.es_input_pulsado(Input::I_ABAJO)) resultado.mov_vertical=1;

	if(input.es_input_down(Input::I_SALTAR)) resultado.accion_1=true;
	if(input.es_input_down(Input::I_SACAR_ENFUNDAR_ARMA)) resultado.accion_2=true;

	return resultado;
}

bool Controlador::loop()
{
	float tiempo_frame=controlador_fps.obtener_delta();
	controlador_fps.acumular(tiempo_frame);
	float acumulado=controlador_fps.obtener_acumulado_y_reset();
	//TODO: Este valor debería ser fijo...
	float delta=0.01;

	while(acumulado >= delta)
	{
		input.turno();

		if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE))
		{
			return false;
		}
		else if(input.es_tecla_down(SDL_SCANCODE_P)) 
		{
			pausado=!pausado;	
			break;
		}
 		else if(input.es_tecla_down(SDL_SCANCODE_L)) 
		{
			frame_a_frame=!frame_a_frame;
			break;
		}

		if(pausado) return true;
		if(frame_a_frame && !input.es_tecla_down(SDL_SCANCODE_K)) return true;

		for(Plataforma_movil * p : plataformas) p->turno(delta);
		if(proyectiles.size()) procesar_turno_proyectiles(delta);

		//Sincronizar con el jugador la Y... Desincronizar.
		if(plataforma_union)
		{
			jugador.ajustar_y_abajo(plataforma_union->acc_espaciable_y());			
			jugador.recibir_vector_plataforma(*plataforma_union, delta);
			plataforma_union=NULL;
		}

		Input_usuario IU=obtener_input_usuario();

		if(IU.mov_vertical && !jugador.es_en_escalera()) controlar_subida_escalera(IU.mov_vertical);
		else if(IU.mov_horizontal && jugador.es_en_escalera()) controlar_bajada_escalera(IU.mov_horizontal);

		jugador.recibir_input(IU);

		//Acciones sobre el jugador.

		Espaciable::t_caja anterior=jugador.copia_caja();

		jugador.turno(delta);

		if(jugador.es_en_escalera())
		{
			jugador.movimiento(Espaciable::T_Y, delta);
			controlar_colisiones_jugador_escalera();
			controlar_fin_movimiento_vertical_jugador();
		}
		else if(jugador.es_con_arma())
		{
			jugador.turno_arma();
		}
		else
		{
			jugador.movimiento(Espaciable::T_X, delta);
			controlar_colisiones_jugador(Espaciable::T_X, anterior);
			controlar_fin_movimiento_horizontal_jugador();

			anterior=jugador.copia_caja();
			jugador.movimiento(Espaciable::T_Y, delta);
			controlar_colisiones_jugador(Espaciable::T_Y, anterior);
			controlar_fin_movimiento_vertical_jugador();
		}

		//Disparar??
		if(jugador.es_generar_proyectil()) generar_disparo_jugador();

		acumulado-=delta;
	}

	//Fin de acciones sobre el jugador.
	controlador_fps.acumular(acumulado);
	controlador_fps.turno();

	dibujar();
//	for(int i=0; i<30; ++i) dibujar();

	pantalla.actualizar();
	Audio::procesar_cola_sonido();

	return true;
}

/*Este es el método de detección y respuesta de colisiones para el jugador.
Es grande y tiene varios métodos lambda dentro. Se usa para los estados de
aire y tierra. El estado de escalera es específico y funciona de otra forma.*/

void Controlador::controlar_colisiones_jugador(unsigned int t, const Espaciable::t_caja& caja_original)
{
	DLibH::Vector_2d v=jugador.acc_vector();

	void (Jugador::*metodo_ajuste)(float)=NULL;
	unsigned int direccion_colision=0; //Al escoger la dirección es importante pensar que si voy "bajando" quiero saber si algo es sólido "por arriba".
	unsigned int direccion_movimiento=0;
	unsigned int coordenada_ordenacion=0;
	bool mayor=true;

	switch(t)
	{
		case Espaciable::T_X:
			coordenada_ordenacion=Ordenador_info_colision::OX;

			if(v.x > 0)
			{
				mayor=true;
				direccion_colision=Espaciable::D_IZQUIERDA;
				direccion_movimiento=Espaciable::D_DERECHA;
			}
			else if(v.x < 0)
			{
				mayor=false;
				direccion_colision=Espaciable::D_DERECHA;
				direccion_movimiento=Espaciable::D_IZQUIERDA;
			}
			else return;
		break;

		case Espaciable::T_Y:
			coordenada_ordenacion=Ordenador_info_colision::OY;

			if(v.y > 0)
			{
				mayor=false;
				direccion_colision=Espaciable::D_ARRIBA;
				direccion_movimiento=Espaciable::D_ABAJO;
			}
			else if(v.y < 0)
			{
				mayor=true;
				direccion_colision=Espaciable::D_ABAJO;
				direccion_movimiento=Espaciable::D_ARRIBA;
			}
			else return;
		break;
	}

	Espaciable::t_caja caja=jugador.copia_caja();

	//Esta lambda determina si un espaciable entrará o no en la colisión.

	//TODO: CREO QUE ESTO ES SUPERFLUO!!!!!. Si no lo es, please, explicar.
	auto proceso_colision=[this, &caja, &direccion_colision](const std::vector<const Espaciable *>& origen, std::vector<const Espaciable *>& destino)
	{
		for(const Espaciable * c : origen)
		{
			if(caja.es_en_colision_con(c->copia_caja()) ) 
				destino.push_back(c);
		}
	};

	//Con cajas... Se obtienen todas las cajas para las cuales el jugador está
	//en contacto, incluyendo las formas de rampa, por ejemplo. Si alguna de
	//las cajas es una rampa, quitaría las adjuntas a las rampas. Al final
	//quedarán sólo las considerables... Purgamos también todas aquellas que
	//sean sólidas sólo por arriba y que queden por debajo.

	std::vector<const Celda *> todas_cajas=nivel.celdas_para_caja(caja);
	Celda::purgar_vector_de_indicadores_rampas(todas_cajas);
	Celda::purgar_vector_de_celdas_solo_solidas_por_arriba_segun_caja(todas_cajas, caja_original);

	std::vector<const Espaciable *> colisiones_cajas;
	std::vector<const Espaciable *> todas_cajas_espaciables; //Vamos a convertir esas celdas en "espaciables".
	todas_cajas_espaciables.insert(todas_cajas_espaciables.begin(), todas_cajas.begin(), todas_cajas.end());

	proceso_colision(todas_cajas_espaciables, colisiones_cajas);

	//Con actores...
	std::vector<const Espaciable *> todos_actores;
	std::vector<const Espaciable *> colisiones_actores;

	//Los obstáculos estáticos? Todos.
	todos_actores.insert(todos_actores.begin(), obstaculos.begin(), obstaculos.end());

	//Los móviles irán sólo los que procedan. De momento capamos al eje Y,
	//puesto que serían pasables por debajo... 

	if(t==Espaciable::T_Y) obtener_plataformas_moviles(caja_original, todos_actores);

	proceso_colision(todos_actores, colisiones_cajas);
	
	unsigned int tam=colisiones_cajas.size()+colisiones_actores.size();

	if(tam)
	{
		std::vector<const Espaciable *> colisiones;
		colisiones.reserve(tam);
		colisiones.insert(colisiones.end(), colisiones_cajas.begin(), colisiones_cajas.end());
		colisiones.insert(colisiones.end(), colisiones_actores.begin(), colisiones_actores.end());

		//En este punto vamos a comprobar las colisiones contra todos
		//los espaciables que tenemos guardados. Para cada uno de ellos
		//obtendremos la información de colisión. Luego ordenaremos 
		//estas informaciones para obtener la más relevante.

		std::vector<Info_colision> info;

		for(const Espaciable * e : colisiones) 
			info.push_back(e->obtener_info_colision(jugador, direccion_movimiento));

		//La ordenación se hace de menor a mayor, siendo el primer criterio
		//"hay ajuste en el eje principal" y el segundo variable (es mayor?
		//es menor?) según se ha especificado en el ordenador. Si chocamos
		//en una rampa con una pared al comprobar el eje X ignorará la rampa 
		//porque el eje X tiene prioridad sobre el eje Y del ajuste de
		//la rampa.

		Ordenador_info_colision ord(mayor, coordenada_ordenacion);
		std::sort(info.begin(), info.end(), ord);
		Info_colision &ic=info.back();

		Datos_colision dc=ic.espaciable->obtener_datos_colision();
		if(dc.plataforma_movil) plataforma_union=dc.plataforma_movil;

		//Los ajustes con estos métodos suponen también cambios en el estado
		//del jugador, como el vector o el estado en si (aire, tierra...).

		if(ic.ajuste_x) 
		{
			metodo_ajuste=ic.dir_ajuste_x==Espaciable::D_IZQUIERDA ? &Jugador::ajustar_x_izquierda : &Jugador::ajustar_x_derecha;
			(jugador.*metodo_ajuste)(ic.x);
		}

		if(ic.ajuste_y) 
		{
			metodo_ajuste=ic.dir_ajuste_y==Espaciable::D_ARRIBA ? &Jugador::ajustar_y_arriba : &Jugador::ajustar_y_abajo;
			(jugador.*metodo_ajuste)(ic.y);
		}
	}
}

/*Cuando estamos en una escalera se aplican unas normas ligeramente distintas.
Para empezar, como la escalera es una celda sabemos que la X del jugador va a
estar fija en una posición, de modo que sólo tenemos que encargarnos de las
celdas que haya en lo alto y en lo bajo de la caja de colisión.

La caja de colisión es especial: se una una caja de menor altura que la del 
jugador, para permitir que los "pies" queden colgando y que no sea tan rígido.

El funcionamiento es sencillo: cualquier cosa que NO sea una escalera se 
considera una colisión. Cuando la colisión es "por arriba" nos detenemos.
Cuando es por abajo, nos bajamos de la escalera. */

void Controlador::controlar_colisiones_jugador_escalera()
{
	DLibH::Vector_2d v=jugador.acc_vector();

	//Si no hay vector, a casa.
	if(!v.y) 
	{
		return;
	}
	//Si vamos subiendo y la celda superior no es una escalera, detenemos el movimiento, actuando como una colisión normal.
	else if(v.y < 0)
	{
		Espaciable::t_caja caja=jugador.obtener_caja_escalera();
		Celda &c=nivel.obtener_celda_en_coordenadas(caja.origen.x, caja.origen.y);

		if(!c.es_tipo_escalera())
		{
			Info_colision i=c.obtener_info_colision_caja_solida(Espaciable::D_ARRIBA);
			if(i.ajuste_y) jugador.ajustar_y_arriba_sin_estado(i.y);
		}
	}
	//Si vamos bajando y la celda inferior no es una escalera, nos bajamos.
	else
	{
		//Empezamos por la caja completa: tiene sentido puesto que podemos
		//estar contra algo sólido (nos paramos) o contra el final de una
		//escalera colgando... Si vemos que lo que hay abajo NO es una 
		//escalera miraremos si es sólido.

		Espaciable::t_caja caja=jugador.copia_caja();
		Celda &c=nivel.obtener_celda_en_coordenadas(caja.origen.x, caja.origen.y+caja.h);

		if(!c.es_tipo_escalera())
		{
			if(c.es_tipo_solido())
			{
				Info_colision i=c.obtener_info_colision_caja_solida(Espaciable::D_ABAJO);
				if(i.ajuste_y) jugador.ajustar_y_abajo_sin_estado(i.y);				
				jugador.establecer_en_suelo();
			}
			else
			{
				//Lo que hay en los pies no es sólido, puede que estemos en una escalera
				//colgante... Podemos seguir bajando hasta que esta otra caja, más pequeña,
				//nos tire.

				Espaciable::t_caja caja_e=jugador.obtener_caja_escalera();
				Celda &ce=nivel.obtener_celda_en_coordenadas(caja_e.origen.x, caja_e.origen.y+caja_e.h);
				if(!ce.es_tipo_escalera()) jugador.establecer_en_aire();
			}
		}
	}
}

/*
realizar un movimiento en eje X:

Cuando el movimiento termina:

	Si el movimiento ha tenido una corrección en Y:
		coloca en tierra.
		return

	En caso contrario,
		Si el estado es en el aire:
			return
		Si el estado es en tierra:
			Obtener celdas en el suelo, ignora marcadores si hay rampas
			Hay alguna rampa y está en el centro? 
				Aplica rampa, coloca en tierra.
				return
			De lo contrario.
				Obtener celdas 1px por debajo del suelo, ignora marcadores si hay rampas.				
				Hay alguna rampa y está en el centro?
					Aplica rampa, coloca en tierra.
					return
				No hay ninguna celda?
					Coloca en aire.
					return;
				Hay sólidas (estoy saliendo de una rampa)
					Coloca en tierra, haz snap!!
					return
				Cualquier otro caso nos deja justo como estamos.

Este método es vital para el juego: a cada paso que se da en el Eje y hay 
posibilidades de caer, seguir una rampa o mantenerse en tierra (que tiene mucha
importancia para aplicar la gravedad). Todos estos casos se controlan aquí
de forma muy pesada. Si hay algún sitio donde se podría optimizar mucho sería
por aquí.

//TODO: Creo que aquí hay espacio para mejorar.
*/

void Controlador::controlar_fin_movimiento_horizontal_jugador()
{
	if(jugador.es_en_suelo())
	{
		//Esta caja está a nivel de los pies: 1 px por encima de lo que esté pisando.
		Espaciable::t_caja suelo=jugador.obtener_caja_suelo();
		std::vector<const Celda *> celdas=nivel.celdas_para_caja(suelo);		

		Celda::purgar_vector_de_indicadores_rampas(celdas);
		suelo.origen.y=floor(suelo.origen.y);
		float centro=round(suelo.origen.x + (suelo.w / 2));

		std::vector<const Celda *> rampas;

		//Hay alguna rampa y está en el centro?. Un one liner :D.
		auto hay_rampa_en_centro=[centro, &rampas](const Celda* c) {if(c->es_tipo_rampa() && c->acc_espaciable_x() <= centro && centro <= c->acc_espaciable_fx()) rampas.push_back(c);};

		std::for_each(celdas.begin(), celdas.end(), hay_rampa_en_centro);

		if(rampas.size())
		{
			jugador.ajustar_y_abajo(rampas[0]->obtener_snap_y_para_x(centro));	//Esto ya pone el estado del jugador como "suelo".
		}
		else
		{
			suelo.origen.y++;
			celdas=nivel.celdas_para_caja(suelo);		

			if(!celdas.size())
			{
				//Habiendo llegado a este punto podemos considerar tres casos:
				//1. la detección de rampas tiene sus fallos y estamos bajando una rampa pasando de una celda de rampa a otra.
				//2. estamos terminando de bajar una rampa y entrando en suelo sólido.
				//3. Estamos andando por encima de una escalera... Puede ser la parte "caminable" o "no caminable".
				//3. realmente estamos cayendo.

				Celda& cf=nivel.obtener_celda_en_coordenadas(centro, suelo.origen.y+1);
			
				if(cf.es_tipo_rampa())
				{
					float y=cf.obtener_snap_y_para_x(centro);
					jugador.ajustar_y_abajo(y);	//Esto ya pone el estado del jugador como "suelo".
				}
				else if(cf.es_tipo_solido() || cf.es_tipo_rampa_techo() || cf.es_tipo_escalera_solida())
				{
					jugador.ajustar_y_abajo(cf.acc_espaciable_y());
				}
				else if(cf.es_tipo_escalera_no_solida())
				{
					jugador.establecer_en_aire();					
				}
				else
				{
					jugador.establecer_en_aire();
				}
			}
			else
			{
				Celda::purgar_vector_de_indicadores_rampas(celdas);
				std::for_each(celdas.begin(), celdas.end(), hay_rampa_en_centro);

				//Aquí hay una pequeña posibilidad borderline...
				if(celdas.size()==1 && celdas[0]->es_tipo_escalera_no_solida())
				{
					jugador.establecer_en_aire();
				}
				else if(rampas.size())
				{
					float y=rampas[0]->obtener_snap_y_para_x(centro);
					jugador.ajustar_y_abajo(y);	//Esto ya pone el estado del jugador como "suelo".
				}
				else
				{
					jugador.ajustar_y_abajo(celdas[0]->acc_espaciable_y());	//Esto ya pone el estado del jugador como "suelo".		
				}
			}
		}
	}
}

/*Después de un movimiento vertical, sin estamos en escalera, cancelamos el 
vector y del jugador.*/

void Controlador::controlar_fin_movimiento_vertical_jugador()
{
	if(jugador.es_en_escalera())
	{
		//TODO: Esto es SUPER redundante... Pero no hay método "cancelar vector y".
		jugador.establecer_en_escalera();
	}
}

void Controlador::dibujar()
{
	pantalla.limpiar(128, 128, 128, 255);
	camara.enfocar_a(jugador.acc_espaciable_x()-300, jugador.acc_espaciable_y()-200);
	
	//camara.enfocar_a(224, 0);

	//TODO: PQ FALLA CON pure virtual method cuando el Representable es const????

	std::vector<Representable *> vr;
	nivel.obtener_celdas_representables(vr);
	vr.push_back(&jugador);
	for(Obstaculo_generico * o : obstaculos) vr.push_back(o);
	for(Plataforma_movil * p : plataformas) vr.push_back(p);
	for(Proyectil * p : proyectiles) vr.push_back(p);

	Ordenador_representables ordenador;
	std::sort(vr.begin(), vr.end(), ordenador);
	representador.generar_vista(pantalla, camara, vr);

	//TODO: Esto no es real... Falta el desplazamiento de la cámara.

	int x_raton=floor(input.x_raton() / Celda::DIM_CELDA);
	int y_raton=floor(input.y_raton() / Celda::DIM_CELDA);

	std::string debug=" RAT: "+DLibH::Herramientas::entero_a_cadena(x_raton)+
			", "+DLibH::Herramientas::entero_a_cadena(y_raton)+
			" pos("+DLibH::Herramientas::float_a_cadena(jugador.acc_espaciable_x())+
			","+DLibH::Herramientas::float_a_cadena(jugador.acc_espaciable_y())+
			") fpos("+DLibH::Herramientas::float_a_cadena(jugador.acc_espaciable_fx())+
			","+DLibH::Herramientas::float_a_cadena(jugador.acc_espaciable_fy())+
			")\n vel("+DLibH::Herramientas::float_a_cadena(jugador.acc_vector().x)+
			","+DLibH::Herramientas::float_a_cadena(jugador.acc_vector().y)+")"+
			"FPS:"+DLibH::Herramientas::entero_a_cadena(controlador_fps.acc_frames_contados());

	DLibV::Representacion_texto_fija_estatica txt(pantalla.acc_renderer(), DLibV::Gestor_superficies::obtener(Recursos_graficos::RS_FUENTE_BASE), debug, 600, 20);
	txt.establecer_posicion(0, 6);
	txt.volcar(pantalla);

	SDL_Rect cp=DLibH::Herramientas_SDL::nuevo_sdl_rect(
		jugador.acc_espaciable_x()-camara.acc_x(), 
		jugador.acc_espaciable_y()-camara.acc_y(), 
		jugador.acc_espaciable_w(), 
		jugador.acc_espaciable_h()); 

	DLibV::Representacion_primitiva_caja_estatica CAJA(cp, 255, 0, 0);
	CAJA.establecer_alpha(128);
	CAJA.volcar(pantalla);

	for(Proyectil * p : proyectiles) 
	{	
		Espaciable::t_caja c=p->copia_caja();
		CAJA.establecer_alpha(128);
		CAJA.establecer_posicion(c.origen.x-camara.acc_x(), c.origen.y-camara.acc_y(), c.w, c.h);
		CAJA.volcar(pantalla);
	}
}

/*De entre todas las plataformas móviles sólo vamos a escoger aquellas que estén
realmente por debajo del jugador. Se podrían, por tanto, traspasar desde abajo.
La caja que recibimos no es la posición actual, sino la posición antes de haber
procesado el eje de turno.*/

void Controlador::obtener_plataformas_moviles(const Espaciable::t_caja& caja, std::vector<const Espaciable *>& destino)
{
	for(const Espaciable * e : plataformas)
	{		
		if(Espaciable::es_sobre(caja, e->copia_caja()) )
			destino.push_back(e);
	}
}

/*Cuando se recibe una pulsación de "arriba" o "abajo" y el jugador no está
en una escalera controlamos la posibilidad de subir a una. Si el jugador está
completamente dentro de una celda de escalera estableceremos su estado y 
lo centraremos en la escalera en si. 

Hay un detalle a tener en cuenta: si estás en el suelo, con una escalera en la
celda actual y dejas pulsado abajo, alternarías entre los estados de escalera
y soltarse. Esta particularidad la controlamos aquí.*/

void Controlador::controlar_subida_escalera(short int dir)
{
	Espaciable::t_caja caja=jugador.obtener_caja_escalera();
	std::vector<const Celda *> todas_cajas=nivel.celdas_para_caja(caja);
	float centro=round(caja.origen.x + (caja.w / 2));
	
	//Hay alguna escalera y está en el centro?. Un one liner :D.
	auto hay_escalera_en_centro=[centro](const Celda* c) -> bool {return c->es_tipo_escalera() && c->acc_espaciable_x() <= centro && centro <= c->acc_espaciable_fx();};

	//Para que entremos, todas las celdas deben ser del tipo escalera. No es posible agarrar
	//sólo la parte de abajo o la parte de arriba.

	unsigned int escaleras=std::count_if(todas_cajas.begin(), todas_cajas.end(), hay_escalera_en_centro);

	//Ojo, vamos a comprobar también si hay resultados!!!.
	if(escaleras && escaleras==todas_cajas.size())
	{
		//Una última comprobación, si hemos hecho click "abajo", el
		//jugador está en tierra y en la celda de abajo no hay escalera,
		//te vas a tu casa.

		if(jugador.es_en_suelo() && dir > 0)
		{
			Espaciable::t_caja caja_s=jugador.obtener_caja_suelo();
			DLibH::Punto_2d<int> p=Nivel::rejilla_para_coordenadas(caja_s.origen.x, caja_s.origen.y);
			if(p.x >= 0 && p.y >= 0)
			{
				Celda& c=nivel.obtener_celda_en_rejilla(p.x, p.y+1); //Sacamos la siguiente.
				if(!c.es_tipo_escalera()) return;							
			}
		}

		jugador.establecer_en_escalera();
		jugador.centrar_en_escalera(*todas_cajas[0]);
	}
}

/*Este método controla cuando, desde lo alto de una escalera, hemos pulsado 
izquierda o derecha. Si tenemos a los pies una escalera sólida por debajo o una
celda sólida vamos a bajar de la escalera. En función de si estamos bajando de
una escalera hacia "arriba" o hacia "abajo" hay dos distancias máximas a las
que podemos estar.*/

void Controlador::controlar_bajada_escalera(short int dir)
{
	//La celda a los pies...
	Espaciable::t_caja caja_s=jugador.obtener_caja_suelo();
	DLibH::Punto_2d<int> p=Nivel::rejilla_para_coordenadas(caja_s.origen.x, caja_s.origen.y);
	if(p.x >= 0 && p.y >= 0)
	{
		Celda& c=nivel.obtener_celda_en_rejilla(p.x, p.y+1); //Sacamos la siguiente.
		if(c.es_tipo_solido() || c.es_tipo_escalera_solida()) 	
		{
			float suelo=c.acc_espaciable_y();
			if(suelo - jugador.acc_espaciable_fy() <= Jugador::DIST_MAXIMA_BAJAR_ESCALERA) 
				jugador.ajustar_y_abajo(suelo);
		}
		else
		{
			//Opción B, el jugador está directamente con los pies en una celda escalera sólida: está al final de una escalera.
			Celda& ce=nivel.obtener_celda_en_rejilla(p.x, p.y);
			if(ce.es_tipo_escalera_solida())
			{
				float suelo=ce.acc_espaciable_y();
				if(jugador.acc_espaciable_fy() - suelo <= Jugador::DIST_MAXIMA_SUBIR_ESCALERA)
					jugador.ajustar_y_abajo(suelo);
			}
		}			
	}
}

void Controlador::generar_disparo_jugador()
{
	//TODO: Y si es múltiple?. Podemos generar una interface para ello.
	Origen_proyectil OP=jugador.generar_info_proyectil();
	Proyectil * pr=new Proyectil();
	if(pr)
	{
		pr->configurar(OP.x, OP.y, OP.vx, OP.vy, OP.velocidad);
		proyectiles.push_back(pr);	
	}
}

void Controlador::procesar_turno_proyectiles(float delta)
{
	unsigned int borrar=0;
	Espaciable::t_caja caja(0,0,0,0);

	for(Proyectil * p : proyectiles) 
	{
		p->turno(delta);
		caja=p->copia_caja();

		std::vector<const Celda *> vc=nivel.celdas_para_caja(caja);

		if(vc.size())
		{
			for(const Celda * c : vc)
			{
				if(c->es_traspasable_por_proyectiles()) continue;
				else if(c->colisiona_con_caja_segun_forma(caja))
				{
					p->marcar_para_borrar();
					++borrar;
					break;
				}
			}
		}
	}

	if(borrar)
	{
//		procesar_borrado_actores_con_cantidad(proyectiles, borrar);
		std::vector<Proyectil *>::iterator ini=proyectiles.begin(), fin=proyectiles.end();
		while(borrar && ini < fin)
		{
			if((*ini)->es_para_borrar())
			{
				delete *ini;
				ini=proyectiles.erase(ini);
				fin=proyectiles.end();
				--borrar;
			}
			else
			{
				++ini;
			}
		}
	}
}
/*
void Controlador::procesar_borrado_actores_con_cantidad(std::vector<Actor *>& v, unsigned int cantidad)
{
	std::vector<Actor *>::iterator ini=v.begin(), fin=v.end();

	while(cantidad && ini < fin)
	{
		if((*ini)->es_para_borrar())
		{
			delete *ini;
			ini=v.erase(ini);
			fin=v.end();
			--cantidad;
		}
		else
		{
			++ini;
		}
	}
}*/
